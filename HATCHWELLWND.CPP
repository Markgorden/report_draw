// HatchWellWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Draw.h"
#include "HatchWellWnd.h"
#include "memdc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHatchWellWnd

#define INVALID_HATCH -1
/////////////////////////////////////////////////////////////////////////////
CString CHatchWellWnd::m_hatchName[] = 
{
    "Horizontal",
    "Vertical",
    "ForwardDiagonal",
	"BackwardDiagonal",
	"Cross",
	"???",
	"???",
	"???",
	"???",
	"???",

	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",

	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",

	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",

	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",

	"???",
	"???",
	"???",
	"No"
};

/////////////////////////////////////////////////////////////////////////////
// CHatchWellWnd
CHatchWellWnd::CHatchWellWnd()
{
    Initialise();
	
}

CHatchWellWnd::CHatchWellWnd(CPoint p, int nHatch, CWnd* pParentWnd)
{
    Initialise();
    m_crHatch       = m_crInitialHatch = nHatch;
    m_pParent        = pParentWnd;
    CHatchWellWnd::Create(p, nHatch, pParentWnd);
}

void CHatchWellWnd::Initialise()
{
    m_nNumHatchs       = 53;
 	m_bOffic2000Style   = 0;
    m_nNumColumns       = 0;
    m_nNumRows          = 0;
    m_nBoxSize          = 30;
    m_nMargin           = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrentSel       = INVALID_HATCH;
    m_nChosenHatchSel   = INVALID_HATCH;
    m_pParent           = NULL;
    m_crHatch           = m_crInitialHatch = 0;
	m_foreColor			= Color(255, 0, 0, 255);
	m_backColor         = Color(255, 0, 255,0);
}

CHatchWellWnd::~CHatchWellWnd()
{
}

BOOL CHatchWellWnd::Create(CPoint p, int nHatch, CWnd* pParentWnd)
{
    ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));
    m_pParent  = pParentWnd;
    m_crHatch       = m_crInitialHatch = nHatch;

    // Get the class name and create the window
    CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
                                              0,
                                              (HBRUSH) (COLOR_BTNFACE+1), 
                                              0);

    if (!CWnd::CreateEx(0, szClassName, _T(""), WS_VISIBLE|WS_CHILD| WS_CLIPCHILDREN , 
                        p.x, p.y, 100, 100, // size updated soon
                        pParentWnd->GetSafeHwnd(), 0, NULL))
        return FALSE;


    SetWindowSize();

    // Find which cell (if any) corresponds to the initial hatch
    FindCellFromHatch(nHatch);

    // Capture all mouse events for the life of this window
    SetCapture();

    return TRUE;
}

BEGIN_MESSAGE_MAP(CHatchWellWnd, CWnd)
    //{{AFX_MSG_MAP(CHatchWellWnd)
     ON_WM_NCDESTROY()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHatchWellWnd message handlers

// If an arrow key is pressed, then move the selection
void CHatchWellWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    int row = GetRow(m_nCurrentSel),
        col = GetColumn(m_nCurrentSel);

    if (nChar == VK_DOWN) 
    {
        row++;
        if (GetIndex(row,col) < 0)
        {
			row = col = 0;
        }
		ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_UP) 
    {
        if (row > 0) 
			row--;
        else /* row == 0 */
        {
            row = GetRow(m_nNumHatchs-1); 
            col = GetColumn(m_nNumHatchs-1); 
        }
        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_RIGHT) 
    {
		if (col < m_nNumColumns-1) 
            col++;
        else 
        { 
            col = 0; 
			row++;
        }

        if (GetIndex(row,col) == INVALID_HATCH)
        {
            row = col = 0;
        }

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_LEFT) 
    {
		if (col > 0) col--;
        else /* col == 0 */
        {
            if (row > 0) { row--; col = m_nNumColumns-1; }
            else 
            {
                row = GetRow(m_nNumHatchs-1); 
                col = GetColumn(m_nNumHatchs-1); 
            }
        }
        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_ESCAPE) 
    {
        m_crHatch = m_crInitialHatch;
        EndSelection(HWN_SELENDCANCEL);
        return;
    }

    if (nChar == VK_RETURN || nChar == VK_SPACE)
    {
        EndSelection(HWN_SELENDOK);
        return;
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// auto-deletion
void CHatchWellWnd::OnNcDestroy() 
{
    CWnd::OnNcDestroy();
    delete this;
}


void CHatchWellWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
    int nNewSelection = INVALID_HATCH;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

   // Get the row and column
    nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

    // In range? If not, default and exit
    if (nNewSelection < 0 || nNewSelection >= m_nNumHatchs)
    {
        CWnd::OnMouseMove(nFlags, point);
        return;
    }

    // OK - we have the row and column of the current selection (may be CUSTOM_BOX_VALUE)
    // Has the row/col selection changed? If yes, then redraw old and new cells.
    if (nNewSelection != m_nCurrentSel)
        ChangeSelection(nNewSelection);

    CWnd::OnMouseMove(nFlags, point);
}

// End selection on LButtonUp
void CHatchWellWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{    
	
	CWnd::OnLButtonUp(nFlags, point);

    int nNewSelection = INVALID_HATCH;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;


   // Get the row and column
    nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

    // In range? If not, default and exit
    if (nNewSelection >= 0 && nNewSelection < m_nNumHatchs)
            EndSelection(HWN_SELENDOK);
    	
}

/////////////////////////////////////////////////////////////////////////////
// CHatchWellWnd implementation

int CHatchWellWnd::GetIndex(int row, int col) const
{ 
    if (row < 0 || col < 0 || row >= m_nNumRows || col >= m_nNumColumns)
        return INVALID_HATCH;
    else
    {
        if (row*m_nNumColumns + col >= m_nNumHatchs)
            return INVALID_HATCH;
        else
            return row*m_nNumColumns + col;
    }
}

int CHatchWellWnd::GetRow(int nIndex) const               
{ 
	if (nIndex < 0 || nIndex >= m_nNumHatchs)
        return INVALID_HATCH;
    else
        return nIndex / m_nNumColumns; 
}

int CHatchWellWnd::GetColumn(int nIndex) const            
{ 
	if (nIndex < 0 || nIndex >= m_nNumHatchs)
        return INVALID_HATCH;
    else
        return nIndex % m_nNumColumns; 
}

void CHatchWellWnd::FindCellFromHatch(int crHatch)
{
    for (int i = 0; i < m_nNumHatchs; i++)
    {
        if (i == crHatch)
        {
            m_nChosenHatchSel = i;
            return;
        }
    }

    m_nChosenHatchSel = INVALID_HATCH;
}

// Gets the dimensions of the hatch cell given by (row,col)
BOOL CHatchWellWnd::GetCellRect(int nIndex, const LPRECT& rect)
{

    if (nIndex < 0 || nIndex >= m_nNumHatchs)
        return FALSE;

    rect->left = GetColumn(nIndex) * m_nBoxSize + m_nMargin;
    rect->top  = GetRow(nIndex) * m_nBoxSize + m_nMargin;

   
	rect->right = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;

    return TRUE;
}

// Works out an appropriate size and position of this window
void CHatchWellWnd::SetWindowSize()
{

    // Get the number of columns and rows
    //m_nNumColumns = (int) sqrt((double)m_nNumHatchs);    // for a square window (yuk)
    m_nNumColumns = 9;
    m_nNumRows = m_nNumHatchs / m_nNumColumns;
    if (m_nNumHatchs % m_nNumColumns) m_nNumRows++;

     // Get the current window position, and set the new size
	CRect rect, rect1;
	GetWindowRect(rect);
	GetParent()->GetWindowRect(rect1);
	int x = rect.left - rect1.left;
	int y = rect.top - rect1.top;

    m_WindowRect.SetRect(x,y, 
                         x+m_nNumColumns*m_nBoxSize + 2*m_nMargin,
                          y+ m_nNumRows*m_nBoxSize + 2*m_nMargin);
    MoveWindow(m_WindowRect, FALSE);
}

void CHatchWellWnd::ChangeSelection(int nIndex)
{
    CClientDC dc(this);        // device context for drawing

    if ((m_nCurrentSel >= 0 && m_nCurrentSel < m_nNumHatchs))
    {
        // Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel = m_nCurrentSel;
        m_nCurrentSel = INVALID_HATCH;
        DrawCell(&dc, OldSel);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentSel = nIndex;
    DrawCell(&dc, m_nCurrentSel);

    // Store the current hatch
    m_crHatch = m_nCurrentSel;
    m_pParent->SendMessage(HWN_SELCHANGEBT, (WPARAM) m_crHatch, 0);
}

void CHatchWellWnd::EndSelection(int nMessage)
{
    ReleaseCapture();

    // If custom text selected, perform a custom hatch selection
    if (nMessage == HWN_SELENDCANCEL)
        m_crHatch = m_crInitialHatch;

	m_pParent->SendMessage(nMessage, (WPARAM) m_crHatch, 0);
    
}

void CHatchWellWnd::DrawCell(CDC* pDC, int nIndex)
{


    CRect rect;
    if (!GetCellRect(nIndex, rect)) return;

    // fill background
	if (m_nChosenHatchSel == nIndex && m_nCurrentSel != nIndex)
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));
	else
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
	if (m_nCurrentSel == nIndex) 
		pDC->DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);
	else if (m_nChosenHatchSel == nIndex)
		pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);


    // Draw the cell hatch
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
	Graphics graphics(pDC->m_hDC);

	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	HatchBrush hBrush((HatchStyle)nIndex, m_foreColor,m_backColor);

	graphics.FillRectangle(&hBrush,theRect);

}


void CHatchWellWnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	  CDC  *pdrawDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	  CMemDC memDC(pdrawDC);
	  CDC  *pDC = &memDC;
	
	//CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;

	//rect.InflateRect(1,1);

	pDC->FillSolidRect(rect, RGB(0, 0, 0 ));
	rect.DeflateRect(1,1);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
    // Draw hatch cells
    for (int i = 0; i < m_nNumHatchs; i++)
        DrawCell(pDC, i);


	//CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CHatchWellWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
    SetCapture();
	
}

void CHatchWellWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
    ReleaseCapture();
	
}
