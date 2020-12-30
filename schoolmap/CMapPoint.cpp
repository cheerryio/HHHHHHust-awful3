// CMapPoint.cpp : implementation file
//

#include "pch.h"
#include "schoolmap.h"
#include "CMapPoint.h"

#include <vector>
#include "structs.h"
#include "Manager.h"
#include <iostream>

using namespace std;

// CMapPoint

IMPLEMENT_DYNAMIC(CMapPoint, CWnd)

CMapPoint::CMapPoint() {}

CMapPoint::CMapPoint(int id,int x, int y) :id(id),x(x), y(y) {}

CMapPoint::~CMapPoint() {}

BEGIN_MESSAGE_MAP(CMapPoint, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CMapPoint message handlers

void CMapPoint::OnPaint() {
    CPaintDC dc(this);
    CRect rect(0,0,2*Manager::mapPointSize, 2*Manager::mapPointSize);
    CBrush cb(RGB(255, 0, 0));
    CBrush* originalBrush = dc.SelectObject(&cb);
    dc.Ellipse(&rect);
    dc.SelectObject(originalBrush);
    return;
}

void CMapPoint::OnLButtonDown(UINT nflag, CPoint point) {
    if (Manager::startVertexId == id)
        Manager::startVertexId = -1;
    else
        Manager::startVertexId = id;
    string title = Manager::startendString(Manager::startVertexId, Manager::endVertexId);
    SetWindowTextA(AfxGetMainWnd()->GetSafeHwnd(), title.c_str());
    
    return;
}

void CMapPoint::OnRButtonDown(UINT nflag, CPoint point) {
    if (Manager::endVertexId == id)
        Manager::endVertexId = -1;
    else
        Manager::endVertexId = id;
    string title = Manager::startendString(Manager::startVertexId, Manager::endVertexId);
    SetWindowTextA(AfxGetMainWnd()->GetSafeHwnd(), title.c_str());
    return;
}

bool isset = false;
void CMapPoint::OnMouseMove(UINT nflag, CPoint point) {
    tagTRACKMOUSEEVENT trackMouseEvent{
        sizeof(TRACKMOUSEEVENT),
        TME_HOVER  | TME_LEAVE,         // Be aware I just want catch these two events
        this->GetSafeHwnd(),
        2000                            // 2 seconds interval
    };
    if (!isset) {
        TrackMouseEvent(&trackMouseEvent);  // register trackMouseEvent
        isset = true;
    }

    return;
}

void CMapPoint::OnMouseLeave() {
    isset = false;
    if (Manager::organizationDropMenuOn) {
        Manager::organizationDropMenuOn = false;
        delete Manager::dropMenu;
    }
}

void CMapPoint::OnMouseHover(UINT nFlags, CPoint point) {
    if (!Manager::organizationDropMenuOn) {
        Manager::organizationDropMenuOn = true;     // prevent other nodes from generating dropMenu
        Manager::dropMenu = new CListBox;
        Manager::dropMenu->Create(WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_HSCROLL,
            CRect(x, y, x + 200, y + 200), Manager::childview, 1);
        for (int i = 0; i < Manager::vertexes[id].orgs.size(); i++) {
            Manager::dropMenu->AddString(Manager::organizations[Manager::vertexes[id].orgs[i]].name);
        }
    }
    return;
}


