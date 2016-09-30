/********************************************************************/
/* Draw.h:	Interface for CDraw					            */
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/

//

#if !defined(AFX_Draw_H__64222BB5_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_)
#define AFX_Draw_H__64222BB5_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawApp:
// See Draw.cpp for the implementation of this class
//

class CDrawApp : public CWinApp, public CBCGPWorkspace
{
public:
	int m_projectType;
	CDrawApp();
	~CDrawApp();

public:
	CView* GetActiveView();
	BOOL		m_bWin4;
	CDC			m_dcScreen;
	int			m_nDefFont;

	//OPTIONS values get from registry.
	BOOL		m_bStopbeforeEvaluate;
	BOOL		m_bAutoUpdate;
	BOOL		m_bAutoMoleculeWeight;
	BOOL		m_bSyntaxChecking;
	UINT		m_stackDepth;
	UINT		m_fluxFontSize;
	SmoothingMode m_smoothingMode;
	BOOL		m_bGamma;
private:
	ULONG_PTR m_gdiplusToken;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Override from CBCGWorkspace
	virtual void PreLoadState();
// Implementation
//	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CDrawApp)
	afx_msg void OnAppAbout();
	afx_msg void OnNextPage();
	afx_msg void OnPrevPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDrawApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Draw_H__64222BB5_A5DC_11D3_B4DC_00A0C995EA3D__INCLUDED_)
