#include "tag.h"
#include "target.h"
#include <stdio.h>

Tag::Tag(addr_t loc, int len) :
	loc_(loc), len_(len)
{
    init_val_ = new Val(loc_, len_);
	fprintf(stderr, "Created Tag at %lx (+%d)\n", loc_, len_);
}

Tag::Tag(Access* a)
{
	addAccess(a);
	loc_ = a->loc();
	len_ = a->len();
    init_val_ = new Val(loc_, len_);
	fprintf(stderr, "Created Tag at %lx (+%d)\n", loc_, len_);
}

Tag::~Tag()
{
    delete init_val_;
	std::vector<Access*>::iterator it = access_.begin();
	for( ; it!=access_.end(); ++it)
		delete *it;

	std::map<addr_t, Trace*>::iterator jt = tforw_.begin();
	for( ; jt!=tforw_.end(); ++jt)
		delete jt->second;
	jt = tbackw_.begin();
	for( ; jt!=tbackw_.end(); ++jt)
		delete jt->second;
	// DO NOT double delete the access ptrs
	// DO NOT double delete the access ptrs
}

const Access* Tag::lastAccess() const
{
	if(access_.empty())
		return NULL;
	return access_.back();
}
addr_t Tag::loc() const
{
	return loc_;
}
int Tag::len() const
{
	return len_;
}

int Tag::log(addr_t rip, addr_t loc)
{
	Access* a = new Access(rip, loc);
	addAccess(a);
	return 1;
}

int Tag::addAccess(Access* a)
{
	access_.push_back(a);
	rip_access_.insert(std::pair<addr_t, Access*>(
				a->rip(), a));
	return 0;

}

int Tag::addTraceF(addr_t rip, Tag* t, TTYPE type)
{
	Trace* tr = new Trace(t, type);
	std::pair<std::map<addr_t, Trace*>::iterator, bool> ret =
		tforw_.insert(std::pair<addr_t, Trace*>(
				rip, tr));
	if(!ret.second) {
		delete tr;
		return 0;
	}
	return 1;
}
int Tag::addTraceB(addr_t rip, Tag* t, TTYPE type)
{
	Trace* tr = new Trace(t, type);
	std::pair<std::map<addr_t, Trace*>::iterator, bool> ret =
		tbackw_.insert(std::pair<addr_t, Trace*>(
				rip, tr));
	if(!ret.second) {
		delete tr;
		return 0;
	}
	return 1;
}
