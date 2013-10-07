#include "vrange.h"
#include "target.h"
#include <assert.h>

VRange::VRange(const Tag* tag, const Val& start_val, const Val& stop_val) :
	tag_(tag), start_(start_val), stop_(stop_val), current_(start_val)
{
}

VRange::~VRange()
{
}

/*const Val& VRange::next() const
{
    // TODO
	return ++current_;
}*/

bool VRange::setNext() const
{
	// TODO
    //if(current_.cmp(stop_)) return false;
    T::arget().write(tag_->loc(), (void*)(current_).val(), tag_->len());
    //current_ = inc_->next(current_);
    //++current_;
    return true;
}
