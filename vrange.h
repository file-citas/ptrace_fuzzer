#ifndef VRAGNE_H
#define VRANGE_H

#include "tag.h"
#include "val.h"

class VRange
{
	public:
		VRange(const Tag* t, const Val& start_val, const Val& stop_val);
		~VRange();

		const Val& next();
		void setNext();

	private:
		const Tag* tag_;
		const Val start_;
		const Val stop_;
		Val current_;
};

#endif
