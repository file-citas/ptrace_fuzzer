#include "vrange.h"
#include "target.h"
#include <assert.h>

VRange::VRange(Tag* tag, Val* start_val, Val* stop_val) :
	tag_(tag), start_(start_val), stop_(stop_val), current_(start_val)
{
    unsigned char incStep = 1;
    inc_ = new IncNum(&incStep, 1);
}

VRange::~VRange()
{
	delete inc_;
}

bool VRange::setNext() const
{
    current_->mkStr();
    T::arget().write(tag_->loc(), (void*)current_->val(), tag_->len());
    addr_t newLoc = inc_->inc(current_);
    if(newLoc != 0) {
	    // TODO update back references;
	    fprintf(stderr, "Variable moved to %lx\n", newLoc);
    }
    if(current_->cmp(*stop_)) return false;
    return true;
}

void VRange::setInc(Inc* inc)
{
	delete inc_;
	inc_ = inc;
}
