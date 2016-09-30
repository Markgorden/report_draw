// TextFormatPage.cpp : implementation file
//

#include "stdafx.h"
#include "draw.h"
#include "TextFormatPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextFormatPage property page

IMPLEMENT_DYNCREATE(CTextFormatPage, CPropertyPage)

CTextFormatPage::CTextFormatPage() : CPropertyPage(CTextFormatPage::IDD)
{
	//{{AFX_DATA_INIT(CTextFormatPage)
	m_borderType = -1;
	m_allignment = -1;
	m_penSize = 0;
	m_Fill = -1;
	m_round = FALSE;
	m_Transparency = 0;
	//}}AFX_DATA_INIT
	m_FillPicker.m_effectpage = 0x7;
	m_LinePicker.m_effectpage = 0x1;
}

CTextFormatPage::~CTextFormatPage()
{
}

void CTextFormatPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextFormatPage)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_transCtrl);
	DDX_Control(pDX, IDC_LINEPICKER, m_linePicker);
	DDX_Control(pDX, IDC_LINE_COLOR, m_LinePicker);
	DDX_Control(pDX, IDC_FILL_COLOR, m_FillPicker);
	DDX_CBIndex(pDX, IDC_BORDER, m_borderType);
	DDX_CBIndex(pDX, IDC_TEXT_ALLIGNMENT, m_allignment);
	DDX_Text(pDX, IDC_LINE_SIZE, m_penSize);
	DDX_Radio(pDX, IDC_NOFILL, m_Fill);
	DDX_Check(pDX, IDC_ROUND_PEN, m_round);
	DDX_Text(pDX, IDC_EDIT1, m_Transparency);
	DDV_MinMaxUInt(pDX, m_Transparency, 0, 100);
	//}}AFX_DATA_MAP
	DDX_ColourPicker(pDX, IDC_FILL_COLOR, m_FillColor);
	DDX_ColourPicker(pDX, IDC_LINE_COLOR, m_LineColor);
	DDX_LinePicker(pDX, IDC_LINEPICKER, (int&)m_dashStyle);
}


BEGIN_MESSAGE_MAP(CTextFormatPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTextFormatPage)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextFormatPage message handlers

BOOL CTextFormatPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CSpinButtonCtrl *p;
	p=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	p->SetRange(0,100);

	m_linePicker.SetMaximumLines(5);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextFormatPage::SetValues(CDrawText * p)
{
	m_allignment = p->m_allignment;
	m_borderType = p->m_nBorderType;
	//if it is a arrow;
	m_dashStyle =  p->m_dashStyle;
//	m_dashCap = p->m_dashCap ;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
    m_FillPicker.m_BrushType = p->m_brushType;
	m_LinePicker.m_BrushType = p->m_penbrushType;
	//m_penType ;
	m_FillPicker.m_StartColor = p->m_brushColor; //when hatch pattern used, this is foreColor
    m_FillPicker.m_EndColor = p->m_gradientColor;
	m_LinePicker.m_StartColor = p->m_penColor;
	m_LinePicker.m_EndColor = p->m_penEndColor;
	m_penSize = p->m_penWidth;
	m_FillPicker.m_linearFillMode = p->m_linearGradientMode;
	m_LinePicker.m_linearFillMode = p->m_penlinearGradientMode;
 	m_Transparency = p->m_Transparency;

	m_FillPicker.m_hatchStyle = p->m_hatchStyle;
	m_FillPicker.m_fileName =p->m_fillImage;
	m_FillPicker.m_wrapMode = p->m_wrapMode;
	m_FillPicker.m_nStretch = p->m_nStretch; //0, - center path gradient.
	m_FillPicker.m_bToCenter = p->m_bToCenter;
	if (!(p->m_state&0x01))
		m_LinePicker.m_noColor = TRUE;

	if (!(p->m_state&0x02))
		m_FillPicker.m_noColor = TRUE;

}

void CTextFormatPage::GetValues(CDrawText *p)
{
	p->m_allignment = m_allignment;
	p->m_nBorderType = m_borderType;
	p->m_nShape = (CDrawRect::Shape)m_borderType;
	
	p->m_dashStyle = m_dashStyle;
//	p->m_dashCap = (DashCap) m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
    p->m_brushType = m_FillPicker.m_BrushType;
    p->m_penbrushType = m_LinePicker.m_BrushType;
	p->m_brushColor = m_FillPicker.m_StartColor; //when hatch pattern used, this is foreColor
    p->m_gradientColor = m_FillPicker.m_EndColor;
	p->m_penColor= m_LinePicker.m_StartColor;
	p->m_penEndColor = m_LinePicker.m_EndColor;
	p->m_penWidth = m_penSize;
	p->m_linearGradientMode =  m_FillPicker.m_linearFillMode;
	p->m_penlinearGradientMode =  m_LinePicker.m_linearFillMode;
	p->m_hatchStyle=  m_FillPicker.m_hatchStyle;
	p->m_fillImage =m_FillPicker.m_fileName;
	p->m_wrapMode = m_FillPicker.m_wrapMode;
	p->m_bToCenter = m_FillPicker.m_bToCenter; //0, - center path gradient.
	p->m_nStretch = m_FillPicker.m_nStretch;
	if (p->m_brushType == BrushTypeSolidColor )
		p->m_brushColor.SetFromCOLORREF(m_FillColor);

	p->m_Transparency = m_Transparency;

	if (m_LinePicker.m_BrushType == BrushTypeSolidColor )
		p->m_penColor.SetFromCOLORREF(m_LineColor);

	if (m_FillPicker.m_noColor)
		p->m_state = p->m_state&(0xff^0x02);
	else
		p->m_state = p->m_state|0x02;

	if (m_LinePicker.m_noColor)
		p->m_state = p->m_state&(0xff^0x01);
	else
		p->m_state = p->m_state|0x01;
}

void CTextFormatPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar == &m_transCtrl){
		switch (nSBCode){
		case SB_THUMBTRACK://5
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_LINERIGHT: //1
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()+1);
			break;
		case SB_RIGHT : //7
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()+1);
			break;
		case SB_PAGERIGHT  ://3
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()+10);
			break;
		case SB_PAGELEFT: //2
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()-10);
			break;//4
		case 8:
			{
				CString s;
				s.Format("%d",pScrollBar->GetScrollPos());
				GetDlgItem(IDC_EDIT1)->SetWindowText(s);
				break;
			}
		case SB_LEFT: //6
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()-1);
			break;
		case SB_LINELEFT: //0
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos()-1);
			break;
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
