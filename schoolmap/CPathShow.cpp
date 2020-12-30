// pathShow.cpp : implementation file
//

#include "pch.h"
#include "schoolmap.h"
#include "CPathShow.h"
#include "Manager.h"

#include <vector>

using namespace std;

// pathShow

IMPLEMENT_DYNAMIC(CPathShow, CWnd)

CPathShow::CPathShow() {}

static void CALLBACK MyTimerProc(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime) {
	if (Manager::pathShow == nullptr)
		return;

	Manager::isPathShow = Manager::isPathShow ? false : true;
	Manager::childview->Invalidate();
	//Manager::childview->Invalidate();

	return;
}
CPathShow::CPathShow(vector<int> pointSequence):pointSequence(pointSequence) {
	// why cant i set timer when passing hwnd:this->GetSafeHwnd()???????
	int timerID = ::SetTimer(Manager::childview->GetSafeHwnd(), 2, 1000, MyTimerProc);
}

CPathShow::~CPathShow()
{
}


BEGIN_MESSAGE_MAP(CPathShow, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// pathShow message handlers

void CPathShow::OnPaint() {
	CPaintDC dc(this);

	if (pointSequence.size() < 2 || Manager::vertexes.empty() || !Manager::isPathShow)
		return;

	CPen penBlack;  // Construct it, then initialize
	penBlack.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&penBlack);

	dc.MoveTo(CPoint(Manager::vertexes[pointSequence[0]].x, Manager::vertexes[pointSequence[0]].y));
	for (int i = 1; i < pointSequence.size(); i++) {
		dc.LineTo(CPoint(Manager::vertexes[pointSequence[i]].x, Manager::vertexes[pointSequence[i]].y));
	}
	dc.SelectObject(pOldPen);
	return;
}


void CPathShow::OnTimer(UINT_PTR nIDEvent) {
	AfxMessageBox(L"wawaw");

	return;
}
