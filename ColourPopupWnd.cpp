// ColourPopupWnd.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
// Extended by Tao Zhu (tzzt@yahoo.com)
// Copyright (c) 2002.


// Updated 30 May 1998 to allow any number of colours, and to
//                     make the appearance closer to Office 97. 
//                     Also added "Default" text area.         (CJM)
//
//         13 June 1998 Fixed change of focus bug (CJM)
//         30 June 1998 Fixed bug caused by focus bug fix (D'oh!!)
//                      Solution suggested by Paul Wilkerson.

// Updated 23 March 2002.   Allowed "No Fill" and "Fill Effect..." Button
//							Office 2000 style drawing.					 						  
//							GDI+ support: Transparency, fill patterns. Check the 
//                          helper class CPatternPage, CGradientPage, CFillPicutrePage


// ColourPopupWnd is a helper class for the colour picker control
// CColourPicker. Check out the header file or the accompanying 
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

#include "stdafx.h"
#include "Draw.h"
#include <math.h>
#include "ColourPicker.h"
#include "ColourPopupWnd.h"
#include "EffectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NOFILL_BOX_VALUE  -5
#define EFFECT_BOX_VALUE  -4
#define DEFAULT_BOX_VALUE -3
#define CUSTOM_BOX_VALUE  -2
#define INVALID_COLOUR    -1

#define MAX_COLOURS       100


ColourTableEntry CColourPopupWnd::m_crColours[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Dark grey")         },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         },
    { RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0xFF, 0x00),    _T("Bright green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};

/////////////////////////////////////////////////////////////////////////////
// CColourPopupWnd

CColourPopupWnd::CColourPopupWnd()
{
    Initialise();
}

CColourPopupWnd::CColourPopupWnd(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                           LPCTSTR szDefaultText /* = NULL */,
                           LPCTSTR szCustomText  /* = NULL */,
						   LPCTSTR szEffectText  /* = NULL */,
						   LPCTSTR szNoText)
{
    Initialise();
    m_crColour       = m_crInitialColour = crColour;
    m_pParent        = pParentWnd;
    m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
    m_strCustomText  = (szCustomText)?  szCustomText  : _T("");
    m_strEffectText  = (szEffectText)?  szEffectText  : _T("");
	m_strNoText		 = (szNoText)?  szNoText  : _T("");


    CColourPopupWnd::Create(p, crColour, pParentWnd, szDefaultText, szCustomText, szEffectText,szNoText);
}

void CColourPopupWnd::Initialise()
{
    m_nNumColours       = sizeof(m_crColours)/sizeof(ColourTableEntry);
    ASSERT(m_nNumColours <= MAX_COLOURS);
    if (m_nNumColours > MAX_COLOURS)
        m_nNumColours = MAX_COLOURS;

 	m_bOffic2000Style   = TRUE;
    m_nNumColumns       = 0;
    m_nNumRows          = 0;
    m_nBoxSize          = 18;
    m_nMargin           = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrentSel       = INVALID_COLOUR;
    m_nChosenColourSel  = INVALID_COLOUR;
    m_pParent           = NULL;
    m_crColour          = m_crInitialColour = RGB(0,0,0);

    // Idiot check: Make sure the colour square is at least 5 x 5;
    if (m_nBoxSize - 2*m_nMargin - 2 < 5) m_nBoxSize = 5 + 2*m_nMargin + 2;

    // Create the font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    m_Font.CreateFontIndirect(&(ncm.lfMessageFont));

    // Create the palette
    struct {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[MAX_COLOURS];
    } pal;

    LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion    = 0x300;
    pLogPalette->palNumEntries = (WORD) m_nNumColours; 

    for (int i = 0; i < m_nNumColours; i++)
    {
        pLogPalette->palPalEntry[i].peRed   = GetRValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peGreen = GetGValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peBlue  = GetBValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peFlags = 0;
    }

    m_Palette.CreatePalette(pLogPalette);
}

CColourPopupWnd::~CColourPopupWnd()
{
    m_Font.DeleteObject();
    m_Palette.DeleteObject();
}

BOOL CColourPopupWnd::Create(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                          LPCTSTR szDefaultText /* = NULL */,
                          LPCTSTR szCustomText  /* = NULL */,
						  LPCTSTR szEffectText  /* = NULL */,
						  LPCTSTR szNoText)
{
    ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));
    ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(CColourPicker)));
    m_pParent  = pParentWnd;
    m_crColour = m_crInitialColour = crColour;

    // Get the class name and create the window
    CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
                                              0,
                                              (HBRUSH) (COLOR_BTNFACE+1), 
                                              0);

    if (!CWnd::CreateEx(0, szClassName, _T(""), WS_VISIBLE|WS_POPUP, 
                        p.x, p.y, 100, 100, // size updated soon
                        pParentWnd->GetSafeHwnd(), 0, NULL))
        return FALSE;

	m_bDestroied = FALSE; //a HWND is attached. add by Tao
    // Store the Custom text
    if (szCustomText != NULL) 
        m_strCustomText = szCustomText;

    // Store the Default Area text
    if (szDefaultText != NULL) 
        m_strDefaultText = szDefaultText;
   
    // Store the Default Area text
    if (szEffectText != NULL) 
        m_strEffectText = szEffectText;
  
	// Set the window size
	if (szNoText!=NULL)
		m_strNoText = szNoText;

    SetWindowSize();

    // Create the tooltips
    CreateToolTips();

    // Find which cell (if any) corresponds to the initial colour
    FindCellFromColour(crColour);

    // Capture all mouse events for the life of this window
    SetCapture();

    return TRUE;
}

BEGIN_MESSAGE_MAP(CColourPopupWnd, CWnd)
    //{{AFX_MSG_MAP(CColourPopupWnd)
    ON_WM_NCDESTROY()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourPopupWnd message handlers

// For tooltips
BOOL CColourPopupWnd::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CWnd::PreTranslateMessage(pMsg);
}

// If an arrow key is pressed, then move the selection
void CColourPopupWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    int row = GetRow(m_nCurrentSel),
        col = GetColumn(m_nCurrentSel);

    if (nChar == VK_DOWN) 
    {
        if (row == DEFAULT_BOX_VALUE) 
            row = col = 0; 
        else if (row == CUSTOM_BOX_VALUE)
        {
            if (m_strDefaultText.GetLength())
                row = col = DEFAULT_BOX_VALUE;
            else
                row = col = 0;
        }
        else
        {
            row++;
            if (GetIndex(row,col) < 0)
            {
                if (m_strCustomText.GetLength())
                    row = col = CUSTOM_BOX_VALUE;
                else if (m_strDefaultText.GetLength())
                    row = col = DEFAULT_BOX_VALUE;
                else
                    row = col = 0;
            }
        }
        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_UP) 
    {
        if (row == DEFAULT_BOX_VALUE)
        {
            if (m_strCustomText.GetLength())
                row = col = CUSTOM_BOX_VALUE;
            else
           { 
                row = GetRow(m_nNumColours-1); 
                col = GetColumn(m_nNumColours-1); 
            }
        }
        else if (row == CUSTOM_BOX_VALUE)
        { 
            row = GetRow(m_nNumColours-1); 
            col = GetColumn(m_nNumColours-1); 
        }
        else if (row > 0) row--;
        else /* row == 0 */
        {
            if (m_strDefaultText.GetLength())
                row = col = DEFAULT_BOX_VALUE;
            else if (m_strCustomText.GetLength())
                row = col = CUSTOM_BOX_VALUE;
            else
            { 
                row = GetRow(m_nNumColours-1); 
                col = GetColumn(m_nNumColours-1); 
            }
        }
        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_RIGHT) 
    {
        if (row == DEFAULT_BOX_VALUE) 
            row = col = 0; 
        else if (row == CUSTOM_BOX_VALUE)
        {
            if (m_strDefaultText.GetLength())
                row = col = DEFAULT_BOX_VALUE;
            else
                row = col = 0;
        }
        else if (col < m_nNumColumns-1) 
            col++;
        else 
        { 
            col = 0; row++;
        }

        if (GetIndex(row,col) == INVALID_COLOUR)
        {
            if (m_strCustomText.GetLength())
                row = col = CUSTOM_BOX_VALUE;
            else if (m_strDefaultText.GetLength())
                row = col = DEFAULT_BOX_VALUE;
            else
                row = col = 0;
        }

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_LEFT) 
    {
        if (row == DEFAULT_BOX_VALUE)
        {
            if (m_strCustomText.GetLength())
                row = col = CUSTOM_BOX_VALUE;
            else
           { 
                row = GetRow(m_nNumColours-1); 
                col = GetColumn(m_nNumColours-1); 
            }
        }
        else if (row == CUSTOM_BOX_VALUE)
        { 
            row = GetRow(m_nNumColours-1); 
            col = GetColumn(m_nNumColours-1); 
        }
        else if (col > 0) col--;
        else /* col == 0 */
        {
            if (row > 0) { row--; col = m_nNumColumns-1; }
            else 
            {
                if (m_strDefaultText.GetLength())
                    row = col = DEFAULT_BOX_VALUE;
                else if (m_strCustomText.GetLength())
                    row = col = CUSTOM_BOX_VALUE;
                else
                { 
                    row = GetRow(m_nNumColours-1); 
                    col = GetColumn(m_nNumColours-1); 
                }
            }
        }
        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_ESCAPE) 
    {
        m_crColour = m_crInitialColour;
        EndSelection(CPN_SELENDCANCEL);
        return;
    }

    if (nChar == VK_RETURN || nChar == VK_SPACE)
    {
        EndSelection(CPN_SELENDOK);
        return;
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// auto-deletion
void CColourPopupWnd::OnNcDestroy() 
{
    CWnd::OnNcDestroy();
    delete this;
}

void CColourPopupWnd::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	if (m_bOffic2000Style){
		dc.FillSolidRect(rect,RGB(50,50,50));
		rect.DeflateRect(1,1);
		dc.FillSolidRect(rect,RGB(255,255,255));
	}
	else
		dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);

    // Draw the No fil Area text
    if (m_strNoText.GetLength())
        DrawCell(&dc, NOFILL_BOX_VALUE);

    // Draw the Default Area text
    if (m_strDefaultText.GetLength())
        DrawCell(&dc, DEFAULT_BOX_VALUE);
 
    // Draw colour cells
    for (int i = 0; i < m_nNumColours; i++)
        DrawCell(&dc, i);
    
     // Draw effect text
    if (m_strCustomText.GetLength())
        DrawCell(&dc, EFFECT_BOX_VALUE);
 
	// Draw custom text
    if (m_strCustomText.GetLength())
        DrawCell(&dc, CUSTOM_BOX_VALUE);


}

void CColourPopupWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
    int nNewSelection = INVALID_COLOUR;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

    // First check we aren't in text box
    if (m_strNoText.GetLength() && m_NoTextRect.PtInRect(point))
        nNewSelection = NOFILL_BOX_VALUE;
    else if (m_strCustomText.GetLength() && m_CustomTextRect.PtInRect(point))
        nNewSelection = CUSTOM_BOX_VALUE;
    else if (m_strDefaultText.GetLength() && m_DefaultTextRect.PtInRect(point))
        nNewSelection = DEFAULT_BOX_VALUE;
	else if (m_strEffectText.GetLength() && m_EffectTextRect.PtInRect(point))
        nNewSelection = EFFECT_BOX_VALUE;
    else
    {
        // Take into account text box
        if (m_strDefaultText.GetLength()) 
            point.y -= m_DefaultTextRect.Height()+2*m_nMargin;  

         if (m_strNoText.GetLength()) 
            point.y -= m_NoTextRect.Height()+m_nMargin;  
       // Get the row and column
        nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

        // In range? If not, default and exit
        if (nNewSelection < 0 || nNewSelection >= m_nNumColours)
        {
            CWnd::OnMouseMove(nFlags, point);
            return;
        }
    }

    // OK - we have the row and column of the current selection (may be CUSTOM_BOX_VALUE)
    // Has the row/col selection changed? If yes, then redraw old and new cells.
    if (nNewSelection != m_nCurrentSel)
        ChangeSelection(nNewSelection);

    CWnd::OnMouseMove(nFlags, point);
}

// End selection on LButtonUp
void CColourPopupWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{    
    if(m_bDestroied)   //bug fixed by Tao
		return;
	
	CWnd::OnLButtonUp(nFlags, point);

    DWORD pos = GetMessagePos();
    point = CPoint(LOWORD(pos), HIWORD(pos));

	m_bDestroied = TRUE;

    if (m_WindowRect.PtInRect(point))
        EndSelection(CPN_SELENDOK);
    else
        EndSelection(CPN_SELENDCANCEL);
	
}

/////////////////////////////////////////////////////////////////////////////
// CColourPopupWnd implementation

int CColourPopupWnd::GetIndex(int row, int col) const
{ 
    if ((row == CUSTOM_BOX_VALUE || col == CUSTOM_BOX_VALUE) && m_strCustomText.GetLength())
        return CUSTOM_BOX_VALUE;
    else if ((row == DEFAULT_BOX_VALUE || col == DEFAULT_BOX_VALUE) && m_strDefaultText.GetLength())
        return DEFAULT_BOX_VALUE;
	else if ((row == EFFECT_BOX_VALUE || col == EFFECT_BOX_VALUE) && m_strEffectText.GetLength())
		return EFFECT_BOX_VALUE;
	else if ((row == NOFILL_BOX_VALUE || col == NOFILL_BOX_VALUE) && m_strNoText.GetLength())
		return NOFILL_BOX_VALUE;
    else if (row < 0 || col < 0 || row >= m_nNumRows || col >= m_nNumColumns)
        return INVALID_COLOUR;
    else
    {
        if (row*m_nNumColumns + col >= m_nNumColours)
            return INVALID_COLOUR;
        else
            return row*m_nNumColumns + col;
    }
}

int CColourPopupWnd::GetRow(int nIndex) const               
{ 
    if (nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength())
        return CUSTOM_BOX_VALUE;
    else if (nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength())
        return DEFAULT_BOX_VALUE;
    else if (nIndex == EFFECT_BOX_VALUE && m_strEffectText.GetLength())
        return EFFECT_BOX_VALUE;
    else if (nIndex == NOFILL_BOX_VALUE && m_strNoText.GetLength())
        return NOFILL_BOX_VALUE;
    else if (nIndex < 0 || nIndex >= m_nNumColours)
        return INVALID_COLOUR;
    else
        return nIndex / m_nNumColumns; 
}

int CColourPopupWnd::GetColumn(int nIndex) const            
{ 
    if (nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength())
        return CUSTOM_BOX_VALUE;
    else if (nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength())
        return DEFAULT_BOX_VALUE;
    else if (nIndex == EFFECT_BOX_VALUE && m_strEffectText.GetLength())
        return EFFECT_BOX_VALUE;
    else if (nIndex == NOFILL_BOX_VALUE && m_strNoText.GetLength())
        return NOFILL_BOX_VALUE;
    else if (nIndex < 0 || nIndex >= m_nNumColours)
        return INVALID_COLOUR;
    else
        return nIndex % m_nNumColumns; 
}

void CColourPopupWnd::FindCellFromColour(COLORREF crColour)
{
    if (crColour == CLR_DEFAULT && m_strDefaultText.GetLength())
    {
        m_nChosenColourSel = DEFAULT_BOX_VALUE;
        return;
    }

    for (int i = 0; i < m_nNumColours; i++)
    {
        if (GetColour(i) == crColour)
        {
            m_nChosenColourSel = i;
            return;
        }
    }

    if (m_strCustomText.GetLength())
        m_nChosenColourSel = CUSTOM_BOX_VALUE;
    else
        m_nChosenColourSel = INVALID_COLOUR;
}

// Gets the dimensions of the colour cell given by (row,col)
BOOL CColourPopupWnd::GetCellRect(int nIndex, const LPRECT& rect)
{
    if (nIndex == CUSTOM_BOX_VALUE)
    {
        ::SetRect(rect, 
                  m_CustomTextRect.left,  m_CustomTextRect.top,
                  m_CustomTextRect.right, m_CustomTextRect.bottom);
        return TRUE;
    }
    else if (nIndex == DEFAULT_BOX_VALUE)
    {
        ::SetRect(rect, 
                  m_DefaultTextRect.left,  m_DefaultTextRect.top,
                  m_DefaultTextRect.right, m_DefaultTextRect.bottom);
        return TRUE;
    }
    else if (nIndex == EFFECT_BOX_VALUE)
    {
        ::SetRect(rect, 
                  m_EffectTextRect.left,  m_EffectTextRect.top,
                  m_EffectTextRect.right, m_EffectTextRect.bottom);
        return TRUE;
    }
    else if (nIndex == NOFILL_BOX_VALUE)
    {
        ::SetRect(rect, 
                  m_NoTextRect.left,  m_NoTextRect.top,
                  m_NoTextRect.right, m_NoTextRect.bottom);
        return TRUE;
    }
    if (nIndex < 0 || nIndex >= m_nNumColours)
        return FALSE;

    rect->left = GetColumn(nIndex) * m_nBoxSize + m_nMargin;
    rect->top  = GetRow(nIndex) * m_nBoxSize + m_nMargin;

    // Move everything down if we are displaying a default text area
    if (m_strDefaultText.GetLength()) 
        rect->top += (2*m_nMargin + m_DefaultTextRect.Height());

    // Move everything down if we are displaying a nofill text area
    if (m_strNoText.GetLength()) 
        rect->top += (m_nMargin + m_NoTextRect.Height());
   
	rect->right = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;

    return TRUE;
}

// Works out an appropriate size and position of this window
void CColourPopupWnd::SetWindowSize()
{
    CSize TextSize;
    // If we are showing a custom or default text area, get the font and text size.
    if (m_strCustomText.GetLength() || m_strDefaultText.GetLength()|| 
		m_strNoText.GetLength() || m_strEffectText.GetLength())
    {
        CClientDC dc(this);
        CFont* pOldFont = (CFont*) dc.SelectObject(&m_Font);

        // Get the size of the custom text (if there IS custom text)
        TextSize = CSize(0,0);
        if (m_strCustomText.GetLength())
            TextSize = dc.GetTextExtent(m_strCustomText);

        // Get the size of the default text (if there IS default text)
        if (m_strDefaultText.GetLength())
        {
            CSize DefaultSize = dc.GetTextExtent(m_strDefaultText);
            if (DefaultSize.cx > TextSize.cx) TextSize.cx = DefaultSize.cx;
            if (DefaultSize.cy > TextSize.cy) TextSize.cy = DefaultSize.cy;
        }
        if (m_strNoText.GetLength())
        {
            CSize DefaultSize = dc.GetTextExtent(m_strNoText);
            if (DefaultSize.cx > TextSize.cx) TextSize.cx = DefaultSize.cx;
            if (DefaultSize.cy > TextSize.cy) TextSize.cy = DefaultSize.cy;
        }
        if (m_strEffectText.GetLength())
        {
            CSize DefaultSize = dc.GetTextExtent(m_strEffectText);
            if (DefaultSize.cx > TextSize.cx) TextSize.cx = DefaultSize.cx;
            if (DefaultSize.cy > TextSize.cy) TextSize.cy = DefaultSize.cy;
        }

        dc.SelectObject(pOldFont);
        TextSize += CSize(2*m_nMargin,2*m_nMargin);

        // Add even more space to draw the horizontal line
        TextSize.cy +=2*m_nMargin; 
    }

    // Get the number of columns and rows
    //m_nNumColumns = (int) sqrt((double)m_nNumColours);    // for a square window (yuk)
    m_nNumColumns = 8;
    m_nNumRows = m_nNumColours / m_nNumColumns;
    if (m_nNumColours % m_nNumColumns) m_nNumRows++;

     // Get the current window position, and set the new size
    CRect rect;
    GetWindowRect(rect); 
    m_WindowRect.SetRect(rect.left, rect.top, 
                         rect.left + m_nNumColumns*m_nBoxSize + 2*m_nMargin,
                         rect.top  + m_nNumRows*m_nBoxSize + 2*m_nMargin);

	//check if "No fill" or "no line" button exist, add the size the rect
    if (m_strNoText.GetLength()) 
    {
        if (TextSize.cx > rect.Width())
            rect.right = rect.left + TextSize.cx;
        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

        // Work out the text area
        m_NoTextRect.SetRect(		m_nMargin,
									m_nMargin, 
									m_nMargin+TextSize.cx, 
									2*m_nMargin+TextSize.cy);

        m_WindowRect.bottom += m_NoTextRect.Height() + 2*m_nMargin;
    }

    // if default text, then expand window if necessary, and set text width as
    // window width

    if (m_strDefaultText.GetLength()) 
    {
        if (TextSize.cx > m_WindowRect.Width())
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

        // Work out the text area
        m_DefaultTextRect.SetRect(  m_nMargin,
									m_NoTextRect.Height() + 2*m_nMargin, 
									m_nMargin+TextSize.cx, 
									m_NoTextRect.Height()  + 3*m_nMargin + TextSize.cy);

        m_WindowRect.bottom += m_DefaultTextRect.Height() + 3*m_nMargin;
    }


    // if custom text, then expand window if necessary, and set text width as
    // window width
    if (m_strCustomText.GetLength()) 
    {
        if (TextSize.cx > m_WindowRect.Width())
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

        // Work out the text area
        m_CustomTextRect.SetRect(   m_nMargin, 
									m_WindowRect.Height(), 
									m_nMargin+TextSize.cx, 
									m_WindowRect.Height()+m_nMargin+TextSize.cy);

        m_WindowRect.bottom += m_CustomTextRect.Height() + 2*m_nMargin;
   }

    if (m_strEffectText.GetLength()) 
    {
        if (TextSize.cx > m_WindowRect.Width())
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

        // Work out the text area
        m_EffectTextRect.SetRect(m_nMargin, 
								 m_WindowRect.Height(), 
                                 m_nMargin+TextSize.cx, 
                                 m_WindowRect.Height()+m_nMargin+TextSize.cy);
        m_WindowRect.bottom += m_EffectTextRect.Height() + 2*m_nMargin;
    }
   // Need to check it'll fit on screen: Too far right?
    CSize ScreenSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
    if (m_WindowRect.right > ScreenSize.cx)
        m_WindowRect.OffsetRect(-(m_WindowRect.right - ScreenSize.cx), 0);

    // Too far left?
    if (m_WindowRect.left < 0)
        m_WindowRect.OffsetRect( -m_WindowRect.left, 0);

    // Bottom falling out of screen?
    if (m_WindowRect.bottom > ScreenSize.cy)
    {
        CRect ParentRect;
        m_pParent->GetWindowRect(ParentRect);
        m_WindowRect.OffsetRect(0, -(ParentRect.Height() + m_WindowRect.Height()));
    }

    // Set the window size and position
    MoveWindow(m_WindowRect, TRUE);
}

void CColourPopupWnd::CreateToolTips()
{
    // Create the tool tip
    if (!m_ToolTip.Create(this)) return;

    // Add a tool for each cell
    for (int i = 0; i < m_nNumColours; i++)
    {
        CRect rect;
        if (!GetCellRect(i, rect)) continue;
            m_ToolTip.AddTool(this, GetColourName(i), rect, 1);
    }
}

void CColourPopupWnd::ChangeSelection(int nIndex)
{
    CClientDC dc(this);        // device context for drawing

    if (nIndex > m_nNumColours)
        nIndex = CUSTOM_BOX_VALUE; 

    if ((m_nCurrentSel >= 0 && m_nCurrentSel < m_nNumColours) ||
        m_nCurrentSel == CUSTOM_BOX_VALUE || m_nCurrentSel == DEFAULT_BOX_VALUE ||
		m_nCurrentSel == EFFECT_BOX_VALUE || m_nCurrentSel == NOFILL_BOX_VALUE )
    {
        // Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel = m_nCurrentSel;
        m_nCurrentSel = INVALID_COLOUR;
        DrawCell(&dc, OldSel);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentSel = nIndex;
    DrawCell(&dc, m_nCurrentSel);

    // Store the current colour
    if (m_nCurrentSel == CUSTOM_BOX_VALUE)
        m_pParent->SendMessage(CPN_SELCHANGEBT, (WPARAM) m_crInitialColour, 0);
    else if (m_nCurrentSel == DEFAULT_BOX_VALUE)
    {
        m_crColour = CLR_DEFAULT;
        m_pParent->SendMessage(CPN_SELCHANGEBT, (WPARAM) CLR_DEFAULT, 0);
    }
	else if (m_nCurrentSel == EFFECT_BOX_VALUE)
	{
 //SOME INFOMATION ABOUT PATTEN HAS TO PASS TO PARENT
	}
    else if (m_nCurrentSel == NOFILL_BOX_VALUE){
	}
	else
    {
        m_crColour = GetColour(m_nCurrentSel);
        m_pParent->SendMessage(CPN_SELCHANGEBT, (WPARAM) m_crColour, 0);
    }
}

void CColourPopupWnd::EndSelection(int nMessage)
{
    ReleaseCapture();

    // If custom text selected, perform a custom colour selection
    if (nMessage != CPN_SELENDCANCEL && m_nCurrentSel == CUSTOM_BOX_VALUE)
    {
        CColorDialog dlg(m_crInitialColour, CC_FULLOPEN | CC_ANYCOLOR, this);

        if (dlg.DoModal() == IDOK)
            m_crColour = dlg.GetColor();
        else
            nMessage = CPN_SELENDCANCEL;
		if (m_pParent->IsKindOf(RUNTIME_CLASS(CColourPicker))) {
			CColourPicker* picker =(CColourPicker* )m_pParent;
			picker->m_BrushType = BrushTypeSolidColor;
			picker->m_noColor = FALSE;
		}

    } 
    if (nMessage != CPN_SELENDCANCEL && m_nCurrentSel == EFFECT_BOX_VALUE)
    {
        CEffectDlg dlg(this);
		
 		if (m_pParent->IsKindOf(RUNTIME_CLASS(CColourPicker))){
			CColourPicker* picker =(CColourPicker* )m_pParent;
			dlg.m_effectpage = picker->m_effectpage;
			dlg.m_BrushType = picker->m_BrushType;
			dlg.m_StartColor = picker->m_StartColor;
			dlg.m_EndColor = picker->m_EndColor;
			dlg.m_fileName = picker->m_fileName;
			dlg.m_nColor = 2;
		    dlg.m_linearFillMode = 	picker->m_linearFillMode;
			if (dlg.m_BrushType == BrushTypePathGradient )
				dlg.m_nStyle = 4+picker->m_bToCenter; //4, or 5
			else if (dlg.m_BrushType == BrushTypeLinearGradient )
				dlg.m_nStyle = (int)picker->m_linearFillMode;
			dlg.m_nStretch = picker->m_nStretch;
			dlg.m_hatchStyle = picker->m_hatchStyle ;
			dlg.m_wrapMode =picker->m_wrapMode ;
			
		}
        if (dlg.DoModal() == IDOK){
            //My code directly access the member variable of CColourPicker, too lasy to send a message;
			//This is not a good implementation. but it works fine.
			if (m_pParent->IsKindOf(RUNTIME_CLASS(CColourPicker))){
				CColourPicker* picker =(CColourPicker* )m_pParent;

				picker->m_BrushType = dlg.m_BrushType;
				picker->m_StartColor = dlg.m_StartColor;
				picker->m_EndColor = dlg.m_EndColor;
				picker->m_fileName = dlg.m_fileName;
				picker->m_linearFillMode = dlg.m_linearFillMode;
				picker->m_bToCenter = dlg.m_nStyle-4; //0, 1;
				if (picker->m_bToCenter<0)
					picker->m_bToCenter = 0;
				picker->m_hatchStyle = dlg.m_hatchStyle;
				picker->m_wrapMode = dlg.m_wrapMode;
				picker->m_nStretch = dlg.m_nStretch;
				picker->m_noColor = FALSE;
			}

		}
        else
            nMessage = CPN_SELENDCANCEL;
    } 
    if (nMessage != CPN_SELENDCANCEL && m_nCurrentSel == NOFILL_BOX_VALUE){
		if (m_pParent->IsKindOf(RUNTIME_CLASS(CColourPicker))){
				CColourPicker* picker =(CColourPicker* )m_pParent;
				picker->m_noColor = TRUE;
		}
	}
	else if (nMessage != CPN_SELENDCANCEL && m_nCurrentSel != EFFECT_BOX_VALUE){
		if (m_pParent->IsKindOf(RUNTIME_CLASS(CColourPicker))){
			CColourPicker* picker =(CColourPicker* )m_pParent;
			picker->m_BrushType = BrushTypeSolidColor;
			picker->m_noColor = FALSE;
		}
	}

    if (nMessage == CPN_SELENDCANCEL)
        m_crColour = m_crInitialColour;

	m_pParent->SendMessage(nMessage, (WPARAM) m_crColour, 0);
    
    DestroyWindow();
}

void CColourPopupWnd::DrawCell(CDC* pDC, int nIndex)
{

	if (m_bOffic2000Style){
		DrawCell2000(pDC,nIndex);
		return;
	}

    // For the No Line or No Fill Text area
    if (m_strNoText.GetLength() && nIndex == NOFILL_BOX_VALUE){
        // Draw button
		CRect TextButtonRect = m_NoTextRect;
		TextButtonRect.top += 2*m_nMargin;
          // Fill background
		pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

        TextButtonRect.DeflateRect(1,1);

        // fill background
		if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
			pDC->FillSolidRect(TextButtonRect, RGB(200,200,200));
		else
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
		if (m_nCurrentSel == nIndex) 
			pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		else if (m_nChosenColourSel == nIndex)
			pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
    // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strNoText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
		return;
	}
   if (m_strEffectText.GetLength() && nIndex == EFFECT_BOX_VALUE){
        // Draw button
		CRect TextButtonRect = m_EffectTextRect;
		TextButtonRect.top += 2*m_nMargin;
          // Fill background
		pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

        // Draw horizontal line
        pDC->FillSolidRect(m_EffectTextRect.left+2*m_nMargin, m_EffectTextRect.top,
                           m_EffectTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_EffectTextRect.left+2*m_nMargin, m_EffectTextRect.top+1,
                           m_EffectTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        TextButtonRect.DeflateRect(1,1);

        // fill background

		if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
		else
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
		if (m_nCurrentSel == nIndex) 
			pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		else if (m_nChosenColourSel == nIndex)
			pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strEffectText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
		return;
	}

    if (m_strCustomText.GetLength() && nIndex == CUSTOM_BOX_VALUE)
    {
        // The extent of the actual text button
        CRect TextButtonRect = m_CustomTextRect;
        TextButtonRect.top += 2*m_nMargin;

        // Fill background
        pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

        // Draw horizontal line
        pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top,
                           m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top+1,
                           m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        TextButtonRect.DeflateRect(1,1);

        // fill background
		if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
		else
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
		if (m_nCurrentSel == nIndex) 
			pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		else if (m_nChosenColourSel == nIndex)
			pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);

        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strCustomText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    // For the Default Text area
    if (m_strDefaultText.GetLength() && nIndex == DEFAULT_BOX_VALUE)
    {
        // Fill background
       if(!m_bOffic2000Style)
	      pDC->FillSolidRect(m_DefaultTextRect, ::GetSysColor(COLOR_3DFACE));

        // The extent of the actual text button
        CRect TextButtonRect = m_DefaultTextRect;
        TextButtonRect.DeflateRect(1,1);

        // Draw horizontal line
        pDC->FillSolidRect(m_DefaultTextRect.left+2*m_nMargin, m_DefaultTextRect.top-1,
                           m_DefaultTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_DefaultTextRect.left+2*m_nMargin, m_DefaultTextRect.top,
                          m_DefaultTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

       // fill background
		if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
		else
			pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
      // Draw thin line around text
		CRect LineRect = TextButtonRect;
		LineRect.DeflateRect(2*m_nMargin,2*m_nMargin);
		CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(LineRect);
		pDC->SelectObject(pOldPen);
		// Draw button
		if (m_nCurrentSel == nIndex) 
			pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		else if (m_nChosenColourSel == nIndex)
			pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strDefaultText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    CRect rect;
    if (!GetCellRect(nIndex, rect)) return;

    // Select and realize the palette
    CPalette* pOldPalette = NULL;
    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
        pDC->RealizePalette();
    }

    // fill background
	if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));
	else
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
	if (m_nCurrentSel == nIndex) 
		pDC->DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);
	else if (m_nChosenColourSel == nIndex)
		pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);

    CBrush brush(PALETTERGB(GetRValue(GetColour(nIndex)), 
                            GetGValue(GetColour(nIndex)), 
                            GetBValue(GetColour(nIndex)) ));
    CPen   pen;
    pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    // Draw the cell colour
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
    pDC->Rectangle(rect);

    // restore DC and cleanup
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    brush.DeleteObject();
    pen.DeleteObject();

    if (pOldPalette && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
        pDC->SelectPalette(pOldPalette, FALSE);
}

BOOL CColourPopupWnd::OnQueryNewPalette() 
{
    Invalidate();    
    return CWnd::OnQueryNewPalette();
}

void CColourPopupWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
    CWnd::OnPaletteChanged(pFocusWnd);

    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
        Invalidate();
}

void CColourPopupWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

    ReleaseCapture();
    //DestroyWindow(); - causes crash when Custom colour dialog appears.
}

// KillFocus problem fix suggested by Paul Wilkerson.
// Another bug caused by this, when custom colur dialog is open, activate
// other app, it will crash.
void CColourPopupWnd::OnActivateApp(BOOL bActive, DWORD hTask) 
{
	CWnd::OnActivateApp(bActive, (DWORD)hTask);

    if(m_bDestroied)   //bug fixed by Tao
		return;

	// If Deactivating App, cancel this selection
	if (!bActive)
		 EndSelection(CPN_SELENDCANCEL);
}

void CColourPopupWnd::DrawCell2000(CDC *pDC, int nIndex)
{

   if (m_strNoText.GetLength() && nIndex == NOFILL_BOX_VALUE){
        // Draw button
		CRect TextButtonRect = m_NoTextRect;
		TextButtonRect.top += 2*m_nMargin;
        TextButtonRect.DeflateRect(1,1);

        // fill background
		pDC->FillSolidRect(TextButtonRect, RGB(255,255,255));
   // Draw button
		if (m_nCurrentSel == nIndex) {
			pDC->FillSolidRect(TextButtonRect, RGB(50,50,50));
			CRect rect = TextButtonRect;
			rect.DeflateRect(1,1);
			pDC->FillSolidRect(rect, RGB(180,180,220));
		}
			//else if (m_nChosenColourSel == nIndex)
			//	pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strNoText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
		return;
	}
   if (m_strEffectText.GetLength() && nIndex == EFFECT_BOX_VALUE){
        // Draw button
		CRect TextButtonRect = m_EffectTextRect;
		TextButtonRect.top += 2*m_nMargin;
          // Fill background
        // Draw horizontal line
        pDC->FillSolidRect(m_EffectTextRect.left+2*m_nMargin, m_EffectTextRect.top,
                           m_EffectTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_EffectTextRect.left+2*m_nMargin, m_EffectTextRect.top+1,
                           m_EffectTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        TextButtonRect.DeflateRect(1,1);

        // fill background

		pDC->FillSolidRect(TextButtonRect, RGB(255,255,255));
    // Draw button
		if (m_nCurrentSel == nIndex) {
			pDC->FillSolidRect(TextButtonRect, RGB(50,50,50));
			CRect rect = TextButtonRect;
			rect.DeflateRect(1,1);
			pDC->FillSolidRect(rect, RGB(180,180,220));
		}
		//	else if (m_nChosenColourSel == nIndex)
			//	pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
       // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strEffectText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
		return;
	}

    if (m_strCustomText.GetLength() && nIndex == CUSTOM_BOX_VALUE)
    {
        // The extent of the actual text button
        CRect TextButtonRect = m_CustomTextRect;
        TextButtonRect.top += 2*m_nMargin;

        // Draw horizontal line
        pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top,
                           m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top+1,
                           m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        TextButtonRect.DeflateRect(1,1);

        // fill background
		pDC->FillSolidRect(TextButtonRect, RGB(255,255,255));
	// Draw button
		if (m_nCurrentSel == nIndex) {
			pDC->FillSolidRect(TextButtonRect, RGB(50,50,50));
			CRect rect = TextButtonRect;
			rect.DeflateRect(1,1);
			pDC->FillSolidRect(rect, RGB(180,180,220));
		}
		//	else if (m_nChosenColourSel == nIndex)
		//		pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strCustomText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    // For the Default Text area
    if (m_strDefaultText.GetLength() && nIndex == DEFAULT_BOX_VALUE)
    {
        // The extent of the actual text button
        CRect TextButtonRect = m_DefaultTextRect;
        TextButtonRect.DeflateRect(1,1);

        // Draw horizontal line
        pDC->FillSolidRect(m_DefaultTextRect.left+2*m_nMargin, m_DefaultTextRect.top-1,
                           m_DefaultTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_DefaultTextRect.left+2*m_nMargin, m_DefaultTextRect.top,
                          m_DefaultTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

       // fill background
		pDC->FillSolidRect(TextButtonRect, RGB(255,255,255));

		CRect rect = TextButtonRect;
		rect.DeflateRect(0,1);
		pDC->FillSolidRect(rect, RGB(50,50,50));
		rect.DeflateRect(1,1);
		pDC->FillSolidRect(rect, RGB(255,255,255));
		if (m_nCurrentSel == nIndex) 
			pDC->FillSolidRect(rect, RGB(180,180,220));
		//else if (m_nChosenColourSel == nIndex)
		//	pDC->FillSolidRect(rect, RGB(255,255,255));
      // Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strDefaultText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    CRect rect;
    if (!GetCellRect(nIndex, rect)) return;

    // Select and realize the palette
    CPalette* pOldPalette = NULL;
    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
        pDC->RealizePalette();
    }

    // fill background
	pDC->FillSolidRect(rect, RGB(255,255,255));
	if (m_nCurrentSel == nIndex) {
		pDC->FillSolidRect(rect, RGB(50,50,50));
		CRect rect1 = rect;
		rect1.DeflateRect(1,1);
		pDC->FillSolidRect(rect1, RGB(180,180,220));
	}
	else if (m_nChosenColourSel == nIndex){
		pDC->FillSolidRect(rect, RGB(50,50,50));
		CRect rect1 = rect;
		rect1.DeflateRect(1,1);
		pDC->FillSolidRect(rect1, RGB(200,200,240));
	}

    CBrush brush(PALETTERGB(GetRValue(GetColour(nIndex)), 
                            GetGValue(GetColour(nIndex)), 
                            GetBValue(GetColour(nIndex)) ));
    CPen   pen;
    pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    // Draw the cell colour
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
    pDC->Rectangle(rect);

    // restore DC and cleanup
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    brush.DeleteObject();
    pen.DeleteObject();

    if (pOldPalette && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
        pDC->SelectPalette(pOldPalette, FALSE);

}