#include "myelf.h"


Elf::Elf(char* fname)
{
	Elf         *elf;
	Elf_Scn     *scn = NULL;
	GElf_Shdr   shdr;
	Elf_Data    *data;
	int         fd, ii, count;

	elf_version(EV_CURRENT);

	fd = open(fname, O_RDONLY);
	if(fd<0) perror("fopen");

	elf = elf_begin(fd, ELF_C_READ, NULL);

	while ((scn = elf_nextscn(elf, scn)) != NULL) {
		gelf_getshdr(scn, &shdr);
		if (shdr.sh_type == SHT_SYMTAB) break;
	}

	data = elf_getdata(scn, NULL);
	count = shdr.sh_size / shdr.sh_entsize;

	std::pair<std::map<char*,addr_t>::iterator,bool> ret;
	for (ii = 0; ii < count; ++ii) {
		GElf_Sym sym;
		gelf_getsym(data, ii, &sym);
		char* symname = elf_strptr(elf, shdr.sh_link, sym.st_name);
		int n = strlen(symname)+1;
		char* tmp = new char[n];
		strncpy(tmp, symname, n);
		ret = func_.insert(std::pair<char*, addr_t>(
			tmp,
			sym.st_value));
		if(!ret.second)
			delete [] tmp;
	}

	elf_end(elf);
	if(close(fd)!=0) perror("close");
}


Elf::~Elf()
{
	std::map<char*, addr_t>::iterator it = func_.begin();
	for( ; it!=func_.end(); ++it)
		delete [] it->first;
	func_.clear();
}

addr_t Elf::get_func(char* name) {
	std::map<char*, addr_t>::iterator i = func_.find(name);
	if(i==func_.end()) {
		fprintf(stderr, "ERROR: can not find %s\n", name);
		exit(1);
	}
	return i->second;
}
