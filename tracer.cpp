#include "tracer.h"
#include "target.h"
#include "distorm.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <math.h>
#include <limits>

Tracer::Tracer(int argc, char** argv) :
    heap_min_(std::numeric_limits<addr_t>::max()),
    heap_max_(0),
    stack_min_(std::numeric_limits<addr_t>::max()),
    stack_max_(0)
{
	// prep target
	T::arget().init(argv);
	// find argv
	struct user_regs_struct regs;
	T::arget().safe_ptrace(PTRACE_GETREGS, 0, &regs);
	for(int i=0; i<argc; ++i) {
		addr_t a = regs.rsi + i*sizeof(addr_t);
		addr_t aDst = 0;
        int len = strlen(argv[i])+1;
		Tag* src = new Tag(a, sizeof(addr_t));
		T::arget().read(a, (void*)&aDst, sizeof(addr_t));
        Tag* dst = new Tag(aDst, len);
        char tmp[16];
        T::arget().read(aDst, (void*)&tmp, sizeof(addr_t));
        fprintf(stderr, "%c\n", tmp[0]);

		src->addTraceF(0, dst, PTR);
		dst->addTraceB(0, src, PTR);

		addTag(src);
		addTag(dst);
	}
	// protect stack so we get a segfault on access
	T::arget().protect_stack(PROT_NONE);
}
Tracer::~Tracer()
{
	std::set<Tag*>::iterator it =tags_.begin();
	for( ; it!=tags_.end(); ++it)
		delete *it;
	// dont double delete tags ...
	std::vector<Branch*>::iterator jt = branchs_.begin();
	for( ; jt!=branchs_.end(); ++jt)
		delete *jt;
}

int Tracer::trace()
{
	// continue target
	T::arget().safe_ptrace(PTRACE_CONT, 0, NULL);
	int status;
	waitpid(T::arget().pid(), &status, 0);
	if(WIFEXITED(status))
	{
		fprintf(stderr, "child exit status %d\n", 
				WEXITSTATUS(status));
		return 0;
	}

	// assume WIFSTOPPED
	struct user_regs_struct regs;
	T::arget().safe_ptrace(PTRACE_GETREGS, 0, &regs);
	siginfo_t si;
	T::arget().safe_ptrace(PTRACE_GETSIGINFO, 0 , &si); 

	//printf("%lx\n", regs.rip);
	if(regs.rip==0 ){//|| regs.rip==T::arget().cstop()) {
		T::arget().protect_stack(PROT_READ|PROT_WRITE);
		T::arget().safe_ptrace(PTRACE_CONT, 0, NULL);
		return 0; // we are done
	}

	// un protect stack
	T::arget().protect_stack(PROT_READ|PROT_WRITE);

	// if the next instruction is not in our area we break on return
    if(!T::arget().inCode(regs.rip)) {
		fprintf(stderr, "%lx not in code\n", regs.rip);
		addr_t returnadr = T::arget().safe_ptrace(PTRACE_PEEKTEXT, 
				regs.rsp, NULL);
		if(returnadr==0) return 0;
		T::arget().bp()->set(returnadr);
		T::arget().safe_ptrace(PTRACE_CONT, 0, NULL);

		int status;
		waitpid(T::arget().pid(), &status, 0);
		T::arget().bp()->unset(returnadr);

		T::arget().protect_stack(PROT_NONE);
		return 1;
	}

	// handle signal
	switch(si.si_signo) {
		case SIGTRAP:
			handle_cjmp();
			break;
		case SIGSEGV:
			handle_segv((addr_t)si.si_addr, regs.rip);
			break;
		default:
			fprintf(stderr, "Unknown Signal %d\n", si.si_signo);
			return 0; // done
	}

	// reexecute instruction and reprotect stack
	T::arget().singlestep();
	T::arget().protect_stack(PROT_NONE);
	return 1;
}

int Tracer::handle_cjmp()
{
	// unset breakpoint
	struct user_regs_struct regs;
	T::arget().safe_ptrace(PTRACE_GETREGS, 0, &regs);
	addr_t rip = regs.rip-0x1; // -0x1 because 'int $0x3' has already been executed
	T::arget().bp()->unset(rip);

	fprintf(stderr, "cjmp at %lx\n", regs.rip);

	// create branch instance
	const _DInst* ijmp = T::arget().getI(rip);
	addr_t to = 0;
	if (ijmp->flags & FLAG_RIP_RELATIVE)
		to = INSTRUCTION_GET_RIP_TARGET(ijmp);
	else
		to = INSTRUCTION_GET_TARGET(ijmp);
	Branch* b = new Branch(rip, to);
	if(!addBranch(b))
		return 0;
	
	// iterate over last accessed tags
	std::vector<Tag*>::reverse_iterator tag = last_tag_.rbegin();
	for(; tag!=last_tag_.rend(); ++tag) {
		const Access* ta = (*tag)->lastAccess();
		if(ta == NULL) continue;
		_OffsetType lastRip = ta->rip();
		for(_OffsetType off = 0; off+lastRip < rip; ) {
			// decode instruction
			const _DInst* inst = T::arget().getI(lastRip+off);
			// compared accessed flags
			if(inst->modifiedFlagsMask & ijmp->testedFlagsMask) {
				fprintf(stderr, "Set in %lx\n", lastRip+off);
				if(inst->ops[0].type == O_IMM || inst->ops[1].type == O_IMM) {
					fprintf(stderr, "value %lx\n", inst->imm.sqword);
					regs.rip = rip;
					T::arget().safe_ptrace(PTRACE_SETREGS, 0, &regs);
					return 1;
				}
				else 
					fprintf(stderr, "No immediate operands\n");
				size_t len = fmin((*tag)->lastAccess()->len(),inst->ops[0].size);
				Val* val = new Val((unsigned char*)&inst->imm.sqword, len);
				Cond* c = new Cond((*tag), val);
				// until ??
				// add condition to branch
				b->addCond(c);
			}
			off += inst->size;
		}
	}
	getchar();
	return 0;
}

int Tracer::handle_segv(addr_t loc, addr_t rip)
{
	std::multimap<addr_t, Tag*>::iterator tag = mem_tags_.find(loc);
	// handle tagged memory
	if(tag!=mem_tags_.end()) {
		tag->second->log(rip, loc);
		last_tag_.push_back(tag->second);
	} else { // handle untagged memory
		try {
			Access* a = new Access(rip, loc);
			Tag* t = new Tag(a);
			std::vector<Tag*>::reverse_iterator ltag;
			switch(a->atype())
			{
				case READ:
					addTag(t);
					break;
				case WRITE:
					if(addTag(t)) {
						for(ltag = last_tag_.rbegin(); ltag!=last_tag_.rend(); ++ltag) {
							if(t==*ltag) continue;
							const Access* ta = (*ltag)->lastAccess();
							if(ta == NULL) continue;
							if(ta->atype()==READ && // read<->write
									a->reg() == ta->reg()) {
								t->addTraceB(rip, *ltag, CPY);
                                (*ltag)->addTraceF(rip, t, CPY);
							}
						}
					}
					break;
				default:
					delete t;
					fprintf(stderr, "NO TYPE\n");
			}
		} catch (...) {
			return 0;
		}
	}
	// add new tag
	return 0;
}

int Tracer::addTag(Tag* t)
{
	std::pair<std::set<Tag*>::iterator, bool> ret = tags_.insert(t);
	if(!ret.second) { // tag is already present
		delete t;
		return 0;
	}
	stack_min_ = fmin(stack_min_, t->loc());
	stack_max_ = fmax(stack_max_, t->loc() + t->len());
	for(int i=0; i<t->len(); ++i)
		mem_tags_.insert(std::pair<addr_t, Tag*>(t->loc()+i, t));
	if(t->lastAccess()!=NULL)
		last_tag_.push_back(t);
	return 1;
}

int Tracer::addBranch(Branch* b)
{
	std::pair<std::map<addr_t, Branch*>::iterator, bool> ret = 
		rip_branch_.insert(std::pair<addr_t, Branch*>(b->from(),b));
	if(!ret.second) { // tag is already present
		delete b;
		return 0;
	}
	branchs_.push_back(b);
	return 1;
}
