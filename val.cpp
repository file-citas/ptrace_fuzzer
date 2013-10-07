#include "val.h"
#include "target.h"
#include <assert.h>

Val::Val(addr_t loc, int len) :
	len_(len)
{
	val_ = new unsigned char[len_];
	T::arget().read(loc, val_, len_);
	mkStr();
}
Val::Val(unsigned char* val, int len) :
	len_(len)
{
	val_ = new unsigned char[len_];
	for(int i=0; i<len_; ++i)
		val_[i] = val[i];
	mkStr();
}
Val::Val(const Val& val) :
	len_(val.len_)
{
	val_ = new unsigned char[len_];
    for(int i=0; i<len_; ++i)
		val_[i] = val.val_[i];
	mkStr();
}
Val::~Val()
{
	delete[] val_;
	delete[] str_;
}

int Val::mkStr()
{
	str_ = new char[len_*4+1];
	int i;
	for(i=0; i<=len_*3; i+=3) {
		snprintf(&str_[i], 4, "%2X ", 0xff&val_[i]);
	}
	str_[i] = 0;
	return i;
}

Val& Val::operator++()
{
	long long* x = (long long*) val_;
	*x+=0x1;
	return *this;
}

bool Val::cmp(const Val &val) const
{
    for(int i=0; i<len_ && i<val.len(); ++i)
        if(val_[i]!=val.val()[i])
            return false;
    return true;
}

void Val::val(unsigned char* newVal, int newLen)
{
    assert(newLen<=len_);
    for(int i=0; i<newLen; ++i) {
        //fprintf(stderr, "%2x->%2x\n", val_[i], newVal[i]);
        val_[i] = newVal[i];
    }
    //fprintf(stderr, "New Value set (%2x ...)\n", val_[0]);
}
