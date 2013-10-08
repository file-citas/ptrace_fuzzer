#include "inc.h"
#include <stdio.h>
#include <string.h>
#include <limits>
#include <assert.h>
#include <math.h>

Inc::Inc(unsigned char* incStep, int len) : len_(len)
{
	incStep_ = new unsigned char[len_];
	for(int i=0; i<len_ ; ++i)
		incStep_[i] = incStep[i];
	maxChar_ = std::numeric_limits<unsigned char>::max();
}
Inc::~Inc()
{
	delete[] incStep_;
}

IncNum::IncNum(unsigned char* incStep, int len) : Inc(incStep, len)
{
}
IncNum::~IncNum()
{
}
addr_t IncNum::inc(Val* val, unsigned char maxByte)
{
	int i;
	addr_t newLoc = 0;
	int vLen = val->len();
	if(len_>=vLen) {
		val->len(len_+1);
		vLen = len_ + 1;
	}
	unsigned char overflow = 0;
    for(i=0; i<len_ ; ++i) {
        val->val()[i] += incStep_[i] + overflow;
        overflow = fmax(0,val->val()[i] + incStep_[i] - maxChar_);
	}
    val->val()[i] += overflow;
	val->mkStr();
	return newLoc;
}

IncStr::IncStr(unsigned char* incStep, int len) : Inc(incStep, len)
{
	assert(len_==1);
	currentByte_ = 0;
	incByte_ = 0;
	reset_ = NULL;
}
IncStr::~IncStr()
{
	delete[] reset_;
}
addr_t IncStr::inc(Val* val, unsigned char maxByte)
{
	return 0;
}
IncStrLen::IncStrLen(unsigned char* incStep, int len) : Inc(incStep, len)
{
	assert(len==1);
}
IncStrLen::~IncStrLen()
{
}
addr_t IncStrLen::inc(Val* val, unsigned char maxByte)
{
	addr_t newLoc = 0;
	int vLen = val->len();
	if(vLen < maxByte)
	{
		val->len(maxByte);
		vLen = maxByte;
	}
	int currentByte = strlen((char*)val->val())+1;
	assert(currentByte < vLen);
	val->val()[currentByte] = 'a';
	val->val()[currentByte+1] = 0;
	return newLoc;
}
