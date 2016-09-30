// Draw.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "Draw.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "DrawDoc.h"
#include "NetworkView.h"
//#include "BtnST.h"
#include "hyperlink.h"
#include "DrawProperty.h"
#include "UNDO MANAGER\UndoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDS_MAILADDR "mailto:tazst6@pitt.edu"
#define IDS_HOMEPAGEADDR  "http://www.pitt.edu/MetaboLogic" 


/////////////////////////////////////////////////////////////////////////////
// CDrawApp

BEGIN_MESSAGE_MAP(CDrawApp, CWinApp)
	//{{AFX_MSG_MAP(CDrawApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawApp construction

CDrawApp::CDrawApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	DWORD dwVersion = ::GetVersion();
	m_bWin4 = (BYTE)dwVersion >= 4;
	m_nDefFont = (m_bWin4) ? DEFAULT_GUI_FONT : ANSI_VAR_FONT;
	m_dcScreen.Attach(::GetDC(NULL));
	m_bStopbeforeEvaluate = FALSE;
	m_smoothingMode = SmoothingModeHighQuality;
	//m_smoothingMode = SmoothingModeHighSpeed;
	//m_smoothingMode = SmoothingModeDefault;
	m_bGamma = TRUE;
}

CDrawApp::~CDrawApp()
{
	if (m_dcScreen.m_hDC != NULL)
		::ReleaseDC(NULL, m_dcScreen.Detach());
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CDrawApp object

CDrawApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Customization load/save methods

void CDrawApp::PreLoadState ()
{
	GetMouseManager()->AddView (IDR_NETWORK, _T("My view"), IDR_NETWORK);
	// TODO: add another views and windows were mouse double click
	// customization is required
	GetContextMenuManager()->AddMenu (_T("My menu"), ID_POPUP_MENU);
	// TODO: add another context menus here
}

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {64222BB2-A5DC-11D3-B4DC-00A0C995EA3D}
static const CLSID clsid =
{ 0x64222bb2, 0xa5dc, 0x11d3, { 0xb4, 0xdc, 0x0, 0xa0, 0xc9, 0x95, 0xea, 0x3d } };

/////////////////////////////////////////////////////////////////////////////
// CDrawApp initialization

BOOL CDrawApp::InitInstance()
{

	SetRegistryKey(_T("Draw"));
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	gUndoManager = new CUndoManager();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	SetRegistryBase (_T("Settings"));

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();
	InitSkinManager ();

	//Using GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_NETWORK,
		RUNTIME_CLASS(CDrawDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CNetworkView));

	pDocTemplate->SetContainerInfo(IDR_DRAWTYPE_CNTR_IP);
	
	AddDocTemplate(pDocTemplate);
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_email;
	CHyperLink	m_www;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EMAIL, m_email);
	DDX_Control(pDX, IDC_WWW, m_www);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDrawApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDrawApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set HyperLink for E-Mail
	m_email.SetURL(_T(IDS_MAILADDR));
	m_email.SetUnderline(TRUE);
	m_email.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDER));

	// Set HyperLink for Home Page
	m_www.SetURL(_T(IDS_HOMEPAGEADDR));
	m_www.SetUnderline(TRUE);
	m_www.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDER));
		
	//SetLogoFont("Verdana", -25);

	// Write class version
	CString sVersion;
	sVersion.Format("MetaboLogic v%s","1.0");
	GetDlgItem(IDC_VERSION)->SetWindowText(sVersion);

	// Fill friends listbox
	CListBox* lbxFriends;
	lbxFriends = (CListBox*)GetDlgItem(IDC_FRIENDS);
	lbxFriends->AddString("Zhu Jian <jianz@andrew.cmu.edu>");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



int CDrawApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	Gdiplus::GdiplusShutdown(m_gdiplusToken);	
	
	delete gUndoManager;
	return CWinApp::ExitInstance();
}


CView* CDrawApp::GetActiveView()
{

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)m_pMainWnd;
	if (pFrame==NULL) return NULL;

	CMDIChildWnd *pChild = 
				 (CMDIChildWnd *) pFrame->GetActiveFrame();

	if (pChild==NULL) return NULL;

	return pChild->GetActiveView();
}
