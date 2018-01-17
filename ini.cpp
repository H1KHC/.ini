#include "ini.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

INIFile *f;
INISection *s;
INIKey *k;
string filename;
bool needSave = false;
vector<string> cmds;

void help(map<string, void(*)()>& commandMap) {
	cout <<"Available commands:\n";
	for(auto p : commandMap) {
		cout <<"  "<<p.first<<'\n';
	}
	cout <<endl;
}

void quit() {
	exit(0);
}

void test() {
	int size = cmds.size();
	cout <<"Size:"<<size<<'\n';
	for(int i = 0; i < size; ++i)
		cout <<'#'<<i<<':'<<cmds[i]<<'\n';
	cout <<endl;
}

void open() {
	if(cmds.size() != 2) {
		cout <<"usage: open name"<<endl;
	}
	else if(f) {
		if(!s) {
			INISection *ns = iniFindSection(f, cmds[1].c_str());
			if(ns != nullptr) s = ns;
		} else cout <<"You haven't close current file yet\n";
	} else {
		filename = cmds[1];
		f = iniLoadFile(filename.c_str());
		if(f == nullptr) cout <<"Couldn't open such file\n";
	}
}

void create() {
	if(cmds.size() < 3) {
		cout <<"usage: new [file/section] name \n"
			   "   or  new key name [value]\n";
	} else if(cmds[1] == "file") {
		if(cmds.size() != 3)
			cout <<"usage: new [file/section] name \n"
				   "   or  new key name [value]\n";
		else {
			if(f)
				cout<<"You haven't close the file yet\n";
			else {
				f = iniNewFile();
				filename = cmds[2];
				needSave = true;
			}
		}
	} else if(cmds[1] == "section") {
		if(cmds.size() != 3)
			cout <<"usage: new [file/section] name \n"
				   "   or  new key name [value]\n";
		else {
			if(!f)
				cout <<"You haven't open a file yet\n";
			else if(s)
				cout <<"You haven't close the section yet\n";
			else {
				s = iniNewSection(f, cmds[2].c_str());
				needSave = true;
			}
		}
	} else if(cmds[1] == "key") {
		if(cmds.size() == 3) cmds.push_back("");
		else if(cmds.size() != 4)
			cout <<"usage: new [file/section] name \n"
				   "   or  new key name [value]\n";
		else if(!s)
			cout <<"You haven't open a section yet\n";
		else k = iniNewKey(s, cmds[2].c_str(), cmds[3].c_str()),
			 needSave = true;
	}
	cout <<flush;
}

void save() {
	if(cmds.size() > 1) {
		cout <<"usage: save"<<endl;
	} else if(!f) {
		if(cmds.size() == 1) cout <<"You haven't open a file yet\n";
	} else {
		iniSaveFile(f, filename.c_str());
		needSave = false;
	}
}

void rename() {
	if(cmds.size() < 3) {
		cout <<"usage: rename file name\n"
			   "   or  rename [section/key] [oldname] newname\n";
	} else if(cmds[1] == "file") {
		if(cmds.size() != 3)
			cout <<"usage: rename file name\n"
				   "   or  rename [section/key] [oldname] newname\n";
		else {
			if(!f)
				cout<<"You haven't open a file yet\n";
			else
				filename = cmds[2],
				needSave = true;
		}
	} else if(cmds[1] == "section") {
		switch(cmds.size()) {
			case 3: {
				if(!s)
					cout <<"You haven't open a section yet\n";
				else if(!iniChangeSectionName(s, cmds[2].c_str()))
					cout <<"Rename failed\n";
				else needSave = true;
			}; break;
			case 4: {
				if(!f)
					cout <<"You haven't open a file yet\n";
				else {
					INISection *ns = iniFindSection(f, cmds[2].c_str());
					if(ns == nullptr) cout <<"Couldn't find such section\n";
					else if(iniChangeSectionName(ns, cmds[3].c_str()) == INI_FAILED)
						cout <<"Rename failed\n";
					else s = ns,
						needSave = true;
				}
			}; break;
			default:{
				cout <<"usage: rename file name\n"
					   "   or  rename [section/key] [oldname] newname\n";
			}
		}
	} else if(cmds[1] == "key") {
		switch(cmds.size()) {
			case 3: {
				if(!k)
					cout <<"You haven't open a key yet\n";
				else if(iniChangeKeyName(k, cmds[2].c_str()) == INI_FAILED)
					cout <<"Rename failed";
				else needSave = true;
			}; break;
			case 4: {
				if(!s)
					cout <<"You haven't open a section yet\n";
				else {
					INIKey *nk = iniFindKey(s, cmds[2].c_str());
					if(nk == nullptr) cout <<"Couldn't find such key\n";
					else if(iniChangeKeyName(nk, cmds[3].c_str()) == INI_FAILED)
						cout <<"Rename failed\n";
					else k = nk, needSave = true;
				}
			}; break;
			default:{
				cout <<"usage: rename file name\n"
					   "   or  rename [section/key] [oldname] newname\n";
			}
		}
	}
	cout <<flush;
}

void set() {
	switch(cmds.size()) {
		case 2: {
			if(!k)
				cout <<"You haven't open a key yet\n";
			else if(iniChangeKeyValue(k, cmds[1].c_str()) == INI_FAILED)
				cout <<"Value set failed\n";
			else needSave = true;
		}; break;
		case 3: {
			if(!s)
				cout <<"You haven't open a section yet\n";
			else {
				INIKey *nk = iniFindKey(s, cmds[1].c_str());
				if(nk == nullptr)
					cout <<"Couldn't find such key\n";
				else if(iniChangeKeyValue(k, cmds[2].c_str()) == INI_FAILED)
					cout <<"Value set failed\n";
				else k = nk, needSave = true;
			}
		}; break;
		default: cout <<"usage: set [oldname] newname\n";
	}
	cout <<flush;
}

void close() {
	size_t size = cmds.size();
	if(size > 3 || (size == 3 && cmds[2] != "force")) {
		cout <<"usage: close [file/section/key] [force]\n";
	} else if(size == 1) {
		if(k) k = nullptr;
		else if(s) s = nullptr;
		else if(f) f = nullptr;
		else cout <<"You haven't open a file yet\n";
	} else if(cmds[1] == "file") {
		if(!f) cout <<"You haven't open a file yet\n";
		else if(needSave && size == 2) cout <<"The file haven't been saved yet\n";
		else filename = "", k = nullptr, s = nullptr, f = nullptr;
	} else if(cmds[1] == "section") {
		if(!s) cout <<"You haven't open a section yet\n";
		else k = nullptr, s = nullptr;
	} else if(cmds[1] == "key") {
		if(!k) cout <<"You haven't open a key yet\n";
		else k = nullptr;
	} else cout <<"usage: close [file/section/key] [force]\n";
}

void list() {
	size_t size = cmds.size();
	if(size > 3) {
		cout <<"usage: list file [all]\n"
			   "   or  list section\n"
			   "   or  list [all]";
	} else if(size == 1) {
		if(!f)
			cout <<"You haven't open a file yet\n";
		else if(!s) {
			cout <<filename <<':';
			for(INISection *s = iniGetFirstSection(f); s; s = iniGetNextSection(s))
				cout <<'[' <<iniGetSectionName(s) <<"]\n";
		} else {
			cout <<'[' <<iniGetSectionName(s) <<"]\n";
			for(INIKey *k = iniGetFirstKey(s); k; k = iniGetNextKey(k))
				cout <<iniGetKeyName(k) <<" = " <<iniGetKeyValue(k) <<'\n';
		}
	} else if(cmds[1] == "file") {
		if(!f)
			cout <<"You haven't open a file yet\n";
		else if(size == 2) {
			for(INISection *s = iniGetFirstSection(f); s; s = iniGetNextSection(s))
				cout <<'[' <<iniGetSectionName(s) <<"]\n";
		} else if(size == 3 && cmds[2] == "all") {
			for(INISection *s = iniGetFirstSection(f); s; s = iniGetNextSection(s)) {
				cout <<'[' <<iniGetSectionName(s) <<"]\n";
				for(INIKey *k = iniGetFirstKey(s); k; k = iniGetNextKey(k))
					cout <<iniGetKeyName(k) <<" = " <<iniGetKeyValue(k) <<'\n';
				cout <<'\n';
			}
		} else
			cout <<"usage: list file [all]\n"
				   "   or  list section\n";
	} else if(cmds[1] == "section") {
		if(!s)
			cout <<"You haven't open a section yet\n";
		else {
			cout <<'[' <<iniGetSectionName(s) <<"]\n";
			for(INIKey *k = iniGetFirstKey(s); k; k = iniGetNextKey(k))
				cout <<iniGetKeyName(k) <<" = " <<iniGetKeyValue(k) <<'\n';
		}
	} else if(cmds[1] == "all") {
		if(!f)
			cout <<"You haven't open a file yet\n";
		else {
			for(INISection *s = iniGetFirstSection(f); s; s = iniGetNextSection(s)) {
				cout <<'[' <<iniGetSectionName(s) <<"]\n";
				for(INIKey *k = iniGetFirstKey(s); k; k = iniGetNextKey(k))
					cout <<iniGetKeyName(k) <<" = " <<iniGetKeyValue(k) <<'\n';
				cout <<'\n';
			}
		}
	} else
		cout <<"usage: list file [all]\n"
			   "   or  list section\n"
			   "   or  list";
}

void del() {
	if(cmds.size() != 3) {
		cout <<"usage: del [section/key] name\n";
	} else if(cmds[1] == "section") {
		if(s && cmds[2] == iniGetSectionName(s)) {
			iniDeleteSection(s);
			s = nullptr;
		} else {
			s = iniFindSection(f, cmds[2].c_str());
			if(s == nullptr) {
				cout <<"Couldn't find such section\n";
			} else {
				iniDeleteSection(s);
				s = nullptr;
			}
		}
	} else if(cmds[1] == "key") {
		if(k && cmds[2] == iniGetKeyName(k)) {
			iniDeleteKey(k);
			k = nullptr;
		} else if(s) {
			k = iniFindKey(s, cmds[2].c_str());
			if(k == nullptr)
				cout <<"Couldn't find such key\n";
			else {
				iniDeleteKey(k);
				k = nullptr;
			}
		} else
			cout <<"You haven't open a section yet\n";
	} else {
		cout <<"usage: del [section/key] name\n";
	}
}

static map<string, void(*)()> commandMap;

struct __INIT_AND_DEINIT {
__INIT_AND_DEINIT() {
	commandMap["test"] = test;
	commandMap["open"] = open;
	commandMap["cd"] = open;
	commandMap["new"] = create;
	commandMap["save"] = save;
	commandMap["rename"] = rename;
	commandMap["close"] = close;
	commandMap["list"] = list;
	commandMap["ls"] = list;
	commandMap["q"] = quit;
	commandMap["quit"] = quit;
	commandMap["del"] = del;
	commandMap["rm"] = del;
}
~__INIT_AND_DEINIT() {
	cmds.clear();
	//save();
}
} _INIT_AND_DEINIT;

int main() {
	static string cmd;

	cout <<">>" <<flush;
	while(getline(cin, cmd)) {

		cmds.clear();
		int l = 0, len = cmd.length();
		for(int i = 0; i < len; ++i)
			if(isspace(cmd[i])) {
				if(l < i)
					cmds.push_back(string(cmd.begin() + l, cmd.begin() + i));
				l = i + 1;
			}
		if(l < len) cmds.push_back(string(cmd.begin() + l, cmd.begin() + len));
		// int size = cmds.size();
		// cout <<"Size:"<<size<<'\n';
		// for(int i = 0; i < size; ++i)
		// 	cout <<'#'<<i<<':'<<cmds[i]<<'\n';
		// cout <<endl;
		if(cmds.size() == 0) continue;
		auto it = commandMap.find(cmds[0]);
		if(it == commandMap.end()) help(commandMap);
		else it->second();
		cout <<'\n';
		if(f) {
			cout <<filename;
			if(s) {
				cout <<':' <<iniGetSectionName(s);
				if(k) cout <<':' <<iniGetKeyName(k);
			}
		}
		cout <<">>" <<flush;
	}
}