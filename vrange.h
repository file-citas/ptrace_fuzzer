#ifndef VRAGNE_H
#define VRAGNE_H

#include "tag.h"
#include "val.h"

class VRange
{
	public:
		VRange(const Tag* t, const Val& start_val, const Val& stop_val);
		~VRange();

        const Val& next() const;
        void setNext() const;

	private:
		const Tag* tag_;
		const Val start_;
		const Val stop_;
        mutable Val current_;
};

#endif
