#include "vrange.h"
#include "target.h"
#include <assert.h>

VRange::VRange(const Tag* tag, const Val& start_val, const Val& stop_val) :
	tag_(tag), start_(start_val), stop_(stop_val), current_(start_val)
{
	assert(start_val.len() == stop_val.len());
}

VRange::~VRange()
{
}

const Val& VRange::next() const
{
	// TODO
	return ++current_;
}

void VRange::setNext() const
{
	// TODO
	T::arget().write(tag_->loc(), (void*)(++current_).val(), tag_->len());
}
