#include "../ini.h"
#include "File.h"
#include <cstring>

void INISection::insert (INIKey *k) {
	auto r = set.insert(k);
//	if(!r->second) throw ...;
	k->owner = this;
	k->thisIterator = r.first;
}
void INISection::erase (INIKey *k) {
	set.erase(k->thisIterator);
}

INIKey* iniNewKey (INISection *s, const char *name, const char *value) {
	if(strchr(name, ';') || strchr(name, '#') ||
	   strchr(value, ';') || strchr(value, '#')) {
		setError(INI_ERR_ILLEGAL_VALUE);
		return nullptr;
	}
	if(iniFindKey(s, name)) {
		setError(INI_ERR_NAME_CONFLICTED);
		return nullptr;
	}
	INIKey* k = new INIKey(name, value);
	s->insert(k);
	return k;
}

INIKey* iniFindKey (INISection *s, const char *name) {
	INIKey *k = new INIKey(name);
	auto it = s->set.find(k);
	delete k;
	return const_cast<INIKey*>(it == s->set.end() ? nullptr : *it);
}

INIKey* iniGetFirstKey (INISection *s) {
	return const_cast<INIKey*>(s->set.size() ? *s->set.begin() : nullptr);
}

INISection* iniGetNextSection (INISection *s) {
	auto it = s->thisIterator;
	++it;
	if(it == s->owner->set.end()) return nullptr;
	return const_cast<INISection*>(*it);
}
INISection* iniGetLastSection (INISection *s) {
	auto it = s->thisIterator;
	if(it == s->owner->set.begin()) return nullptr;
	--it;
	return const_cast<INISection*>(*it);
}

const char* iniGetSectionName (INISection *s) {
	return s->name.c_str();
}
size_t iniGetSectionSize (INISection *s) {
	return s->set.size();
}

int iniChangeSectionName (INISection *s, const char *name) {
	if(strchr(name, ']') || strchr(name, ';') || strchr(name, '#')) {
		setError(INI_ERR_ILLEGAL_VALUE);
		return INI_FAILED;
	}
	if(iniFindSection(s->owner, name)) {
		setError(INI_ERR_NAME_CONFLICTED);
		return INI_FAILED;
	}
	s->owner->erase(s);
	s->name = name;
	s->owner->insert(s);
	return INI_SUCCEEDED;
}
 
void iniCleanSection (INISection *s) {
	for(auto it = s->set.begin(), _end = s->set.end(); it != _end; it = s->set.erase(it))
		delete *it;
}

void iniDeleteSection (INISection *s) {
	s->owner->erase(s);
	iniCleanSection(s);
	delete s;
}
