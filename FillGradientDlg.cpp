// FillGradientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "draw.h"
#include "FillGradientDlg.h"
#include "FillPatterndlg.h"
#include "FillPicturedlg.h"

#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "effectdlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFillGradientDlg dialog
IMPLEMENT_DYNAMIC (CFillGradientDlg, CPropertyPage);


CFillGradientDlg::CFillGradientDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFillGradientDlg::IDD)
{
	//{{AFX_DATA_INIT(CFillGradientDlg)
	m_startTransparency = 0;
	m_endTransparency = 0;
	m_dark = 0;
	m_nStyle = -1;
	//}}AFX_DATA_INIT
	m_color1btn.m_strEffectText="";
	m_color1btn.m_strNoText="";
	m_color2btn.m_strEffectText="";
	m_color2btn.m_strNoText="";
	m_bInitilized = FALSE;
}


void CFillGradientDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillGradientDlg)
	DDX_Control(pDX, IDC_SAMPLE, m_sample);
	DDX_Control(pDX, IDC_SCROLLBAR4, m_darkCtrl);
	DDX_Control(pDX, IDC_BUTTON8, m_color2btn);
	DDX_Control(pDX, IDC_BUTTON2, m_color1btn);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_endTr);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_startTr);
	DDX_Text(pDX, IDC_EDIT1, m_startTransparency);
	DDV_MinMaxInt(pDX, m_startTransparency, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_endTransparency);
	DDV_MinMaxInt(pDX, m_endTransparency, 0, 100);
	DDX_Scroll(pDX, IDC_SCROLLBAR4, m_dark);
	DDX_Radio(pDX, IDC_RADIO4, m_nStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillGradientDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFillGradientDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_WM_HSCROLL()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillGradientDlg message handlers

BOOL CFillGradientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //display the gradient based on the selction
	GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->ShowWindow(FALSE);

	GetDlgItem(IDC_SCROLLBAR4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());

    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1))->SetRange(0,100);
    ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2))->SetRange(0,100);

	CScrollBar *pS = (CScrollBar *)GetDlgItem(IDC_SCROLLBAR1);
	pS->EnableScrollBar();
	pS->SetScrollRange(0,100);

	pS = (CScrollBar *)GetDlgItem(IDC_SCROLLBAR2);
	pS->EnableScrollBar();
	pS->SetScrollRange(0,100);

	pS = (CScrollBar *)GetDlgItem(IDC_SCROLLBAR4);
	pS->EnableScrollBar();
	pS->SetScrollRange(0,100);
	

	if (pDlg->m_BrushType == BrushTypeLinearGradient || pDlg->m_BrushType == BrushTypePathGradient ) {
	
		GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);

		if (pDlg->m_nColor==1){
			//hide the button
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(FALSE);

			GetDlgItem(IDC_SCROLLBAR4)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		}
		else if (pDlg->m_nColor==2){
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
			GetDlgItem(IDC_BUTTON8)->ShowWindow(TRUE);

			GetDlgItem(IDC_SCROLLBAR4)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		}

	}

	m_color1btn.SetColour(pDlg->m_StartColor.ToCOLORREF());
	m_color2btn.SetColour(pDlg->m_EndColor.ToCOLORREF());


	//draw the sample
	DrawSample();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFillGradientDlg::OnRadio1() 
{
	if (!m_bInitilized){
		m_bInitilized = TRUE;
		return; //don't know why OnRadio1 is automatically called. SO I use this stupid way to let it jump over once
	}

	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_nColor = 1;
	GetDlgItem(IDC_BUTTON8)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);

	GetDlgItem(IDC_SCROLLBAR4)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);

	SetValue();
	DrawSample();
}

void CFillGradientDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_nColor = 2;
	GetDlgItem(IDC_BUTTON8)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);

	GetDlgItem(IDC_SCROLLBAR4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	SetValue();
	DrawSample();
}

void CFillGradientDlg::OnRadio4() 
{
//Horizontal Linear Graident;
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypeLinearGradient;
	pDlg->m_linearFillMode = LinearGradientModeHorizontal; //0
	pDlg->m_nStyle =0;
	m_nStyle = 0;
	DrawSample();
}

void CFillGradientDlg::OnRadio5() 
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypeLinearGradient;
	pDlg->m_linearFillMode = LinearGradientModeVertical ; //1
	pDlg->m_nStyle = 1;
	m_nStyle = 1;
	DrawSample();
	
}
 

void CFillGradientDlg::OnRadio6() 
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypeLinearGradient;
	pDlg->m_linearFillMode = LinearGradientModeForwardDiagonal ; //2
	pDlg->m_nStyle = 2;
	m_nStyle = 2;
	DrawSample();
}

void CFillGradientDlg::OnRadio7() 
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypeLinearGradient;
	pDlg->m_linearFillMode = LinearGradientModeBackwardDiagonal  ; //2
	pDlg->m_nStyle = 3;
	m_nStyle = 3;
	DrawSample();
}

void CFillGradientDlg::OnRadio8() 
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypePathGradient ;
	pDlg->m_nStyle = 4;
	m_nStyle = 4;
	DrawSample();
}

void CFillGradientDlg::OnRadio9() 
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	pDlg->m_BrushType = BrushTypePathGradient ;
	pDlg->m_nStyle = 5;
	m_nStyle = 5;
	DrawSample();
}

void CFillGradientDlg::DrawSample()
{
	//send message to m_sample to let it redraw;
	m_sample.Invalidate();
	m_sample.UpdateWindow();

}

void CFillGradientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	if (pScrollBar == &m_startTr ||pScrollBar == &m_endTr ||pScrollBar == &m_darkCtrl){
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
				if ( pScrollBar == &m_startTr ){
					GetDlgItem(IDC_EDIT1)->SetWindowText(s);
				}
				else if(pScrollBar == &m_endTr){
					GetDlgItem(IDC_EDIT2)->SetWindowText(s);
				}
				SetValue();
				DrawSample();
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


LONG CFillGradientDlg::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
	SetValue();
	DrawSample();
    return TRUE;
}

void CFillGradientDlg::SetValue()
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());

	pDlg->m_BrushType = (pDlg->m_nStyle>=4)? BrushTypePathGradient :BrushTypeLinearGradient;

	if (pDlg->m_nStyle<4)
		pDlg->m_linearFillMode = (LinearGradientMode)pDlg->m_nStyle;


	Color c1;
	c1.SetFromCOLORREF(m_color1btn.GetColour());
	Color c2;

	if (pDlg->m_nColor ==1){
		int gray = 255*m_darkCtrl.GetScrollPos()/100;
		c2 = Color(255, gray, gray, gray);
	}
	else
		c2.SetFromCOLORREF(m_color2btn.GetColour());

    //set the alpha value based on the transparency textbox;
	BYTE alpha1 = (100 - m_startTr.GetScrollPos())*255/100;
	BYTE alpha2 = (100 - m_endTr.GetScrollPos())*255/100;
	
	//Direcly set up the color

	pDlg->m_StartColor = SetA(c1,alpha1);
	pDlg->m_EndColor = SetA(c2,alpha2);
}
/////////////////////////////////////////////////////////////////////////////
// CSampleButton

CSampleButton::CSampleButton()
{
}


IMPLEMENT_DYNCREATE(CSampleButton, CButton)


void CSampleButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct){


	if (GetParent()->IsKindOf(RUNTIME_CLASS(CFillGradientDlg)))
		DrawItemLinear(lpDrawItemStruct);
	else if (GetParent()->IsKindOf(RUNTIME_CLASS(CFillPatternDlg)))
		DrawItemPattern(lpDrawItemStruct);
	else if (GetParent()->IsKindOf(RUNTIME_CLASS(CFillPictureDlg)))
		DrawItemPicture(lpDrawItemStruct);

}
void CSampleButton::DrawItemLinear(LPDRAWITEMSTRUCT lpDrawItemStruct){

	CFillGradientDlg *pDlg = (CFillGradientDlg *)GetParent();

	CEffectDlg *pEffDlg = (CEffectDlg *)(GetParent()->GetParent()->GetParent());

	CRect rect = lpDrawItemStruct->rcItem;
	CDC  *pdrawDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC memDC(pdrawDC);
	CDC  *pDC = &memDC;

	Graphics graphics(pDC->m_hDC);

	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	Brush *pBrush =NULL;
	
	GraphicsPath path;
	path.AddRectangle(theRect);

	PathGradientBrush pthGrBrush(&path);	
	// Set the color at the center of the path to blue.

	Color c1 = pEffDlg->m_StartColor;
	Color c2 = pEffDlg->m_EndColor;

	if (pDlg->m_nStyle==4)
		pthGrBrush.SetCenterColor(c1);
	else 
		pthGrBrush.SetCenterColor(c2);

	// Set the color along the entire boundary of the path to aqua.
	if (pDlg->m_nStyle==4){
		Color colors[] = {c2};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	else{
		Color colors[] = {c1};
		int count = 1;
		pthGrBrush.SetSurroundColors(colors, &count);
	}
	//line gradient brush
	LinearGradientMode linearGradientMode = LinearGradientModeHorizontal ; //default is horizontal
	if (pDlg->m_nStyle<4)
		linearGradientMode = (LinearGradientMode)pDlg->m_nStyle;

	LinearGradientBrush linGrBrush( theRect, c1,  c2, linearGradientMode);


	if (pEffDlg->m_BrushType == BrushTypeLinearGradient) {
		pBrush = &linGrBrush;
	}
	else if (pEffDlg->m_BrushType == BrushTypePathGradient){
		pBrush = &pthGrBrush;
	}
	else
		return;
	graphics.FillRectangle(pBrush,theRect);

}


void CSampleButton::DrawItemPattern(LPDRAWITEMSTRUCT lpDrawItemStruct){

	CFillPatternDlg *pDlg = (CFillPatternDlg *)GetParent();

	CEffectDlg *pEffDlg = (CEffectDlg *)(GetParent()->GetParent()->GetParent());

	CRect rect = lpDrawItemStruct->rcItem;

	CDC  *pdrawDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC memDC(pdrawDC);
	CDC  *pDC = &memDC;

	Graphics graphics(pDC->m_hDC);

	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

	Color c1;
	c1.SetFromCOLORREF(pDlg->m_foreColorCtrl.GetColour());
	Color c2;
	c2.SetFromCOLORREF(pDlg->m_backColorCtrl.GetColour());

	//Direcly set up the color

	pEffDlg->m_StartColor = c1;
	pEffDlg->m_EndColor = c2;
	pEffDlg->m_hatchStyle = (HatchStyle) pDlg->m_hatchbtn.m_crHatch;
	
	
	HatchBrush hBrush(pEffDlg->m_hatchStyle, c1,c2);

	graphics.FillRectangle(&hBrush,theRect);

}
void CSampleButton::DrawItemPicture(LPDRAWITEMSTRUCT lpDrawItemStruct){

	CFillPictureDlg *pDlg = (CFillPictureDlg *)GetParent();

	pDlg->m_type = pDlg->m_typeCtrl.GetCurSel();

	if(pDlg->m_type == -1){
		return;
	}

	CEffectDlg *pEffDlg = (CEffectDlg *)(GetParent()->GetParent()->GetParent());

	CRect rect = lpDrawItemStruct->rcItem;

	CDC  *pdrawDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CMemDC memDC(pdrawDC);
	CDC  *pDC = &memDC;

	Graphics graphics(pDC->m_hDC);

	Rect theRect (rect.TopLeft().x,rect.TopLeft().y,rect.Size().cx,rect.Size().cy); 

    int len = pEffDlg->m_fileName.GetLength();
	unsigned short* string = new unsigned short[pEffDlg->m_fileName.GetLength()+1];
	int i = 0;
	for (; i<len;i++)
		string[i] = (unsigned short) pEffDlg->m_fileName[i];
	string[i] = 0;

	Image* image;
	image = Image::FromFile((WCHAR*)string);
	
	delete [] string;

	TextureBrush tBrush(image);



	if (pDlg->m_type <=4){ //tile;
		tBrush.SetWrapMode((WrapMode)pDlg->m_type);
		if (pDlg->m_type==4){ //has to move the theRect to center;
			if (rect.Width()>image->GetWidth())
				theRect.X += (rect.Width()-image->GetWidth())*.5;
			if (rect.Height()>image->GetHeight())
				theRect.Y += (rect.Height()-image->GetHeight())*.5;
			theRect.Width = image->GetWidth();
			theRect.Height = image->GetHeight();
			graphics.DrawImage(image,theRect);
		}
	}
	else if (pDlg->m_type ==5){ //strech
		tBrush.SetTransform(&Matrix(1.0*rect.Width()/image->GetWidth(), 0.0f, 0.0f,
				1.0*rect.Height()/image->GetHeight(), 0.0f, 0.0f));
	}

	if (pDlg->m_type !=4)
		graphics.FillRectangle(&tBrush,theRect);

	delete image;

}


Color SetA(Color &c,BYTE a)
{
	DWORD a1 = a;
	ARGB argb = c.GetValue();
	a1 = a1*0x1000000;
	argb = argb & 0x00ffffff;
	argb = argb | a1;
	Color c1(argb);
	return c1;
}
