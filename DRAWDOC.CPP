// DrawDoc.cpp : implementation of the CDrawDoc class
//

#include "stdafx.h"
#include "Draw.h"
#include <afxpriv.h>

#include "DrawDoc.h"
#include "CntrItem.h"
#include "networkview.h"
#include "mainfrm.h"
#include "DrawProperty.h"
#include "UNDO MANAGER\UndoManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <direct.h>
 
#define  BUFSIZE 256

static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 
/////////////////////////////////////////////////////////////////////////////
// CDrawDoc

IMPLEMENT_DYNCREATE(CDrawDoc, COleDocument)

BEGIN_MESSAGE_MAP(CDrawDoc, COleDocument)
	//{{AFX_MSG_MAP(CDrawDoc)
	ON_COMMAND(ID_VIEW_PAPERCOLOR, OnViewPapercolor)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc construction/destruction

CDrawDoc::CDrawDoc()
{
	// Use OLE compound files
	EnableCompoundFile();
	m_nMapMode					=	MM_ANISOTROPIC;
	m_paperColor				=	RGB(255, 255, 255);
	m_nCurrentPage              =   1;
	m_nTotalPage                =   1;
	ComputePageSize();

}

CDrawDoc::~CDrawDoc()
{	
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
		delete m_objects.GetNext(pos);


}

BOOL CDrawDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;
	gUndoManager->RegisterTarget(this);

	return TRUE;
}


BOOL CDrawDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	gUndoManager->RegisterTarget(this);
	if (!COleDocument::OnOpenDocument(lpszPathName)) return false;
	//must save the contents in the grid to last active Document;

	//set the m_workDir
	int len=strlen(lpszPathName);
	CString filename(lpszPathName,len);
	len = filename.ReverseFind('\\');
	m_workDir = filename.Left(len);
	//change the current dir to the m_workDir
	_chdir( m_workDir); 
	SetModifiedFlag(FALSE); //onToolBuild may change it.
	ShowPageNum();
	return true;
}

BOOL CDrawDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	gUndoManager->ClearTarget(this);
	
	return COleDocument::OnSaveDocument(lpszPathName);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawDoc server implementation




/////////////////////////////////////////////////////////////////////////////
// CDrawDoc serialization

void CDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_paperColor;
		m_objects.Serialize(ar);
		ar << m_nTotalPage;
		ar << m_nCurrentPage;
	}
	else
	{
		ar >> m_paperColor;
		m_objects.Serialize(ar);
		ar >> m_nTotalPage;
		ar >> m_nCurrentPage;
	}

	// Calling the base class COleServerDoc enables serialization
	//  of the container document's COleClientItem objects.
	COleDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc diagnostics

#ifdef _DEBUG
void CDrawDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CDrawDoc::Dump(CDumpContext& dc) const
{
	//COleServerDoc::Dump(dc);
	//dumpint objects list
	
	POSITION pos;
	pos= m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_objects.GetPrev(pos);
		pObj->Dump(dc);
	}

}
#endif //_DEBUG

//CDraw operation
void CDrawDoc::Draw(CDC* pDC, CNetworkView* pView)
{
	Draw(pDC,pView,m_nCurrentPage);
}

void CDrawDoc::Draw(CDC* pDC, CNetworkView* pView, UINT nPage)
{
	POSITION pos = m_objects.GetHeadPosition();

	//pDC->TextOut(0,0,"somthing");
	while (pos != NULL)
	{
		CDrawObj* pObj = m_objects.GetNext(pos);
		if (pObj->m_nPage != nPage) continue; //don't draw if is not in the current page
		pObj->Draw(pDC);
		if (pView->m_bActive && !pDC->IsPrinting() && pView->IsSelected(pObj))
			pObj->DrawTracker(pDC, CDrawObj::selected);
	}
}

void CDrawDoc::Add(CDrawObj* pObj)
{
	m_objects.AddTail(pObj);
	pObj->m_pDocument = this;
	pObj->m_nPage = m_nCurrentPage;
	pObj->OnAdded();
}

/*Precondition, pAfter is already in the list.
*/
void CDrawDoc::Add(CDrawObj* pObj, CDrawObj* pAfter)
{
	if (pAfter==NULL){
		Add(pObj);
		return;
	}
	//insert to correct position;
	POSITION pos=m_objects.Find( pAfter);
	m_objects.InsertBefore(pos, pObj);
	pObj->m_pDocument = this;
	pObj->m_nPage = m_nCurrentPage;
	pObj->OnAdded();
}

bool CDrawDoc::Remove(CDrawObj* pObj)
{
	// Find and remove from document
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL){
		m_objects.RemoveAt(pos);
	}

	// call remove for each view so that the view can remove from m_selection
	pos = GetFirstViewPosition();

	//note by tao,not all the views have the Remove function.
	while (pos != NULL){
		CView* temp=GetNextView(pos);
		if(temp->IsKindOf( RUNTIME_CLASS( CNetworkView ) ) )
			((CNetworkView*)temp)->Remove(pObj);
	}
	return true;
}

// point is in logical coordinates
//return the Object at point
CDrawObj* CDrawDoc::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	POSITION pos = m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_objects.GetPrev(pos);
		if (pObj->Intersects(rect) && pObj->m_nPage==m_nCurrentPage)
			return pObj;
	}
	return NULL;
}

void CDrawDoc::ComputePageSize()
{
	CSize new_size(850, 1100);  // 8.5" x 11" default, unit: 0.01 inch

	CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		// GetPrinterDC returns a HDC so attach it
		CDC dc;
		HDC hDC= dlg.CreatePrinterDC();

		LPDEVMODE p = dlg.GetDevMode();

		ASSERT(hDC != NULL);
		dc.Attach(hDC);

		// Get the size of the page in inches
		new_size.cx = MulDiv(dc.GetDeviceCaps(HORZSIZE), 1000, 254);
		new_size.cy = MulDiv(dc.GetDeviceCaps(VERTSIZE), 1000, 254);
	}

	// if size changed then iterate over views and reset
	if (new_size != m_size)
	{
		m_size = new_size;
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL){
			CView* temp=GetNextView(pos);
			if(temp->IsKindOf( RUNTIME_CLASS( CNetworkView ) ) )
				((CNetworkView*)temp)->SetPageSize(m_size);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc commands
void CDrawDoc::OnViewPapercolor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	m_paperColor = dlg.GetColor();
//	SetModifiedFlag();
	UpdateAllViews(NULL);

}


void CDrawDoc::Initialize()
{
	//initialzie all the CDrawReaction's m_pNode[];
	//initialzie all the CDrawNodes m_ReactionList
	//initialize the links to CDrawRegulation;
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_objects.GetNext(pos);
		pObj->Initialize();
	}
}

void CDrawDoc::OnNextPage() 
{
	// TODO: Add your command handler code here
	if (m_nCurrentPage>=m_nTotalPage){
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}
	m_nCurrentPage++;
	ShowPageNum();
	UpdateAllViews(NULL);
}

void CDrawDoc::GoToPage(int nPage)
{
	// TODO: Add your command handler code here
	if (nPage>m_nTotalPage) return;
	m_nCurrentPage=nPage;
	ShowPageNum();
	UpdateAllViews(NULL);

}

void CDrawDoc::OnPagePrevious() 
{
	if (m_nCurrentPage<=1) {
		MessageBeep(MB_ICONEXCLAMATION);
		return;
		}
	m_nCurrentPage--;
	ShowPageNum();
	//	updateview;
	UpdateAllViews(NULL);
}

void CDrawDoc::ShowPageNum()
{
	CMainFrame *pFrame = 
             (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString s;
	s.Format("Page %d/%d",m_nCurrentPage, m_nTotalPage);
	pFrame->m_wndStatusBar.SetPaneText( ID_PAGE, s);
}


void CDrawDoc::OnSetItemRects(LPCRECT lpPosRect, LPCRECT lpClipRect)
{
	// call base class to change the size of the window
//	COleDocument::OnSetItemRects(lpPosRect, lpClipRect);

	// notify first view that scroll info should change
	POSITION pos = GetFirstViewPosition();
	CNetworkView* pView = (CNetworkView*)GetNextView(pos);
	//pView->ResyncScrollSizes();
}


CDrawObj* CDrawDoc::FindObjectAfter(CDrawObj *pObj)
{
	POSITION pos = m_objects.Find(pObj);
	CDrawObj *m_pObjAfterThis = m_objects.GetNext(pos);
	if (!pos)
		m_pObjAfterThis = NULL;
	else
		m_pObjAfterThis=m_objects.GetNext(pos);
	return m_pObjAfterThis;
}

//if return 0, means the object is not in the list;
int GetObjectPosition(const CDrawObjList &list,CDrawObj*pObj){

	int i = 0;
	POSITION pos=list.GetHeadPosition();
	while (pos){
		i++;
		CDrawObj *p=list.GetNext(pos);
		if (p==pObj) return i;
	}
	return 0;
}
