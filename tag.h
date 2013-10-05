#ifndef TAG_H
#define TAG_H

#include "types.h"
#include "access.h"
#include "trace.h"

#include <vector>
#include <map>
#include <tr1/functional>

class Tag
{
	public:
		Tag(addr_t loc, int len);
		Tag(Access* a);
		~Tag();

        const Access* lastAccess() const;
        const std::map<addr_t, Access*>& rip_access() const
        {return rip_access_;}
        const Val* init_val() const {return init_val_;}
		addr_t loc() const;
		int len() const;

		// log access
		int log(addr_t rip, addr_t loc);

        int addTraceF(addr_t rip, Tag* t, TTYPE type);
        int addTraceB(addr_t rip, Tag* t, TTYPE type);

        const std::map<addr_t, Trace*>& tforw() const {
            return tforw_;
        }

	private:
		int addAccess(Access* a);
		std::vector<Access*> access_;
		std::map<addr_t, Access*> rip_access_;

		std::map<addr_t, Trace*> tforw_;
		std::map<addr_t, Trace*> tbackw_;

		addr_t loc_;
		int len_;
        Val* init_val_;
};

namespace std { namespace tr1 {
	template <> struct hash<Tag>
	{
		size_t operator()(const Tag& t) const
		{
			return std::tr1::hash<int>()(t.len()) ^
				std::tr1::hash<addr_t>()(t.loc()) << 4;
		}
	};
}}

#endif
