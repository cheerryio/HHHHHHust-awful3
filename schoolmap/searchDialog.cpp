// searchDialog.cpp : implementation file
//

#include "pch.h"
#include "schoolmap.h"
#include "searchDialog.h"
#include "afxdialogex.h"
#include "Manager.h"

#include <map>
#include <string>
#include <iostream>


using namespace std;

// searchDialog dialog

IMPLEMENT_DYNAMIC(searchDialog, CDialogEx)

BEGIN_MESSAGE_MAP(searchDialog, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &searchDialog::onSearchButtonClicked)
	ON_EN_CHANGE(IDC_START_EDIT, &searchDialog::OnEnChangeStartEdit)
	ON_EN_CHANGE(IDC_END_EDIT, &searchDialog::OnEnChangeEndEdit)
	ON_LBN_SETFOCUS(IDC_INFOLIST1, &searchDialog::OnLbnSetfocusInfolist1)
	ON_EN_SETFOCUS(IDC_START_EDIT, &searchDialog::OnEnSetfocusStartEdit)
	ON_EN_SETFOCUS(IDC_END_EDIT, &searchDialog::OnEnSetfocusEndEdit)
	ON_EN_KILLFOCUS(IDC_START_EDIT, &searchDialog::OnEnKillfocusStartEdit)
	ON_EN_KILLFOCUS(IDC_END_EDIT, &searchDialog::OnEnKillfocusEndEdit)
	ON_LBN_SELCHANGE(IDC_INFOLIST1, &searchDialog::OnLbnSelchangeInfolist1)
	ON_LBN_DBLCLK(IDC_INFOLIST1, &searchDialog::OnLbnDblclkInfolist1)
	ON_LBN_DBLCLK(IDC_INFOLIST2, &searchDialog::OnLbnDblclkInfolist2)
	ON_BN_SETFOCUS(IDC_SEARCH_BUTTON, &searchDialog::OnBnSetfocusSearchButton)
	ON_BN_SETFOCUS(IDOK, &searchDialog::OnBnSetfocusOk)
	ON_BN_SETFOCUS(IDCANCEL, &searchDialog::OnBnSetfocusCancel)
END_MESSAGE_MAP()

searchDialog::searchDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCH_DIALOG, pParent)
{

}

searchDialog::~searchDialog()
{
}

void searchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void searchDialog::onSearchButtonClicked() {
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);

	CString cs1;
	GetDlgItemText(IDC_START_EDIT, cs1);
	CString cs2;
	GetDlgItemText(IDC_END_EDIT, cs2);
	map<CString, int> orgMap = Manager::orgMap;
	auto search1 = orgMap.find(cs1);
	auto search2 = orgMap.find(cs2);
	if (search1 != orgMap.end() && search2 != orgMap.end()) {
		Manager::startVertexId = search1->second;
		Manager::endVertexId = search2->second;
		Manager::pathShowOnMap();
		this->CloseWindow();
	}
	else {
		AfxMessageBox(_T("Bad start end point"));
	}

}


// searchDialog message handlers

void searchDialog::OnEnChangeStartEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString cs1;
	GetDlgItemText(IDC_START_EDIT, cs1);
	vector<pair<CString, double>> r;
	for (int i = 0; i < Manager::organizations.size(); i++) {
		CString name = Manager::organizations[i].name;
		int subsequenceNumber = Manager::longestCommonSubsequence(cs1, CString(name));
		double index = Manager::jaccardIndex(cs1, CString(name),subsequenceNumber );
		r.push_back(pair<CString, double>(name, index));
	}

	sort(r.begin(), r.end(), [](pair<CString, double> a, pair<CString, double> b) {
		return a.second > b.second;
		});

	CListBox* infoList = (CListBox*)CListBox::FromHandle(GetDlgItem(IDC_INFOLIST1)->GetSafeHwnd());
	// delete previous items and add new

	int count = infoList->GetCount();
	for (int i = 0; i < count; i++)
	{
		infoList->DeleteString(0);
	}
	infoList->UpdateData(false);
	for (int i = 0; i < r.size() && r[i].second != 0; i++) {
		infoList->AddString(r[i].first);
	}

	if (infoList->GetCount() == 1) {
		CString text;
		infoList->GetText(0, text);
		CString empty("");
		SetDlgItemText(IDC_END_EDIT, empty);
		SetDlgItemText(IDC_START_EDIT, text);
	}

	return;
}


void searchDialog::OnEnChangeEndEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString cs2;
	GetDlgItemText(IDC_END_EDIT, cs2);
	vector<pair<CString, double>> r;
	for (int i = 0; i < Manager::organizations.size(); i++) {
		CString name = Manager::organizations[i].name;
		int subsequenceNumber = Manager::longestCommonSubsequence(cs2, CString(name));
		double index = Manager::jaccardIndex(cs2, CString(name), subsequenceNumber);
		r.push_back(pair<CString, double>(name, index));
	}

	sort(r.begin(), r.end(), [](pair<CString, double> a, pair<CString, double> b) {
		return a.second > b.second;
		});

	CListBox* infoList = (CListBox*)CListBox::FromHandle(GetDlgItem(IDC_INFOLIST2)->GetSafeHwnd());
	// delete previous items and add new

	int count = infoList->GetCount();
	for (int i = 0; i < count; i++)
	{
		infoList->DeleteString(0);
	}
	infoList->UpdateData(false);
	for (int i = 0; i < r.size() && r[i].second!=0; i++) {
		infoList->AddString(r[i].first);
	}

	if (infoList->GetCount() == 1) {
		CString text;
		infoList->GetText(0, text);
		CString empty("");
		SetDlgItemText(IDC_END_EDIT, empty);
		SetDlgItemText(IDC_END_EDIT, text);
	}
	return;
}

void searchDialog::OnLbnSetfocusInfolist1()
{
	// TODO: Add your control notification handler code here
	// AfxMessageBox(L"focus list");

	return;
}

void searchDialog::OnEnSetfocusStartEdit()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);
}


void searchDialog::OnEnSetfocusEndEdit()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
}


void searchDialog::OnEnKillfocusStartEdit()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
}


void searchDialog::OnEnKillfocusEndEdit()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);
}


void searchDialog::OnLbnSelchangeInfolist1()
{
	// TODO: Add your control notification handler code here
}


void searchDialog::OnLbnDblclkInfolist1()
{
	// TODO: Add your control notification handler code here
	CListBox* infoList = (CListBox*)CListBox::FromHandle(GetDlgItem(IDC_INFOLIST1)->GetSafeHwnd());
	int selected = infoList->GetCurSel();
	CString text;
	infoList->GetText(selected, text);
	SetDlgItemText(IDC_START_EDIT, text);

	return;
}


void searchDialog::OnLbnDblclkInfolist2()
{
	// TODO: Add your control notification handler code here
	CListBox* infoList = (CListBox*)CListBox::FromHandle(GetDlgItem(IDC_INFOLIST2)->GetSafeHwnd());
	int selected = infoList->GetCurSel();
	CString text;
	infoList->GetText(selected, text);
	SetDlgItemText(IDC_END_EDIT, text);

	return;
}


void searchDialog::OnBnSetfocusSearchButton()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
}


void searchDialog::OnBnSetfocusOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
}


void searchDialog::OnBnSetfocusCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFOLIST2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFOLIST1)->ShowWindow(SW_HIDE);
	AfxMessageBox(_T("hello"));
}
