#ifndef TRACER_H
#define TRACER_H

#include "types.h"
#include "tag.h"
#include "branch.h"

#include <set>
#include <vector>
#include <map>

typedef std::multimap<addr_t, Tag*> tagmap;
typedef std::multimap<addr_t, Branch*> branchmap;

class Tracer
{
	public:
		Tracer(int argc, char** argv);
		~Tracer();

        std::set<Tag*> tags() const {return tags_;}
		const std::vector<Branch*> branchs() const;
		
		// run target until event occurs
		int trace();
	
	private:
		int handle_cjmp();
		int handle_segv(addr_t loc, addr_t rip);
		int addTag(Tag* t);
		int addBranch(Branch* t);

		std::set<Tag*> tags_; // list of all tags
		std::vector<Tag*> last_tag_;
		tagmap mem_tags_; // access to tags via memory address
		std::vector<Branch*> branchs_;
		std::map<addr_t, Branch*> rip_branch_;
};

#endif
