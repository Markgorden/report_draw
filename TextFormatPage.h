#if !defined(AFX_TEXTFORMATPAGE_H__DC0A8DA9_CE5C_42D4_8304_04266E8EA915__INCLUDED_)
#define AFX_TEXTFORMATPAGE_H__DC0A8DA9_CE5C_42D4_8304_04266E8EA915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextFormatPage.h : header file
#include "ColourPicker.h"
#include "ArrowPicker.h"
#include "line.h"
#include "linePicker.h"
#include "drawobj.h"
//

/////////////////////////////////////////////////////////////////////////////
// CTextFormatPage dialog

class CTextFormatPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextFormatPage)

// Construction
public:
	void GetValues(CDrawText*);
	void SetValues(CDrawText*);
	CTextFormatPage();
	~CTextFormatPage();

// Dialog Data
	//{{AFX_DATA(CTextFormatPage)
	enum { IDD = IDD_TEXT_FORMAT };
	CScrollBar	m_transCtrl;
	CLinePicker	m_linePicker;
	CColourPicker	m_LinePicker;
	CColourPicker	m_FillPicker;
	int		m_borderType;
	int		m_allignment;
	UINT	m_penSize;
	int		m_Fill;
	BOOL	m_round;
	UINT	m_Transparency;
	//}}AFX_DATA
	DashStyle m_dashStyle;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
	CDrawObj* pObj;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextFormatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextFormatPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTFORMATPAGE_H__DC0A8DA9_CE5C_42D4_8304_04266E8EA915__INCLUDED_)
