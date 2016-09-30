/********************************************************************/
/* DrawDoc.h: Interface for CDrawDoc and derivatives            */
/*				Tao Zhu, September, 2001							*/
/*				University of Pittsburgh							*/
/*              Pittsburhg, PA 15219								*/
/*              tzzt@yahoo.com										*/
/*              Copyright 2001, all rights reserved.				*/
/********************************************************************/

#if !defined(AFX_DRAWDOC_INCLUDED_)
#define AFX_DRAWDOC_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "drawobj.h"

class CDrawSrvrItem;
class CNetworkView;

class CDrawDoc : public COleDocument
{
protected: // create from serialization only
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

public:
	const CSize&	GetSize() const { return m_size; }
	void			ComputePageSize();
	int				GetMapMode() const { return m_nMapMode; }
	COLORREF		GetPaperColor() const { return m_paperColor; }

// Operations
public:
	CDrawObj*		ObjectAt(const CPoint& point);
	void			Draw(CDC* pDC, CNetworkView* pView);
	void			Draw(CDC* pDC, CNetworkView* pView, UINT nPage);
	void			Add(CDrawObj* pObj);
	void			Add(CDrawObj* pObj,CDrawObj* pAfter);
	bool			Remove(CDrawObj* pObj);
	void 			Initialize();
	void			OnNextPage();
	void			OnPagePrevious();
	void			ShowPageNum();
	void			GoToPage (int );
	CDrawObj*		FindObjectAfter(CDrawObj *pObj);
	CDrawObjList*	GetObjects() {return &m_objects;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnSetItemRects(LPCRECT lpPosRect, LPCRECT lpClipRect);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawDoc();

protected:
	CDrawObjList	m_objects;					  /*all drawobjs in the document*/
	CSize			m_size;										/*size of paper */
	COLORREF		m_paperColor;
	int				m_nMapMode;

public:
	CString			m_workDir;     /*location of the working project*/

	int				m_nTotalPage;
	int				m_nCurrentPage;
protected:
	virtual BOOL	OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL	OnSaveDocument(LPCTSTR lpszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawDoc)
	afx_msg void OnViewPapercolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#ifdef DEBUG	
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
int GetObjectPosition(const CDrawObjList &list,CDrawObj*pObj);


#endif // !defined(AFX_DRAWDOC_INCLUDED_)
