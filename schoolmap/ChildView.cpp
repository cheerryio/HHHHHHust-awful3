
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "schoolmap.h"
#include "ChildView.h"
#include "CMapPoint.h"
#include "CPathShow.h"

#include "searchDialog.h"

#include "structs.h"
#include "Manager.h"

#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	Manager::childview = this;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CChildView message handlers
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

// load bmp file as view background
void loadMap(CDC* pDC) {
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CRect rctClient;
	GetClientRect(AfxGetMainWnd()->m_hWnd, rctClient);
	CBitmap m_bmp;
	BITMAP bitmap;
	CString filePath("schoolmap.bmp");
	HINSTANCE hh = AfxGetInstanceHandle();
	HBITMAP hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filePath, IMAGE_BITMAP, 1676, 820, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbitmap == NULL) return;
	m_bmp.Attach(hbitmap);
	m_bmp.GetBitmap(&bitmap);
	CBitmap* pbmpOld = dcMem.SelectObject(&m_bmp);
	pDC->StretchBlt(0, 0, rctClient.Width(), rctClient.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	return;
}

// create a CMapPoint control at each position of point
void drawLocationPoint(CDC* pDC) {
	vector<VertexProps> vertexes = Manager::vertexes;
	int size = Manager::mapPointSize;
	for (int i = 0; i < vertexes.size(); i++) {
		CMapPoint* ppoint = new CMapPoint(vertexes[i].id, vertexes[i].x, vertexes[i].y);
		CRect pointRect(ppoint->x - size, ppoint->y - size, ppoint->x + size, ppoint->y + size);
		ppoint->Create(_T("STATIC"), _T("hello"), WS_CHILD | WS_VISIBLE | SS_NOTIFY, pointRect, Manager::childview, 1);
	}
	return;
}

void CChildView::OnPaint() 
{
	// prepration for painting on dcMem, and copy to dc
	CPaintDC dc(this);
	CDC dcMem;
	int nRet = dcMem.CreateCompatibleDC(&dc);
	if (nRet == 0)
		TRACE("Cannot creat memory dc\n");
	CRect rctClient;
	::GetClientRect(AfxGetMainWnd()->m_hWnd, rctClient);
	CBitmap bmpMem;
	nRet = bmpMem.CreateCompatibleBitmap(&dc,
		rctClient.Width(),
		rctClient.Height());
	if (nRet == 0)
		TRACE("Cannot create compatible bitmap.\n");
	// select the bitmap to memory dc
	CBitmap* pOldBmp = (CBitmap*)(dcMem.SelectObject(&bmpMem));


	loadMap(&dcMem);	//º”‘ÿµÿÕºÕº∆¨
	if (Manager::pointLoaded && Manager::pointFirstLoad) {		//ª≠Œª÷√∫Ïµ„
		drawLocationPoint(&dcMem);
		Manager::pointFirstLoad = false;
	}
	dc.BitBlt(rctClient.left, rctClient.top, rctClient.Width(), rctClient.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBmp);
	bmpMem.DeleteObject();
	dcMem.DeleteDC();
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// create pathShow control
void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	Manager::pathShowOnMap();
	return;
}

void CChildView::OnRButtonDown(UINT nflag, CPoint point) {
	searchDialog dlg;
	dlg.DoModal();
}