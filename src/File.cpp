#include "../ini.h"
#include "File.h"

void INIFile::insert (INISection *s) {
	auto r = set.insert(s);
	s->owner = this;
	s->thisIterator = r.first;
}

void INIFile::erase (INISection *s) {
	set.erase(s->thisIterator);
}

INIFile* iniNewFile () {
	return new INIFile();
}

size_t iniGetFileSize (INIFile *f) {
	return f->set.size();
}

void iniCleanFile (INIFile *f) {
	for(auto it = f->set.begin(), _end = f->set.end(); it != _end; it = f->set.erase(it))
		delete *it;
}

void iniCloseFile (INIFile *f) {
	iniCleanFile(f);
	delete f;
}

INISection* iniNewSection (INIFile *f, const char *name) {
	INISection *s = new INISection(name);
	f->insert(s);
	return s;
}
INISection* iniFindSection (INIFile *f, const char *name) {
	INISection *s = new INISection(name);
	auto it = f->set.find(s);
	delete s;
	return const_cast<INISection*>(it == f->set.end() ? nullptr : *it);
}

INISection* iniGetFirstSection (INIFile *f) {
	return const_cast<INISection*>(f->set.empty() ? nullptr : *f->set.begin());
}
