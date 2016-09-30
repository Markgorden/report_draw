/********************************************************************/
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/
#if !defined(AFX_LINEPROPERTY_H__04B0EAD2_ACD3_11D3_B929_0050DA078B89__INCLUDED_)
#define AFX_LINEPROPERTY_H__04B0EAD2_ACD3_11D3_B929_0050DA078B89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineProperty.h : header file
//

#include "ColourPicker.h"
#include "ArrowPicker.h"
#include "line.h"
#include "linePicker.h"
#include "drawobj.h"
/////////////////////////////////////////////////////////////////////////////
// CLineProperty dialog

class CLineProperty : public CPropertyPage
{
	DECLARE_DYNCREATE(CLineProperty)

// Construction
public:
	void GetValues(CDrawObj *);
	void SetValues(CDrawObj *);
	void EnableArrowSetting( bool set);
	static ArrowTypeG GetArrowType(int );
	static int GetArrowIndex(ArrowTypeG);
	CLineProperty();
	~CLineProperty();

// Dialog Data
	//{{AFX_DATA(CLineProperty)
	enum { IDD = IDD_LINE };
	CScrollBar	m_transCtrl;
	CLinePicker	m_linePicker;
	CArrowPicker	m_arrowEndPicker;
	CArrowPicker	m_arrowStartPicker;
	CColourPicker	m_FillPicker;
	CColourPicker	m_LinePicker;
	int	m_Fill;
	UINT	m_penSize;
	int	m_nArrowStart;
	int	m_nArrowEnd;
	BOOL	m_round;
	BOOL	m_bCurve;
	int		m_Transparency;
	//}}AFX_DATA
//	ArrowType m_arrowStart;
//	ArrowType m_arrowEnd;
//	CDashLine::DashStyle m_lineStyle;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
	bool m_isArrow;
	CDrawObj				* pObj;
	DashStyle				m_dashStyle;
	DashCap					m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
	BrushType               m_brushType;
	PenType					m_penType;
	Color  				    m_brushColor, //when hatch pattern used, this is foreColor
	                        m_gradientColor,
							m_penColor,
							m_penEndColor;
	int						m_penWidth;
	LinearGradientMode      m_linearGradientMode; //for linear gradient only;
	HatchStyle              m_hatchStyle;
	int                     m_startArrowSize,  // relative Arrow Size:  m_startArrowSize/10 * m_penWidth is the real size
	                        m_endArrowSize;
	int						m_startArrow,
							m_endArrow;
	CString					m_fillImage;
	WrapMode                m_wrapMode;
	int						m_FillMode; //0, - center path gradient.
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLineProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLineProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPROPERTY_H__04B0EAD2_ACD3_11D3_B929_0050DA078B89__INCLUDED_)
