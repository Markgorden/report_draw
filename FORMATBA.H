// formatba.h : header file
// Initial code from microsoft wordpad example,
// Something changed according to 
#ifndef __FORMATBA_H__
#define __FORMATBA_H__
/////////////////////////////////////////////////////////////////////////////
// CFormatBar dialog
class CFormatBar : public CBCGPToolBar
{
	DECLARE_SERIAL(CFormatBar)

// Construction
public:
	CFormatBar();
	
// Operations
public:
	virtual void CFormatBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
 
// Attributes
public:
	CSize m_szBaseUnits;
// Implementation
	CPalette	m_palColorPicker;	// Palette for color picker
	int			m_nNumColours;

protected:
	virtual void OnReset ();
	CBCGPColorMenuButton* CreateColorButton (int ID);
	CBCGPToolbarFontCombo* CreateFontComboButton ();

	// Generated message map functions
	//{{AFX_MSG(CFormatBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////
