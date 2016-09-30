// linelist.h : header file
/////////////////////////////////////////////////////////////////////////////
// CLineWidthMenu window
class CDisplayIC : public CDC
{
public:
	CDisplayIC() { CreateIC(_T("DISPLAY"), NULL, NULL, NULL); }
};

class CLineWidthMenu : public CMenu
{
// Construction
public:
	CLineWidthMenu();

// Attributes
public:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

};

/////////////////////////////////////////////////////////////////////////////
// CLineStyleMenu window


class CLineStyleMenu : public CMenu
{
// Construction
public:
	CLineStyleMenu();

// Attributes
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

};

/////////////////////////////////////////////////////////////////////////////
// CLeftArrowMenu window


class CLeftArrowMenu : public CMenu
{
// Construction
public:
	CLeftArrowMenu();

// Attributes
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

};

/////////////////////////////////////////////////////////////////////////////
// CRightArrowMenu window


class CRightArrowMenu : public CMenu
{
// Construction
public:
	CRightArrowMenu();

// Attributes
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

};
class CDashLine;
/////////////////////////////////////////////////////////////////////////////
void gDrawArrow(int lArrowType, int rArrowType,int lArrowSize ,int rArrowSize,
				  POINT  p1, POINT p2, CDC *pDC, CDashLine &dashline, CBrush& brush);
void gDrawArrowHead(int arrowType, POINT  p1, POINT p2, int arrowSize ,CDC *pDC,
					  CBrush& brush);
