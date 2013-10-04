#ifndef TARGET_H
#define TARGET_H

#include "myelf.h"
#include "break.h"
#include "distorm.h"
#include <map>

typedef addr_t _OffsetType;

class InvalidRIP : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return "Instruction Address not decoded";
		}
};

class T
{
	public:
		// get singelton instance
		static T& arget()
		{
			static T inst;
			return inst;
		}
		~T();

		// initialize target
		int init(char** argv);

		// getter
		pid_t pid() const { return pid_;}
		const Elf* elf() const { return elf_;}
		Break* bp() { return bp_;}
		const _DInst* getI(addr_t rip) const;
		const _CodeInfo* getCi() const;
        addr_t cstop() const {return code_stop_;}

		// helper
		bool inCode(addr_t loc) const;
		bool inStack(addr_t loc) const;
		// strlen of str supposed to be at loc
		size_t getStrLen(addr_t loc);

		// ptrace wrapper
		addr_t safe_ptrace(int request, addr_t addr, void* data) const;
		// step over 1 instruction
		int singlestep();
		// set protection flags for stack segment
		int protect_stack(int prot);
		// read from target process
		int read(addr_t loc, void* val, size_t len) const;

	private:
		T() : init_(false), pid_(0) {};
		T(T const&);
		void operator=(T const&);

		// get stack segment from /proc/*/maps
		int getStackSegment(addr_t& sstart, addr_t& sstop);
		// write to target process
		int write(addr_t loc, void* val, size_t len) const;
		// protect arbitrary memory reagion
		int protect(addr_t from, addr_t to, int prot);

		// flag to check if target has been successfully initialized
		bool init_;
		// pid of the target process
		int pid_;

		Elf* elf_;
		Break* bp_;

		addr_t code_start_; 	// start of target code segment
		addr_t code_stop_;  	// end of target code segment
		addr_t stack_start_; 	// start of target stack segment
		addr_t stack_stop_;  	// end of target stack segment

		// remember current stack protection flags to avoid redundant calls
		int last_stack_prot_;
		addr_t syscall_addr_;

		_CodeInfo ci_;
		std::map<addr_t, _DInst*> rip_i_;
};

#endif
