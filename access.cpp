#include "access.h"
#include "target.h"
#include "distorm.h"

Access::Access(addr_t rip, addr_t loc) :
	rip_(rip), loc_(loc)
{
	if(!T::arget().inStack(loc_))
		throw InvalidMem();
	reg_ = -1;
	len_ = -1;
	val_ = NULL;
	const _DInst* inst = T::arget().getI(rip_);
	if(META_GET_FC(inst->meta) == FC_CALL) {
		//fprintf(stderr, "Unknown instruction at %lx\n", rip);
		_DecodedInst di;
		const _CodeInfo* ci = T::arget().getCi();
		distorm_format(ci, inst, &di);
		//fprintf(stderr, "%s %s\n", di.mnemonic.p, di.operands.p);
		atype_ = IDK;
	}else {
		// check if read or write via the operand types
		// alternative would be to modify the protection and reexecute the inst->uction
		if(inst->ops[0].type == O_SMEM) { // probl. write
			atype_ = WRITE;
			len_ = inst->ops[0].size/8; // TODO
			if(inst->ops[1].type == O_REG)
				reg_ = inst->ops[1].index;
		} else if(inst->ops[1].type == O_SMEM) { // probl. read
			atype_ = READ;
			len_ = inst->ops[1].size/8; // TODO
			if(inst->ops[0].type == O_REG)
				reg_ = inst->ops[0].index;
		}
	}

	if(atype_==IDK || len_<1)
		throw InvalidMem();
	val_ = new Val(loc_, len_);
	fprintf(stderr, "%d access on %lx at %lx (reg %d) : %s\n", 
			atype_, loc_, rip_, reg_, val_->str());
}

Access::~Access()
{
	if(val_)
		delete val_;
}

ACCESSTYPE Access::atype() const
{
	return atype_;
}