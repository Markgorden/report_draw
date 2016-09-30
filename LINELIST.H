#if !defined(AFX_LINELIST1_H__4CB6D311_AD9C_11D3_B92E_0050DA121393__INCLUDED_)
#define AFX_LINELIST1_H__4CB6D311_AD9C_11D3_B92E_0050DA121393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineList1.h : header file
/********************************************************************/
/*				Tao Zhu, September, 1999							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 1999, all rights reserved.				*/
/********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CLineList window

class CLineList : public CComboBox
{
// Construction
public:
	CLineList();

// Attributes
public:
	void AddColorItem(COLORREF color);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCIS);
	virtual ~CLineList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELIST1_H__4CB6D311_AD9C_11D3_B92E_0050DA121393__INCLUDED_)
