#include "../ini.h"
#include "Section.h"
#include <cstring>

INIKey* iniGetNextKey (INIKey*k) {
	auto it = k->thisIterator;
	++it;
	if(it == k->owner->set.end()) return nullptr;
	return const_cast<INIKey*>(*it);
}
INIKey* iniGetLastKey (INIKey*k) {
	auto it = k->thisIterator;
	if(it == k->owner->set.begin()) return nullptr;
	--it;
	return const_cast<INIKey*>(*it);
}

const char * iniGetKeyName (INIKey*k) {
	return k->name.c_str();
}
const char * iniGetKeyValue (INIKey*k) {
	return k->value.c_str();
}

int iniChangeKeyName (INIKey*k, const char *name) {
	if(strchr(name, ';') || strchr(name, '#')) {
		setError(INI_ERR_ILLEGAL_VALUE);
		return INI_FAILED;
	}
	if(iniFindKey(k->owner, name)) {
		setError(INI_ERR_NAME_CONFLICTED);
		return INI_FAILED;
	}
	k->owner->erase(k);
	k->name = name;
	k->owner->insert(k);
	return INI_SUCCEEDED;
}

int iniChangeKeyValue (INIKey *k, const char *value) {
	if(strchr(value, ';') || strchr(value, '#')) {
		setError(INI_ERR_ILLEGAL_VALUE);
		return INI_FAILED;
	}
	k->value = value;
	return INI_SUCCEEDED;
}

void iniDeleteKey (INIKey *k) {
	k->owner->erase(k);
	delete k;
}