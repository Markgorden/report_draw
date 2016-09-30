// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__64222BB7_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_)
#define AFX_STDAFX_H__64222BB7_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#if _MSC_VER >= 1300
	#define WPD_CHARFORMAT CHARFORMAT2
	#define WPD_PARAFORMAT PARAFORMAT2
#else
	#define WPD_CHARFORMAT CHARFORMAT
	#define WPD_PARAFORMAT PARAFORMAT
#endif

struct CCharFormat : public WPD_CHARFORMAT
{
	CCharFormat() {cbSize = sizeof(WPD_CHARFORMAT);}
	BOOL operator==(CCharFormat& cf);
};

typedef struct {
    COLORREF crColour;
    TCHAR    *szName;
} ColourTableEntry;

#include <afxtempl.h>       // drawobj.h uses collection class templates

#include <BCGCBProInc.h>

#include <gdiplus.h>

using namespace Gdiplus;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__64222BB7_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_)
