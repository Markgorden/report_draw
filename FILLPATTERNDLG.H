#if !defined(AFX_FILLPATTERNDLG_H__0D46AF5B_E8A3_4D87_A2BC_01A8B8D165AE__INCLUDED_)
#define AFX_FILLPATTERNDLG_H__0D46AF5B_E8A3_4D87_A2BC_01A8B8D165AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillPatternDlg.h : header file
#include "fillgradientdlg.h"
#include "HatchWellWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CFillPatternDlg dialog
class CFillPatternDlg : public CPropertyPage
{
// Construction
public:
	CFillPatternDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNAMIC(CFillPatternDlg);
// Dialog Data
	//{{AFX_DATA(CFillPatternDlg)
	enum { IDD = IDD_FILL_PATTERN };
	CHatchWellWnd	m_hatchbtn;
	CSampleButton	m_sample;
	CColourPicker	m_backColorCtrl;
	CColourPicker	m_foreColorCtrl;
	//}}AFX_DATA

	CHatchWellWnd *pWell;
	void DrawSample();
	void SetValue();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillPatternDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFillPatternDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelHatchEndOK(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLPATTERNDLG_H__0D46AF5B_E8A3_4D87_A2BC_01A8B8D165AE__INCLUDED_)
