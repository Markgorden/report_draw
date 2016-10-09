// TextProperty.cpp : implementation file
//

#include "stdafx.h"
#include "Draw.h"
#include "TextProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// reserve lobyte for charset
#define PRINTER_FONT 0x0100
#define TT_FONT 0x0200
#define DEVICE_FONT 0x0400

#define BMW 16
#define BMH 15

LOGFONT NEAR CTextProperty::m_lfDefFont;
LOGFONT NEAR CTextProperty::m_lfDefFontOld;
LOGFONT NEAR CTextProperty::m_lfDefPrintFont;
LOGFONT NEAR CTextProperty::m_lfDefPrintFontOld;


/////////////////////////////////////////////////////////////////////////////
// CTextProperty property page

IMPLEMENT_DYNCREATE(CTextProperty, CPropertyPage)

CTextProperty::CTextProperty() : CPropertyPage(CTextProperty::IDD)
{
	//{{AFX_DATA_INIT(CTextProperty)
	m_text = _T("");
	//}}AFX_DATA_INIT
    //load all font name to the list
	//fontCombo.SubclassDlgItem(IDC_FONT,this);
	m_bFont=false;
}

CTextProperty::~CTextProperty()
{
}

void CTextProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextProperty)
	DDX_Text(pDX, IDC_TEXT_INPUT, m_text);
	//}}AFX_DATA_MAP
//	DDX_ColourPicker(pDX, IDC_FONT_COLOR, m_textColor);
}


BEGIN_MESSAGE_MAP(CTextProperty, CPropertyPage)
	//{{AFX_MSG_MAP(CTextProperty)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_TEXT_INPUT, &CTextProperty::OnEnChangeTextInput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProperty message handlers





void CTextProperty::OnFont() 
{
   LOGFONT lf;
	int x=m_font.GetLogFont(&lf);
   //::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_EFFECTS|CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
    
	
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
        m_bFont=false; 
		if (m_font.CreateFontIndirect(&lf))
		{
			CWaitCursor wait;
			//SetFont(&m_font);
			m_lfDefFont = lf;
			m_bFont=true; //create font successfully

	/*		if (GetPrinterFont() == NULL)
			{
				// notify container that content has changed
				GetDocument()->UpdateAllItems(NULL);
			}*/
			m_color=dlg.GetColor();
		}
	}
       
} 

BOOL CTextProperty::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextProperty::OnEnChangeTextInput()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
