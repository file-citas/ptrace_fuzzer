#include "vrange.h"
#include "target.h"
#include <assert.h>

VRange::VRange(Tag* tag, Val* start_val, Val* stop_val) :
	tag_(tag), start_(start_val), stop_(stop_val), current_(start_val)
{
    inc_ = new IncSkip(stop_->val(), stop_->len());
    //unsigned char step = 1;
    //inc_ = new IncNum(&step, 1);
}

VRange::~VRange()
{
	delete inc_;
}

bool VRange::next() const
{
    if(current_->cmp(*stop_)) return false;
    addr_t newLoc = inc_->inc(current_);
    if(newLoc != 0) {
	    // TODO update back references;
	    fprintf(stderr, "Variable moved to %lx\n", newLoc);
    }
    return true;
}
void VRange::set() const
{
    T::arget().write(tag_->loc(), (void*)current_->val(), tag_->len());
}

void VRange::setInc(Inc* inc)
{
	delete inc_;
	inc_ = inc;
}
