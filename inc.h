#ifndef INCRE_H
#define INCRE_H

#include "val.h"


// TODO: init with start value?

class Inc
{
	public:
		Inc(unsigned char* incStep, int len);
		virtual ~Inc();
		virtual addr_t inc(Val* v, unsigned char maxByte = 0) = 0;
	protected:
		unsigned char* incStep_;
		int len_;
		unsigned char maxChar_;

};

class IncNum : public Inc
{
	public:
		IncNum(unsigned char* incStep, int len);
		virtual ~IncNum();
		virtual addr_t inc(Val* val, unsigned char maxByte = 0);

};

class IncStr : public Inc
{
	public:
		IncStr(unsigned char* incStep, int len);
		virtual ~IncStr();
		virtual addr_t inc(Val* val, unsigned char maxByte = 0);
	private:
		// TODO
		int currentByte_;
		int incByte_;
		unsigned char* reset_;

};


class IncStrLen : public Inc
{
	public:
		IncStrLen(unsigned char* incStep, int len);
		virtual ~IncStrLen();
		virtual addr_t inc(Val* val, unsigned char maxByte = 0);
	private:
		// TODO

};

#endif
