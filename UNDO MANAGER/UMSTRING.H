
#if !defined(UMSTRING_H)
#define UMSTRING_H

#ifdef _MFC_VER

#include <afx.h>
typedef CString				UMString;

#else

#include <string>
typedef std::string			UMString;

#endif

#endif
