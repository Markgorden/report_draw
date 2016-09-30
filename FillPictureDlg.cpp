// FillPictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "draw.h"
#include "FillPictureDlg.h"
#include "EffectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillPictureDlg dialog
IMPLEMENT_DYNAMIC (CFillPictureDlg, CPropertyPage);

CFillPictureDlg::CFillPictureDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CFillPictureDlg::IDD)
{
	//{{AFX_DATA_INIT(CFillPictureDlg)
	m_type = -1;
	//}}AFX_DATA_INIT
}


void CFillPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillPictureDlg)
	DDX_Control(pDX, IDC_COMBO1, m_typeCtrl);
	DDX_Control(pDX, IDC_BUTTON2, m_sample);
	DDX_CBIndex(pDX, IDC_COMBO1, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillPictureDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFillPictureDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillPictureDlg message handlers

void CFillPictureDlg::DrawSample()
{

	//send message to m_sample to let it redraw;
	m_sample.Invalidate();
	m_sample.UpdateWindow();

}


void CFillPictureDlg::OnSelchangeCombo1() 
{
	SetValue();
	DrawSample();
}

void CFillPictureDlg::OnButton1() 

{
    static char BASED_CODE szFilter[] = "JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|Windows Bitmap (*.bmp)|*.bmp|Graphics Interchange Format (*.gif)|*.gif|Exchangeable Image File(*.Exif)|*.Exif|Tag Image File Format(*.tiff)|*.tiff|Windows Metafile (*.WMF)|*.wmf|Portable Network Graphics (*.PNG)|*.png|All Files (*.*)|*.*|";
	CFileDialog fileDlg( TRUE, ".gif", "", OFN_HIDEREADONLY, szFilter, this);
	if (fileDlg.DoModal() != IDOK)
		return;
	m_fileName=fileDlg.GetPathName();

    int len = m_fileName.GetLength();
	unsigned short* string = new unsigned short[m_fileName.GetLength()+1];
	int i = 0;
	for ( i = 0; i<len;i++)
		string[i] = (unsigned short) m_fileName[i];
	string[i] = 0;

	Image *image = Image::FromFile((WCHAR*)string);


	if (image->GetLastStatus()!=Ok){
		AfxMessageBox("Not valid image file!");
		m_fileName = "";
	}
	else{
		CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
		pDlg->m_fileName = m_fileName;
		GetDlgItem(IDC_STATICNAME)->SetWindowText(m_fileName);
		SetValue();
		DrawSample();
	}
	delete [] string;
}


void CFillPictureDlg::SetValue()
{
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());

	pDlg->m_BrushType = BrushTypeTextureFill;

	//file name is already set by the OnButton1

	if (m_typeCtrl.GetCurSel() <=4){
		 	pDlg->m_wrapMode = (WrapMode)m_typeCtrl.GetCurSel();
			pDlg->m_nStretch = 0;
	}
	 else
			pDlg->m_nStretch = 1;
}

BOOL CFillPictureDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CEffectDlg *pDlg = (CEffectDlg *)(GetParent()->GetParent());
	if (pDlg->m_fileName!="")
		GetDlgItem(IDC_STATICNAME)->SetWindowText(pDlg->m_fileName);

	m_typeCtrl.SetCurSel(pDlg->m_wrapMode +pDlg->m_nStretch); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
