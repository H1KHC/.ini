#include "Section.h"

struct INIFile {
	typedef std::set<INISection*, INISort<INISection> > container;
	typedef container::iterator iterator;
	container set;
	
	void insert(INISection*);
	void erase(INISection*);
};