#include "types.h"
#include "target.h"
#include "distorm.h"
#include "state.h"

#include <stdlib.h>
#include <sys/user.h>     /* struct user */
#include <assert.h>       /* assert */
#include <errno.h>        /* errno */
#include <signal.h>       /* kill */
#include <stdio.h>        /* perror */
#include <sys/mman.h>
#include <sys/personality.h> // aslr
#include <sys/ptrace.h>   /* ptrace */
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>       /* getpid */
#include <wait.h>         /* waitpid */


#define MAXSTRLEN 256

unsigned char mpsyc [] = {0xb8, 0x0a, 0, 0, 0, 0x0f, 0x05, 0x5d, 0xc3, 0x55, 0x48};
/*
 *This would be better but qt messes everything up :(
 void mprotect_syscall()
 {
 asm("syscall;"  : : "a"(0xa));
 }
 void mprotect_syscall_end() {}
 */

class NoInit : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return "INIT this";
		}
};

T::~T()
{
	delete elf_;
	delete bp_;
	std::map<addr_t, _DInst*>::iterator it = rip_i_.begin();
	for( ; it!=rip_i_.end(); ++it)
		delete it->second;
	delete initial_state_;
}

int T::init(char** argv)
{
	if(init_) return 1;
	pid_ = fork();
	if(pid_ == 0) { // child
		// disable aslr for child .. just for fun
		int orig_personality = personality(0xffffffff);
		personality(orig_personality|ADDR_NO_RANDOMIZE);
		fprintf(stderr, "+%5d+ created child\n", getpid());
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		// execute target (will cause a SIGTRAP)
		execv(argv[0], argv);
		perror("execv");
	} else {
		init_ = true;

		ptrace(PTRACE_SETOPTIONS, pid_, NULL,
				PTRACE_O_TRACEFORK);
		waitpid(pid_, NULL, 0);

		// break on main
		elf_ = new Elf(argv[0]);
		bp_ = new Break(T::arget().pid());
		addr_t main = elf_->get_func((char*)"main");
		bp_->set(main);
		safe_ptrace(PTRACE_CONT, 0, NULL);
		int status;
		waitpid(pid_, &status, 0);
		// lets just hope that we hit out breakpoint ...
		bp_->unset(main);
		singlestep();
		singlestep();
		singlestep();
		singlestep();
		singlestep();
		// TODO ... number of steps??

		// find code segment
		char buf[256];
		sprintf(buf, "/proc/%d/maps", pid_);
		FILE* fp = fopen(buf, "r");
		if(fp==0) {
			perror("fopen");
			exit(1);
		}
		fscanf(fp,"%lx-%lx",
				&code_start_, &code_stop_);
		while(fgets(buf, sizeof(buf), fp)) {
			if(strstr(buf, "stack")!=NULL) {
				sscanf(buf,"%lx-%lx",
						&stack_start_, &stack_stop_);
			}    
		} 
		fclose(fp);

		// store initial state
		struct user_regs_struct regs;
		safe_ptrace(PTRACE_GETREGS, 0, &regs);
		initial_state_ = new State(regs, Memstate(0,0), Memstate(stack_start_, stack_start_+256));
		//code_start_ = elf_->get_func((char*)"_start");
		// TODO: allgemein richtig?
		code_start_ = elf_->get_func((char*)"frame_dummy") + 0x2c;
		code_stop_ = elf_->get_func((char*)"__libc_csu_init");
		fprintf(stderr, "+%5d+ code segment %lx-%lx\n", pid_, code_start_, code_stop_);
		fprintf(stderr, "+%5d+ stack segment %lx-%lx\n", pid_, stack_start_, stack_stop_);

		syscall_addr_ = elf_->get_func((char*)"_start");
		//char* mp = (char*)&mprotect_syscall+4;
		unsigned char* mp = mpsyc;
		//size_t protect_stack_none_len = (addr_t)mprotect_syscall_end - (addr_t)mprotect_syscall-2;
		size_t protect_stack_none_len = 11;
		fprintf(stderr, "Writing mprotect at %lx (%ld bytes)\n", syscall_addr_, protect_stack_none_len);

		write(syscall_addr_, 
				(void*)mp, protect_stack_none_len);

		const size_t nInst = (code_stop_ - code_start_); // TODO
		unsigned char my_code_stream[nInst];
		_DInst result[nInst];
		memset(result, 0, nInst*sizeof(_DInst));
		unsigned int instructions_count = 0;

		read(code_start_, (void*)my_code_stream, nInst);

		_CodeInfo ci = {0};
		ci.code = my_code_stream;
		ci.codeLen = sizeof(my_code_stream);
		ci.dt = Decode64Bits; // TODO evaluate this (prob in elf class)
		ci.codeOffset = code_start_;
		ci.features = 0;

		distorm_decompose(&ci, result, sizeof(result)/sizeof(result[0]), &instructions_count);

		std::vector<addr_t> cjmp;
		// well, if instruction_count == 0, we won't enter the loop.
		for (unsigned int i = 0; i < instructions_count; i++) {
			if (result[i].flags == FLAG_NOT_DECODABLE) {
				// handle instruction error!
				//break;
				//fprintf(stderr, "ERROR decoding\n");
				continue;
			}


			if(META_GET_FC(result[i].meta) == FC_CND_BRANCH) {
				fprintf(stderr, "conditional jump at %lx\n", result[i].addr);
				cjmp.push_back(result[i].addr);
			}
			_DInst* inst = new _DInst;
			*inst = result[i];
			rip_i_.insert(std::pair<addr_t, _DInst*>(
						inst->addr, inst));
			//_DecodedInst di;
			//distorm_format(&ci, inst, &di);
			//printf("%lx: %s %s\n", inst->addr, di.mnemonic.p, di.operands.p);
		}
		for(size_t i=0; i<cjmp.size(); ++i)
			bp_->set(cjmp[i]);
		ci_ = ci;

	}
	return 0;
}
bool T::inCode(addr_t loc) const
{
	return loc<code_stop_ && loc>=code_start_;
}
bool T::inStack(addr_t loc) const
{
	return loc<=stack_stop_ && loc>=stack_start_;
}

int T::singlestep()
{
	if(!init_) {NoInit exp; throw(exp);}
	safe_ptrace(PTRACE_SINGLESTEP, 0, NULL);
	int status;
	waitpid(pid_, &status, 0);
	if(WIFEXITED(status))
	{
		fprintf(stderr, "+%5d+ child exit status %d\n", 
				pid_, WEXITSTATUS(status));
		exit(0);
	}
	siginfo_t si;
	safe_ptrace(PTRACE_GETSIGINFO, 0, &si); 
	struct user_regs_struct regs;
	safe_ptrace(PTRACE_GETREGS, 0, &regs);
	if(si.si_signo!=SIGTRAP) {
		fprintf(stderr, "SI at %lx\n", regs.rip);
		char i[16];
		read(regs.rip, i, 16);
		for(int j=0; j<8; ++j)
			fprintf(stderr, "%2x ", 0xff&i[j]);
		fprintf(stderr, "\n");
		for(int j=8; j<16; ++j)
			fprintf(stderr, "%2x ", 0xff&i[j]);
		fprintf(stderr, "\n");
	}
	return si.si_signo;
}

int T::write(addr_t addr, void *vptr, size_t len) const
{
	if(!init_) {NoInit exp; throw(exp);}
	//assert(len>sizeof(long));

	size_t i, j;
	long new_word;
	long orig_word;
	unsigned char* ptr = (unsigned char*)vptr;
	i = j =0;
	while (i < len) {
		new_word = *((long*)(ptr + i));
		orig_word = safe_ptrace(PTRACE_PEEKTEXT, addr + i, NULL);
		for(j=0; j<sizeof(long) && j+i<len; ++j)
			((unsigned char*)&orig_word)[j] = ((unsigned char*)&new_word)[j];
		safe_ptrace(PTRACE_POKETEXT, addr + i, (void*)orig_word);
		i+=j;
	}
	return i;
}

int T::read(addr_t addr, void *vptr, size_t len) const
{
	if(!init_) {NoInit exp; throw(exp);}
	size_t i, j;
	long word;
	unsigned char* ptr = (unsigned char*)vptr;
	i = j = 0;
	while (i < len) {
		word = safe_ptrace(PTRACE_PEEKTEXT, addr + i, NULL);
		for(j=0; j<sizeof(word) && j+i<len; j++) 
			ptr[i+j] = ((unsigned char*)&word)[j];
		i+=j;
	}
	return i;
}

size_t T::getStrLen(addr_t loc)
{
	if(!init_) {NoInit exp; throw(exp);}
	size_t n = sizeof(addr_t);
	char* str = new char[n];
	size_t cntr = 0;
	while(cntr < MAXSTRLEN) {
		read(loc, str, n);
		for(size_t i=0; i<n; ++i)
			if(str[i]==0) {
				delete str;
				return cntr+i;
			}
		cntr += n;
	}
	delete str;
	return 0;
}

addr_t T::safe_ptrace(int request, addr_t addr, void* data) const
{
	if(!init_) {NoInit exp; throw(exp);}
	addr_t result;
	errno = 0;
	result = ptrace((__ptrace_request)request, pid_, addr, data);
	if(errno != 0){
		perror("ptrace");
		exit(1);
	}else{
		return result;
	}
}

int T::protect_stack(int prot)
{
	if(!init_) {NoInit exp; throw(exp);}
	//if(last_stack_prot_ == prot)
	//	return 0;
	last_stack_prot_ = prot;
	struct user_regs_struct orig_regs, regs;
	safe_ptrace(PTRACE_GETREGS, 0, &orig_regs);
	regs = orig_regs;
	regs.rip = syscall_addr_;
	regs.rdi = stack_start_;
	regs.rsi = stack_stop_ - stack_start_;
	regs.rdx = prot;
	safe_ptrace(PTRACE_SETREGS, 0, &regs);
	safe_ptrace(PTRACE_GETREGS, 0, &regs);
	for(size_t i=0; i<2; ++i) {
		safe_ptrace(PTRACE_GETREGS, 0, &regs);
		if(singlestep()!=5)
			exit(1);
	}
	safe_ptrace(PTRACE_GETREGS, 0, &regs);
	errno=regs.rax;
	if(errno != 0) {
		perror("\tmprotect");
	}

	safe_ptrace(PTRACE_SETREGS, 0, &orig_regs);
	return 0;
}

const _DInst* T::getI(addr_t loc) const
{
	std::map<addr_t, _DInst*>::const_iterator it = rip_i_.find(loc);
	if(it == rip_i_.end()) 
		throw InvalidRIP();

	return it->second;
}
const _CodeInfo* T::getCi() const
{
	return &ci_;
}

void T::reset()
{
    fprintf(stderr, "Resetting Target\n");
	initial_state_->restore();
}

void T::runTo(addr_t rip)
{
    fprintf(stderr, "Run to %lx\n", rip);
	assert(inCode(rip));
	bp_->set(rip);
	safe_ptrace(PTRACE_CONT, 0, NULL);
	int status;
	waitpid(pid_, &status, 0);
	bp_->unset(rip);
    struct user_regs_struct regs;
    safe_ptrace(PTRACE_GETREGS, 0, &regs);
    fprintf(stderr, "ok (RIP: %lx)\n", regs.rip);
}
