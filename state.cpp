#include "state.h"
#include "target.h"
#include <stdlib.h>
#include <assert.h>

Memstate::Memstate(addr_t from, addr_t to) :
    from_(from), to_(to), data_(NULL)
{
    if(from_<to_) {
        fprintf(stderr, "Saving State from %lx to %lx\n", from_, to_);
        data_ = new unsigned char[to_-from_];
        T::arget().read(from_, (void*)data_, to_-from_);
    }
}
Memstate::Memstate(const Memstate& memstate) :
    from_(memstate.from_), to_(memstate.to_), data_(NULL)
{
    if(from_<to_) {
        fprintf(stderr, "Saving State from %lx to %lx\n", from_, to_);
        data_ = new unsigned char[to_-from_];
        memcpy(data_, memstate.data_, to_-from_);
    }
}
Memstate::~Memstate()
{
    if(data_)
        delete data_;
}

void Memstate::restore() const
{
    if(data_) {
        fprintf(stderr, "Restoring State from %lx to %lx\n", from_, to_);
        T::arget().write(from_, (void*)&data_, to_-from_);
    }
}

State::State(const struct user_regs_struct& regs, const Memstate& heapstate,
             const Memstate& stackstate) :
    regs_(regs), heapstate_(heapstate), stackstate_(stackstate)
{
}
State::~State()
{
}
void State::restore() const
{
    struct user_regs_struct r = regs_;
    T::arget().safe_ptrace(PTRACE_SETREGS, 0, &r);
    heapstate_.restore();
    stackstate_.restore();
}
