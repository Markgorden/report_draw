/********************************************************************/
/* MainFrm.h: Interface for CMainFrm					            */
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/********************************************************************/

#ifndef MELAB_MAINFRAME_H
#define MELAB_MAINFRAME_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "WorkspaceBar.h"
#include "formatba.h"
#include "colorlis.h"

class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
//	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPMenuBar			m_wndMenuBar;

	CBCGPToolBar			m_wndToolBar;
	CBCGPToolBar			m_netToolBar;
	CFormatBar			m_formatToolBar;

	CBCGPToolBarImages	m_UserImages;
public:
	CStatusBar			m_wndStatusBar;

public:
	CDocument* GetActiveDoc();
	void UpdateFormatBar();

//Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewCustomize();
	afx_msg void OnViewFormatToolbar();
	afx_msg void OnUpdateViewFormatToolbar(CCmdUI* pCmdUI);
	afx_msg void OnLineWidth();
	afx_msg void OnLineStype();
	afx_msg void OnLeftArrow();
	afx_msg void OnRightArrow();
	//}}AFX_MSG
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	

	BOOL OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeColor(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined MELAB_MAINFRAME_H
