#if !defined(AFX_HATCHWELLWND_H__EBF4CD97_65C2_4F2E_8599_9F1D8A99B2F9__INCLUDED_)
#define AFX_HATCHWELLWND_H__EBF4CD97_65C2_4F2E_8599_9F1D8A99B2F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HatchWellWnd.h : header file
#define HWN_SELCHANGEBT      WM_USER + 1021        // Colour Picker Selection change
#define HWN_DROPDOWN         WM_USER + 1022        // Colour Picker drop down
#define HWN_CLOSEUP          WM_USER + 1023        // Colour Picker close up
#define HWN_SELENDOK         WM_USER + 1024        // Colour Picker end OK
#define HWN_SELENDCANCEL     WM_USER + 1025        // Colour Picker end (cancelled)

/////////////////////////////////////////////////////////////////////////////
// CHatchWellWnd window

class CHatchWellWnd : public CButton
{
// Construction
public:
	CHatchWellWnd();
    CHatchWellWnd(CPoint p, int hatchType, CWnd* pParentWnd);
    void Initialise();

// Attributes
public:
    BOOL Create(CPoint p, int hatchType, CWnd* pParentWnd);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHatchWellWnd)
	//}}AFX_VIRTUAL
public:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    void FindCellFromHatch(int nHatch);
    void SetWindowSize();
    //void CreateToolTips();
    void ChangeSelection(int nIndex);
    void EndSelection(int nMessage);
    void DrawCell(CDC* pDC, int nIndex);

    LPCTSTR GetHatchName(int nIndex)           { return m_hatchName[nIndex]; }
    int  GetIndex(int row, int col) const;
    int  GetRow(int nIndex) const;
    int  GetColumn(int nIndex) const;

	int  m_crHatch,
		 m_nNumHatchs,
		 m_crInitialHatch;
 

// protected attributes
protected:
    static CString m_hatchName[];
    int            m_nNumColours;
    int            m_nNumColumns, m_nNumRows;
    int            m_nBoxSize, m_nMargin;
    int            m_nCurrentSel;
    int            m_nChosenHatchSel;
    CRect          m_WindowRect;
    //CFont          m_Font;
    //CToolTipCtrl   m_ToolTip;
public:
    CWnd*          m_pParent;

	
	BOOL		   m_bOffic2000Style;
	//using GDI+
public:
    Color				m_foreColor;
	Color				m_backColor;
	BrushType			m_BrushType;
// Implementation
public:
	virtual ~CHatchWellWnd();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHatchWellWnd)
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HATCHWELLWND_H__EBF4CD97_65C2_4F2E_8599_9F1D8A99B2F9__INCLUDED_)
