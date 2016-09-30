// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Draw.h"
#include "networkview.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;


BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_COMMAND(ID_VIEW_FORMAT_TOOLBAR, OnViewFormatToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORMAT_TOOLBAR, OnUpdateViewFormatToolbar)
	ON_COMMAND(ID_LINE_WIDTH, OnLineWidth)
	ON_COMMAND(ID_LINE_STYPE, OnLineStype)
	ON_COMMAND(ID_LEFTARROW_STYLE, OnLeftArrow)
	ON_COMMAND(ID_RIGHTARROW_STYLE, OnRightArrow)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// Global help commands

	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND_EX_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
	ON_COMMAND(ID_HELP_FINDER, CBCGPMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CBCGPMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CBCGPMDIFrameWnd::OnContextHelp)

//	ON_REGISTERED_MESSAGE(CPN_SELCHANGE, OnChangeColor)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_PAGE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//--------------------------
	// Load toolbar user images:
	//--------------------------
	if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
	{
		TRACE(_T("Failed to load user images\n"));
		//ASSERT (FALSE);
	}
	else
	{
		CBCGPToolBar::SetUserImages (&m_UserImages);
	}
	CBCGPToolBar::EnableQuickCustomization ();


	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCoomads;

	lstBasicCoomads.AddTail (ID_FILE_NEW);
	lstBasicCoomads.AddTail (ID_FILE_OPEN);
	lstBasicCoomads.AddTail (ID_FILE_SAVE);
	lstBasicCoomads.AddTail (ID_FILE_SAVE_AS);
	lstBasicCoomads.AddTail (ID_FILE_PRINT);
	lstBasicCoomads.AddTail (ID_APP_EXIT);

	lstBasicCoomads.AddTail (ID_EDIT_UNDO);
	lstBasicCoomads.AddTail (ID_EDIT_CUT);
	lstBasicCoomads.AddTail (ID_EDIT_PASTE);

	lstBasicCoomads.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCoomads.AddTail (ID_VIEW_STATUS_BAR);
	lstBasicCoomads.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCoomads.AddTail (ID_VIEW_TOOLBARS);
	//lstBasicCoomads.AddTail (ID_VIEW_PAPERCOLOR);

	lstBasicCoomads.AddTail (ID_MODE_STOICHIOMETRIC);
	lstBasicCoomads.AddTail (ID_MODE_KINECTICMODEL);
	lstBasicCoomads.AddTail (ID_DUMMY_BUILD);
	lstBasicCoomads.AddTail (ID_BUILD_DUMP);
	lstBasicCoomads.AddTail (ID_BUILD_EXPORT_CODE);


	lstBasicCoomads.AddTail (ID_TOOL_FLUX);
	lstBasicCoomads.AddTail (ID_TOOL_FLUXOPTIMIZATION_ONESOLUTION);
	lstBasicCoomads.AddTail (ID_TOOL_SIMULATE);
	lstBasicCoomads.AddTail (ID_TOOL_NMR);
	lstBasicCoomads.AddTail (ID_TOOL_INFO);

	lstBasicCoomads.AddTail (ID_WINDOW_CASCADE);
	lstBasicCoomads.AddTail (ID_WINDOW_TILE_HORZ);

	lstBasicCoomads.AddTail (ID_APP_ABOUT);
	CBCGPToolBar::SetBasicCommands (lstBasicCoomads);

	//-----------------
	// Create menu bar:
	//-----------------
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	//---------------------
	// Create main toolbar:
	//---------------------
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

	//----------------------
	// Create Network toolbar:
	//----------------------
	if (!m_netToolBar.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		ID_VIEW_BUILD_TOOLBAR) ||
		!m_netToolBar.LoadToolBar (IDR_NETWORK))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	CString strNetToolbarTitle;
	strNetToolbarTitle.LoadString (IDS_BUILD_TOOLBAR);
	m_netToolBar.SetWindowText (strNetToolbarTitle);

	//----------------------
	// Create format toolbar:
	//----------------------
	if (!m_formatToolBar.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		ID_VIEW_FORMAT_TOOLBAR) ||
		!m_formatToolBar.LoadToolBar (IDR_FORMAT_TOOLBAR))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	CString strFormatToolbarTitle;
	strFormatToolbarTitle.LoadString (IDR_FORMAT_TOOLBAR);
	m_formatToolBar.SetWindowText (strFormatToolbarTitle);


	//-------------------
	// Create status bar:
	//-------------------
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//--------------------------------
	// Create docking control windows:
	//--------------------------------

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_netToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_formatToolBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_formatToolBar);
	DockControlBarLeftOf (&m_netToolBar, &m_wndToolBar);

	//----------------------------------------
	// Allow user-defined toolbars operations:
	//----------------------------------------
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (LOWORD (lp), HIWORD(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

BOOL CMainFrame::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return FALSE ;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->GetStyle () & WS_VISIBLE), FALSE, FALSE);
	RecalcLayout ();
	return TRUE;
}

void CMainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return ;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->GetStyle () & WS_VISIBLE);
	return;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_MAINFRAME:
		{
			//--------------------------------------
			// Replace "Find..." button by combobox:
			//--------------------------------------
			/*
			m_wndToolBar.ReplaceButton (ID_EDIT_FIND, 
				CBCGToolbarComboBoxButton (ID_EDIT_FIND_COMBO, 
				CImageHash::GetImageOfCommand (ID_EDIT_FIND, FALSE),
				CBS_DROPDOWN));

			*/
			//-------------------------------------
			// Replace "Undo" button by menubutton:
			//-------------------------------------

			
			CMenu menu;
			menu.LoadMenu (IDR_UNDO);

			CMenu* pPopupMenu = menu.GetSubMenu (0);
			VERIFY (pPopupMenu != NULL);
		}
		break;

	case IDR_NETWORK: 
		break;
	}

	return 0;
}

LRESULT CMainFrame::OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp)
{
	int iPageNum = (int) wp;

	CBCGPToolbarCustomize* pDlg = (CBCGPToolbarCustomize*) lp;
	ASSERT_VALID (pDlg);

	// TODO: show help about page number iPageNum


	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
    CBCGPMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup == NULL)
	{
		return TRUE;
	}

    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }


	return TRUE;
}



void CMainFrame::OnClose() 
{
	CBCGPMDIFrameWnd::OnClose();
}

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */);

	pDlgCust->EnableUserDefinedToolbars ();

	//-----------------
	// Setup combboxes:
	//-----------------
	CBCGPToolbarComboBoxButton comboButtonFind (ID_EDIT_FIND_COMBO, 
			CImageHash::GetImageOfCommand (ID_EDIT_FIND, FALSE),
			CBS_DROPDOWN);
	pDlgCust->ReplaceButton (ID_EDIT_FIND, comboButtonFind);

	CBCGPToolbarComboBoxButton comboButtonConfig (ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, 
			CImageHash::GetImageOfCommand (ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, FALSE),
			CBS_DROPDOWNLIST);
	comboButtonConfig.AddItem (_T("Win32 Debug"));
	comboButtonConfig.AddItem (_T("Win32 Release"));
	comboButtonConfig.SelectItem (0);

	pDlgCust->ReplaceButton (ID_DUMMY_SELECT_ACTIVE_CONFIGURATION, comboButtonConfig);

	//-------------------------
	// Setup undo/redo buttons:
	//-------------------------
	CMenu menu;
	menu.LoadMenu (IDR_UNDO);

	CMenu* pPopupMenu = menu.GetSubMenu (0);
	VERIFY (pPopupMenu != NULL);

	pDlgCust->ReplaceButton (ID_EDIT_UNDO, CBCGPToolbarMenuButton 
		(ID_EDIT_UNDO, pPopupMenu->GetSafeHmenu (), 
		CImageHash::GetImageOfCommand (ID_EDIT_UNDO, FALSE)));

	pDlgCust->ReplaceButton (ID_EDIT_REDO, CBCGPToolbarMenuButton 
		(ID_EDIT_REDO, pPopupMenu->GetSafeHmenu (), 
		CImageHash::GetImageOfCommand (ID_EDIT_REDO, FALSE)));

	pDlgCust->Create ();
}


LRESULT CMainFrame::OnChangeColor(WPARAM wp, LPARAM lp)
{
//	CNetworkView* pView = DYNAMIC_DOWNCAST (CNetworkView, GetActiveView());
//  GetActivateView doesn't work for CMDIFrameWnd
//  Use following code, to get the pointer of active View
	//1, Get the active MDI child window.
	//CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//2. Get the active view attached to the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) GetActiveFrame();
    CNetworkView* pView = DYNAMIC_DOWNCAST (CNetworkView, pChild->GetActiveView());
	
    if (pView != NULL)
	{
		switch ((UINT)lp){
		case ID_CHAR_COLOR:
			pView->SetTextColor ((COLORREF) wp);
			pView->OnCharColor();
			break;
		case ID_LINE_COLOR:
			pView->SetLineColor ((COLORREF) wp);
			pView->OnLineColor();
			break;
		case ID_FILL_COLOR:
			pView->SetFillColor ((COLORREF) wp);
			pView->OnFillColor();
			break;
		}			
	}

	return 0l;
}

void CMainFrame::OnViewFormatToolbar() 
{
	// TODO: Add your command handler code here
	ShowControlBar (&m_formatToolBar,
					!(m_formatToolBar.GetStyle () & WS_VISIBLE),
					FALSE,
					FALSE);

}

void CMainFrame::OnUpdateViewFormatToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_formatToolBar.GetStyle () & WS_VISIBLE);
	
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{

	CWnd* pWnd=GetFocus();
	if (pWnd){
		if (pWnd->IsKindOf(RUNTIME_CLASS(CNetworkView)))
			return CBCGPMDIFrameWnd::OnCommand(wParam, lParam);
	}

	if (/*CLocalComboBox::m_bHasFocus  &&*/
		HIWORD (wParam) == 1)	// Command from accelerator
	{
		UINT uiCmd = LOWORD (wParam);
		switch (uiCmd)
		{
		case ID_EDIT_PASTE:
			::SendMessage (::GetFocus (), WM_PASTE, 0, 0);
			return TRUE;

		case ID_EDIT_COPY:
			::SendMessage (::GetFocus (), WM_COPY, 0, 0);
			return TRUE;

		case ID_EDIT_CUT:
			::SendMessage (::GetFocus (), WM_CUT, 0, 0);
			return TRUE;

		case ID_EDIT_CLEAR:
			::SendMessage (::GetFocus (), WM_CLEAR, 0, 0);
			return TRUE;
		}
	}

	if (!pWnd)
		return CBCGPMDIFrameWnd::OnCommand(wParam, lParam);

	return CBCGPMDIFrameWnd::OnCommand(wParam, lParam);
}


void CMainFrame::OnLineWidth() 
{
	//m_formatToolBar update 
	UpdateFormatBar();	

	CLineWidthMenu lineWidthMenu;
	CRect rc;
	int index = m_formatToolBar.CommandToIndex(ID_LINE_WIDTH);
	m_formatToolBar.GetItemRect(index, &rc);
	m_formatToolBar.ClientToScreen(rc);
	lineWidthMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);
	
}

void CMainFrame::OnLineStype() 
{
	UpdateFormatBar();	

	m_formatToolBar.OnUpdateCmdUI(this,TRUE);
	CLineStyleMenu lineStyleMenu;
	CRect rc;
	int index = m_formatToolBar.CommandToIndex(ID_LINE_STYPE);
	m_formatToolBar.GetItemRect(index, &rc);
	m_formatToolBar.ClientToScreen(rc);
	lineStyleMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);
	
}

void CMainFrame::OnLeftArrow() 
{
	
	UpdateFormatBar();	

	m_formatToolBar.OnUpdateCmdUI(this,TRUE);
	int index = m_formatToolBar.CommandToIndex(ID_LEFTARROW_STYLE);
	CLeftArrowMenu lMenu;
	CRect rc;
	m_formatToolBar.GetItemRect(index, &rc);
	m_formatToolBar.ClientToScreen(rc);
	lMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);
}

void CMainFrame::OnRightArrow() 
{
	//m_formatToolBar update 
	UpdateFormatBar();	

	m_formatToolBar.OnUpdateCmdUI(this,TRUE);
	CRightArrowMenu rMenu;
	CRect rc;
	int index = m_formatToolBar.CommandToIndex(ID_RIGHTARROW_STYLE);
	m_formatToolBar.GetItemRect(index, &rc);
	m_formatToolBar.ClientToScreen(rc);
	rMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);
}

void CMainFrame::UpdateFormatBar()
{
	//m_formatToolBar update 
	CRect rect,rect1;
	
	int index = m_formatToolBar.CommandToIndex(ID_LEFTARROW_STYLE);
	m_formatToolBar.GetItemRect(index, rect);
	rect1 = rect;

	index = m_formatToolBar.CommandToIndex(ID_RIGHTARROW_STYLE);
	m_formatToolBar.GetItemRect(index, rect);
	rect1.UnionRect(rect1, rect);

	index = m_formatToolBar.CommandToIndex(ID_LINE_STYPE);
	m_formatToolBar.GetItemRect(index, rect);
	rect1.UnionRect(rect1, rect);

	index = m_formatToolBar.CommandToIndex(ID_LINE_WIDTH);
	m_formatToolBar.GetItemRect(index, rect);
	rect1.UnionRect(rect1, rect);

	m_formatToolBar.InvalidateRect(rect1);
	//================
}

CDocument* CMainFrame::GetActiveDoc()
{
	CMDIFrameWnd *pFrame = 
				 (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = 
				 (CMDIChildWnd *) pFrame->GetActiveFrame();

	CNetworkView *pView = (CNetworkView *) pChild->GetActiveView();
	return (CDocument*) (pView->GetDocument());
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd = GetFocus();
	if (pWnd)
		if (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) //Edit the Grid
			return false;
		
	return CBCGPMDIFrameWnd::PreTranslateMessage(pMsg);
}



BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// By Andy Galland

	CWnd* pWndFocus = GetFocus ();

	if (pWndFocus != NULL)
	{
		//see if the window can handle this message
		if (pWndFocus->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo))
		{
			return TRUE;
		}
	}
	
	return CBCGPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

