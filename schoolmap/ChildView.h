
// ChildView.h : interface of the CChildView class
#pragma once
// CChildView window
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);						//Ignore background update
	afx_msg void OnRButtonDown(UINT nflag, CPoint point);		// Right mouse click, pull up searchDiablog
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point);		// Display dijstra algrithm line
	DECLARE_MESSAGE_MAP()
};

