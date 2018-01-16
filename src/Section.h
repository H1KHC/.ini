#include "Key.h"

struct INIFile;
struct INISection {
	INIFile* owner;
	std::set<INISection*, INISort<INISection> >::iterator thisIterator;
	std::set<INIKey*, INISort<INIKey> > set;
	std::string name;

	bool operator < (const INISection &s) const {
		return name < s.name;
	}

	void insert(INIKey*);
	void erase(INIKey*);

	INISection(std::string&& _name) : owner(nullptr), name(trim(_name)) {};
};