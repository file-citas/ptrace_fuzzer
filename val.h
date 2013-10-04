#ifndef VAL_H
#define VAL_H

#include "types.h"

class Val
{
	public:
		Val(addr_t loc, int len);
		Val(unsigned char* val, int len);
		Val(const Val& v);
		~Val();

		int len() const {return len_;}
		const unsigned char* val() const {return val_;}

		bool cmp(Val& val);
		const char* str() const {return str_;}
	
	private:
		int mkStr();
		unsigned char* val_;
		int len_;
		char* str_;
};


#endif
