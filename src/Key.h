#include <string>
#include <set>
#include "Error.h"

template<class CONTAINER>
struct INISort {
	bool operator () (const CONTAINER*a, const CONTAINER*b) const {
		return a->name < b->name;
	}
};

inline std::string& trim(std::string& s) {
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

struct INISection;
struct INIKey {
	INISection* owner;
	std::set<INIKey*, INISort<INIKey> >::iterator thisIterator;
	std::string name, value;
	bool operator < (const INIKey &k) const {
		return name < k.name;
	}
	INIKey(std::string &&_name, std::string &&_value = "") :
		owner(nullptr), name(trim(_name)), value(trim(_value)) {}
};