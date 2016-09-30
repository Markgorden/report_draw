// LineProperty.cpp : implementation file
//

#include "stdafx.h"
#include "draw.h"
#include "LineProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineProperty property page

IMPLEMENT_DYNCREATE(CLineProperty, CPropertyPage)

CLineProperty::CLineProperty() : CPropertyPage(CLineProperty::IDD)
{
	//{{AFX_DATA_INIT(CLineProperty)
	m_Fill = 1;
	m_penSize = 0;
	m_round = FALSE;
	m_bCurve = FALSE;
	//}}AFX_DATA_INIT
//	m_arrowEnd=noArrow;
//	m_arrowStart=noArrow;
//	m_lineStyle=CDashLine::DL_SOLID;
	m_FillColor=COLORREF(RGB(255,255,255));
	m_LineColor=COLORREF(RGB(0,0,0));
	m_endArrow = 0;
	m_startArrow =0;
	pObj=NULL;
	m_FillPicker.m_effectpage = 0x7;
	m_LinePicker.m_effectpage = 0x1;
}

CLineProperty::~CLineProperty()
{
}

void CLineProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineProperty)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_transCtrl);
	DDX_Control(pDX, IDC_LINEPICKER, m_linePicker);
	DDX_Control(pDX, IDC_ARROW_END, m_arrowEndPicker);
	DDX_Control(pDX, IDC_ARROW_START, m_arrowStartPicker);
	DDX_Control(pDX, IDC_FILL_COLOR, m_FillPicker);
	DDX_Control(pDX, IDC_LINE_COLOR, m_LinePicker);
	DDX_Radio(pDX, IDC_NOFILL, m_Fill);
	DDX_Text(pDX, IDC_LINE_SIZE, m_penSize);
	DDX_Text(pDX, IDC_ARROW_START_SIZE, m_startArrowSize);
	DDX_Text(pDX, IDC_ARROW_END_SIZE, m_endArrowSize);
	DDX_Check(pDX, IDC_ROUND_PEN, m_round);
	DDX_Text(pDX, IDC_EDIT1, m_Transparency);
	DDV_MinMaxInt(pDX, m_Transparency, 0, 100);
	//}}AFX_DATA_MAP
	DDX_ArrowPicker(pDX, IDC_ARROW_END, m_endArrow);
	DDX_ArrowPicker(pDX, IDC_ARROW_START, m_startArrow);
	DDX_ColourPicker(pDX, IDC_FILL_COLOR, m_FillColor);
	DDX_ColourPicker(pDX, IDC_LINE_COLOR, m_LineColor);
	DDX_LinePicker(pDX, IDC_LINEPICKER, (int&)m_dashStyle);


}


BEGIN_MESSAGE_MAP(CLineProperty, CPropertyPage)
	//{{AFX_MSG_MAP(CLineProperty)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineProperty message handlers

BOOL CLineProperty::OnInitDialog() 
{
	
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSpinButtonCtrl *p;
	p=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	p->SetRange(0,100);
	p=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2);
	p->SetRange(0,100);
	p=(CSpinButtonCtrl *)GetDlgItem(IDC_SPIN3);
	p->SetRange(0,100);
	EnableArrowSetting(m_isArrow);


    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN5))->SetRange(0,100);

	CScrollBar *pS = (CScrollBar *)GetDlgItem(IDC_SCROLLBAR1);
	pS->EnableScrollBar();
	pS->SetScrollRange(0,100);

	m_linePicker.SetMaximumLines(5);
	return TRUE;
 }

void CLineProperty::EnableArrowSetting(bool set)
{
    CButton *pb;
	pb=(CButton *)GetDlgItem(IDC_ARROW_START);
	pb->EnableWindow(set);
	pb=(CButton *)GetDlgItem(IDC_ARROW_END);
	pb->EnableWindow(set);
    CEdit *pe;
	pe=(CEdit *)GetDlgItem(IDC_ARROW_START_SIZE);
	pe->EnableWindow(set);
	pe=(CEdit *)GetDlgItem(IDC_ARROW_END_SIZE);
	pe->EnableWindow(set);
}


LONG CLineProperty::OnSelEndOK(UINT lParam, LONG wParam)
{
    return TRUE;
}


void CLineProperty::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void CLineProperty::SetValues(CDrawObj *p)
{
//	m_lineStyle = (CDashLine::DashStyle)p->m_dashStyle;
	m_dashCap = p->m_dashCap ;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
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
	
	m_startArrowSize=p->m_startArrowSize;
	m_endArrowSize=p->m_endArrowSize;
	m_endArrow=GetArrowIndex(p->m_endArrow);  
	m_startArrow=GetArrowIndex(p->m_startArrow);

	m_dashStyle = p->m_dashStyle;

	m_isArrow=false;
	if (p->IsKindOf(RUNTIME_CLASS(CDrawRect)))
		if(((CDrawRect *)p)->m_nShape==CDrawRect::arrow)
			m_isArrow=true;
	if (!(p->m_state&0x01))
		m_LinePicker.m_noColor = TRUE;

	if (!(p->m_state&0x02))
		m_FillPicker.m_noColor = TRUE;

}

void CLineProperty::GetValues(CDrawObj *p)
{
//	p->m_dashStyle = (DashStyle) m_lineStyle;
	p->m_dashCap = (DashCap) m_dashCap;   // 0 :flat, 2: round, 3: triangle, don't know what is 1.
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
	p->m_startArrowSize = m_startArrowSize;
	p->m_endArrowSize = m_endArrowSize;
	p->m_endArrow = GetArrowType(m_endArrow);
	p->m_startArrow = GetArrowType(m_startArrow);
	p->m_dashStyle  = m_dashStyle;
	
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


ArrowTypeG CLineProperty::GetArrowType(int i){
	switch (i){
	case 0:
		return NoAnchor;
	case 1:
		return ArrowAnchor;
	case 2:
		return OpenAnchor;
	case 3:
		return StealthAnchor;
	case 4:
		return DimondAnchor;
	case 5:
		return RoundAnchor;
	default:
		return NoAnchor;
	}
}
int CLineProperty::GetArrowIndex(ArrowTypeG type){
	switch (type){
	case NoAnchor:
		return 0;
	case ArrowAnchor:
		return 1;
	case OpenAnchor:
		return 2;
	case StealthAnchor:
		return 3;
	case DimondAnchor:
		return 4;
	case RoundAnchor:
		return 5;
	default:
		return 0;
	}
}
