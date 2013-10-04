#ifndef _H_ELF
#define _H_ELF

#include "types.h"

#include <string>
#include <map>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <libelf.h>
#include <gelf.h>

#define ERR -1

struct classcomp {
	  bool operator() (const char* lhs, const char* rhs) const
	    {return strcmp(lhs,rhs)<0;}
};

struct classcompAdr {
	  bool operator() (addr_t lhs, addr_t rhs) const
	    {return lhs<rhs;}
};

class Elf {
	public:
		Elf(char* fname);
		~Elf();

		int init();

		addr_t get_func(char* name);
		char* get_func(addr_t ip);

		void print_func();
		
	private:
		std::map<char*, addr_t, classcomp> func_;

};
#endif
