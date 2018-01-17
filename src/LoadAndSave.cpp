#include "File.h"
#include "fastIO.h"
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define BUFFER_SIZE 256
#else
#define BUFFER_SIZE 1048576
#endif

// #pragma GCC push_options
// #pragma GCC optimize ("O0")

static FAST_IN<256> in;
static FAST_OUT<256> out;
INIFile* iniLoadFile (const char *name) {
	char line[1024];
	INIFile *f;
	INISection *s = nullptr;
	if(!in.open(name, "r")) {
		setError(INI_ERR_FILE_OPEN_FAILED);
		return nullptr;
	}
	f = new INIFile();
	while(in.get_line(line) > 0) {
		char *st, *pt;
		pt = strchr(line, ':'); if(pt) *pt = '\0';
		pt = strchr(line, '#'); if(pt) *pt = '\0';	// Cut all the comments

		for(st = line; isspace(*st); ++st); //ltrim

		if(!*st) continue;					// Ignore null lines

		if(*st == '[') {
			pt = strchr(st, ']');
			if(!pt) {
				delete f;
				if(s != nullptr) delete s;
				setError(INI_ERR_FILE_PARSE_FAILED);
				in.close();
				return nullptr;
			}
			*pt = '\0';
			if(s != nullptr) f->insert(s);
			s = new INISection(st + 1);
		} else {
			if(s == nullptr) {	// No Section, set empty name
				s = new INISection("");
			}
			pt = strchr(st, '=');
			if(!pt) {
				if(s != nullptr) delete s;
				delete f;
				setError(INI_ERR_FILE_PARSE_FAILED);
				in.close();
				return nullptr;
			}
			if(pt == st) {		// Name is empty
				delete s;
				delete f;
				setError(INI_ERR_FILE_PARSE_FAILED);
				in.close();
				return nullptr;
			}
			*pt = '\0';
			s->insert(new INIKey(st, pt + 1));
		}
	}
	if(s != nullptr) f->insert(s);
	in.close();
	return f;
}
int iniSaveFile (INIFile *f, const char *name) {
	if(!out.open(name, "w")) {
		setError(INI_ERR_FILE_OPEN_FAILED);
		return INI_FAILED;
	}
	for(INISection* s = iniGetFirstSection(f); s; s = iniGetNextSection(s)) {
		out <<'[' <<iniGetSectionName(s) <<']' <<'\n';
		for(INIKey* k = iniGetFirstKey(s); k; k = iniGetNextKey(k))
			out <<iniGetKeyName(k) <<'=' <<iniGetKeyValue(k) <<'\n';
	}
	out.close();
	return INI_SUCCEEDED;
}

// #pragma GCC pop_options