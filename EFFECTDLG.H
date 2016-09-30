#if !defined(AFX_EFFECTDLG_H__7ACECE1C_9DCC_4348_9CE9_38330118B83C__INCLUDED_)
#define AFX_EFFECTDLG_H__7ACECE1C_9DCC_4348_9CE9_38330118B83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectDlg.h : header file
//

#include "resource.h"
#include "mytabctrl.h"
class CFillGradientDlg;
class CFillPatternDlg;
class CFillPictureDlg;

/////////////////////////////////////////////////////////////////////////////
// CEffectDlg dialog

class CEffectDlg : public CDialog
{
// Construction
public:
	CEffectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEffectDlg)
	enum { IDD = IDD_FILL_DLG };
	CMyTabCtrl	m_Tab;
	//}}AFX_DATA

	int				   m_effectpage;
	CFillGradientDlg *m_pGradientDlg;
	CFillPatternDlg  *m_pPatternDlg;
	CFillPictureDlg  *m_pPictureDlg;

	//using GDI+
    Color				m_StartColor;
	Color				m_EndColor;
	BrushType			m_BrushType;
	LinearGradientMode	m_linearFillMode;
	CString				m_fileName;
	HatchStyle			m_hatchStyle;
	BYTE				m_nColor;
	WrapMode			m_wrapMode;
	BYTE				m_nStyle;
	BYTE				m_nStretch;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEffectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTDLG_H__7ACECE1C_9DCC_4348_9CE9_38330118B83C__INCLUDED_)
