#ifndef VRAGNE_H
#define VRAGNE_H

#include "tag.h"
#include "val.h"
#include "inc.h"

class VRange
{
	public:
        VRange(Tag* t, Val *start_val, Val *stop_val);
		~VRange();

        bool next() const;
        void set() const;
		void setInc(Inc* inc);
        const Inc* inc() {return inc_;}
		const char* str() const {
            return current_->str();
		}

	private:
        Tag* tag_;
        Val* start_;
        Val* stop_;
		Inc* inc_;
        mutable Val* current_;
};

#endif
