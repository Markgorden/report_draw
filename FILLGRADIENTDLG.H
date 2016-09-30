#if !defined(AFX_FILLGRADIENTDLG_H__740FA86A_EA71_46C4_B99F_743D94F491C7__INCLUDED_)
#define AFX_FILLGRADIENTDLG_H__740FA86A_EA71_46C4_B99F_743D94F491C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillGradientDlg.h : header file
#include "colourpicker.h"
Color SetA(Color&, BYTE);

/////////////////////////////////////////////////////////////////////////////
// CFillGradientDlg dialog
class CSampleButton : public CButton
{
// Construction
public:
    CSampleButton();
    DECLARE_DYNCREATE(CSampleButton);
private:

	void DrawItemPattern(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawItemLinear(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawItemPicture(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSampleButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	protected:
//	virtual void PreSubclassWindow();
//	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

};

class CFillGradientDlg : public CPropertyPage
{
// Construction
public:
	void SetValue();
	void DrawSample();
	CFillGradientDlg(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNAMIC(CFillGradientDlg);

// Dialog Data
	//{{AFX_DATA(CFillGradientDlg)
	enum { IDD = IDD_FILL_GRADIENT };
	CSampleButton	m_sample;
	CScrollBar	m_darkCtrl;
	CColourPicker	m_color2btn;
	CColourPicker	m_color1btn;
	CScrollBar	m_endTr;
	CScrollBar	m_startTr;
	int		m_startTransparency;
	int		m_endTransparency;
	int		m_dark;
	int		m_nStyle;
	//}}AFX_DATA

	BOOL    m_bInitilized;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillGradientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFillGradientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	afx_msg void OnRadio9();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLGRADIENTDLG_H__740FA86A_EA71_46C4_B99F_743D94F491C7__INCLUDED_)
