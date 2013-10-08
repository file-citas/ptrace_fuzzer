#include "fuzzer.h"
#include "state.h"
#include "target.h"

#include <algorithm>    // std::swap

Fuzzer::Fuzzer(int argc, char** argv)
{
	tracer_ = new Tracer(argc, argv);
	// initial run
	while(tracer_->trace());
}

Fuzzer::~Fuzzer()
{
	delete tracer_;
}

void Fuzzer::fuzz(addr_t from, addr_t to, const std::vector<const VRange*>& vrange)
{
    if(from>to) std::swap(from, to);
	// reset target
	T::arget().reset();
	// run to start event
	T::arget().runTo(from);
	// save state
	struct user_regs_struct regs;
	T::arget().safe_ptrace(PTRACE_GETREGS, 0, &regs);
	State state(regs, Memstate(tracer_->heap_min(), tracer_->heap_max()), 
			Memstate(tracer_->stack_min(), tracer_->stack_max()));


	fprintf(stderr, "Starting Fuzz from %lx to %lx\n", from, to);

    // iterate over all values for all tags
    state.restore();
    for( auto value : vrange ) {
        do {
            value->set();
			fprintf(stderr, "Starting target with %s... \n", value->str());
			T::arget().runTo(to);
            getchar();
            state.restore();
        }while (value->next());
	}
}
