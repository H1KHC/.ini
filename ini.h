
#ifndef __INI_LOADER_H__
#define __INI_LOADER_H__

#include <stddef.h>		//To use size_t

//#define INI_THREAD_SAFE

#ifdef __cplusplus
#	define DEFAULT(x) = (x)
#else
#	define DEFAULT(...)
#endif

#ifndef INI_DYNAMIC
#	define INI_API
#else
#	ifdef WIN32
#		ifdef INI_IMPLEMENT
#			define INI_API __declspec(dllexport)
#		else
#			define INI_API __declspec(dllimport)
#		endif
#	else
#		define INI_API __attribute__((visibility("hidden")))
#	endif
#endif

#define INI_SUCCEEDED	1
#define INI_FAILED		0

#ifdef __cplusplus
extern "C" {
#endif

enum INI_ERR{
INI_ERR_NO_ERROR,
INI_ERR_ILLEGAL_VALUE,
INI_ERR_NAME_CONFLICTED,
INI_ERR_FILE_OPEN_FAILED,
INI_ERR_FILE_PARSE_FAILED
};

INI_API INI_ERR iniGetError();

struct INIFile;
struct INISection;
struct INIKey;
typedef struct INIFile INIFile;
typedef struct INISection INISection;
typedef struct INIKey INIKey;

// File
INI_API INIFile*		iniNewFile			();
INI_API INIFile*		iniLoadFile			(const char *file);
INI_API int				iniSaveFile			(INIFile*, const char *file);

INI_API size_t			iniGetFileSize		(INIFile*);

INI_API void			iniCleanFile		(INIFile*);

INI_API void			iniCloseFile		(INIFile*);


// Sections
INI_API INISection*		iniNewSection		(INIFile*, const char *name);
INI_API INISection*		iniFindSection		(INIFile*, const char *name);

INI_API INISection*		iniGetFirstSection	(INIFile*);

INI_API INISection*		iniGetNextSection	(INISection*);
INI_API INISection*		iniGetLastSection	(INISection*);

INI_API const char *	iniGetSectionName	(INISection*);
INI_API size_t			iniGetSectionSize	(INISection*);

INI_API int				iniChangeSectionName(INISection*, const char *name);
 
INI_API void			iniCleanSection		(INISection*);

INI_API void			iniDeleteSection	(INISection*);


// Keys
INI_API INIKey*			iniNewKey			(INISection*, const char *name,
														  const char *value);
INI_API INIKey*			iniFindKey			(INISection*, const char *name);

INI_API INIKey*			iniGetFirstKey		(INISection*);

INI_API INIKey*			iniGetNextKey		(INIKey*);
INI_API INIKey*			iniGetLastKey		(INIKey*);

INI_API const char *	iniGetKeyName		(INIKey*);
INI_API const char *	iniGetKeyValue		(INIKey*);
INI_API int				iniChangeKeyName	(INIKey*, const char *name);
INI_API int				iniChangeKeyValue	(INIKey*, const char *value);

INI_API void			iniDeleteKey		(INIKey*);

#ifdef __cplusplus
}
#endif

#endif