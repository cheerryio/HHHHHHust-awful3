#pragma once


// CMapPoint

class CMapPoint : public CWnd
{
	DECLARE_DYNAMIC(CMapPoint)

public:
	int id = 0;		// point id
	int x=3;		// x
	int y=3;		// y

public:
	CMapPoint();
	CMapPoint(int id, int x, int y);
	virtual ~CMapPoint();

//events
public:

protected:
	afx_msg void OnPaint();									// simple circle with radius of 3
	afx_msg void OnLButtonDown(UINT nflag, CPoint point);	// left mouse click,should set the point start or cancel start
	afx_msg void OnRButtonDown(UINT nflag, CPoint point);	// right mouse click,should set the point end or cancel end
	afx_msg void OnMouseMove(UINT nflag, CPoint point);		// set trackMouseEvent,for one in only set once
	afx_msg void OnMouseLeave();							// set flag for next new trackMouseEvent
	afx_msg void OnMouseHover(UINT nflag, CPoint point);	// mouse hover for a period of time event
	DECLARE_MESSAGE_MAP()
};


