#ifndef __FAST_IO_H__
#define __FAST_IO_H__
#include <cstdio>

#pragma GCC push_options
#pragma GCC optimize ("O0")

template<int BUFFER_SIZE = 1048576>
class FAST_IN {
	char buf[BUFFER_SIZE];
	int pt, size;
	FILE *fp;
	bool getBuf() {
		if(fp == nullptr) return false;
		size = fread(buf, sizeof(char), BUFFER_SIZE - 1, fp);
		pt = 0;
		return size > 0;
	}
public:
	bool open(const char *fileName, const char *mod) {
		if(fp != nullptr) return false;
		fp = fopen(fileName, mod);
		pt = 0, size = 0;
		return fp != nullptr;
	}

	bool close() {
		if(fp == nullptr) return false;
		fclose(fp);
		return true;
	}

	int getch() {
		if(pt == size && !getBuf()) return EOF;
		return buf[pt++];
	}

	struct JUDGER_LINE{bool operator()(const int ch)const{return ch != '\n';}};
	struct JUDGER_SPACE{bool operator()(const int ch)const{return !isspace(ch);}};

	template<class JUDGER = JUDGER_SPACE>
	int get_str(char *str, JUDGER judger) {
		int ch = getch(), len;
		while(ch != EOF && !judger(ch)) ch = getch();
		if(ch == EOF) {
			*str = '\0';
			return EOF;
		}
		do *(str++) = ch, ch = getch(), len++; while(ch != EOF && judger(ch));
		*str = '\0';
		return len;
	}

	int get_line(char *str) { return get_str(str, JUDGER_LINE()); }

	template<class T>
	T get_int() {
		int ch, flag = 1;
		T x;
		for(ch = getch(); ch != EOF && (unsigned)(ch ^'0') > 9; ch = getch())
			if(ch == '-') flag = -1;
		if(ch == EOF) return EOF;
		for(x = ch ^ '0', ch = getch(); (unsigned)(ch ^ '0') < 10; ch = getch())
			x = (x << 3) + (x << 1) + (ch ^ '0');
		return x * flag;
	}

	FAST_IN& operator >> (char &ch)  { return ch = getch(), *this; }
	FAST_IN& operator >> (int  &x)   { return x = get_int(), *this; }
	FAST_IN& operator >> (char *str) { return get_str(str, JUDGER_SPACE()); }

	FAST_IN() : pt(0), size(0), fp(nullptr) {}
	FAST_IN(const char *filename, const char *mod = "r") :
		pt(0), size(0), fp(fopen(filename, mod)) {}
	~FAST_IN() { close(); }
};

template<int BUFFER_SIZE = 1048576>
class FAST_OUT {
	char buf[BUFFER_SIZE];
	int pt;
	FILE* fp;

	void flush() {
		if(pt) {
			int sz = fwrite(buf, sizeof(char), pt, fp);
			//if(sz != pt) ERROR_OCCURED;
			sz = sz;
		}
	}

public:
	bool open(const char *fileName, const char *mod) {
		if(fp != nullptr) return false;
		fp = fopen(fileName, mod);
		pt = 0;
		return fp != nullptr;
	}

	bool close() {
		if(pt) flush();
		if(fp == nullptr) return false;
		fclose(fp);
		return true;
	}

	void putch(char ch) {
		buf[pt++] = ch;
		(pt == BUFFER_SIZE) ? flush() : void();
	}
	void put_str(const char *str) {
		while(*str) putch(*(str++));
	}
	void put_str(const char *str, int len) {
		if(pt + len >= BUFFER_SIZE) flush();
		memcpy(buf + pt, str, sizeof(char) * len);
		pt += len;
	}

	template<class T>
	void put_int(T x) {
		char temp[23];
		int top;
		if(x == 0) return putch('0');
		if(x < 0) putch('-'), x = -x;
		for(top = 0; x; temp[++top] = x % 10, x /= 10);
		do putch(temp[top] ^ '0'); while(--top);
	}

	FAST_OUT& operator << (const char ch) { return putch(ch), *this; }
	FAST_OUT& operator << (const char* s) { return put_str(s), *this; }
	FAST_OUT& operator << (const int x) { return put_int(x), *this; }
	FAST_OUT& operator << (const long long x) { return put_int(x), *this; }

	FAST_OUT() : pt(0), fp(nullptr) {}
	FAST_OUT(const char *filename, const char *mod = "w") :
		pt(0), fp(fopen(filename, mod)) {}
	~FAST_OUT() { close(); }
};

#pragma GCC pop_options

#endif