#pragma once


// searchDialog dialog

class searchDialog : public CDialogEx
{
	DECLARE_DYNAMIC(searchDialog)

public:
	searchDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~searchDialog();
	virtual void onSearchButtonClicked();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeStartEdit();
	afx_msg void OnEnChangeEndEdit();
	afx_msg void OnLbnSetfocusInfolist1();
	afx_msg void OnEnSetfocusStartEdit();
	afx_msg void OnEnSetfocusEndEdit();
	afx_msg void OnEnKillfocusStartEdit();
	afx_msg void OnEnKillfocusEndEdit();
	afx_msg void OnLbnSelchangeInfolist1();
	afx_msg void OnLbnDblclkInfolist1();
	afx_msg void OnLbnDblclkInfolist2();
	afx_msg void OnBnSetfocusSearchButton();
	afx_msg void OnBnSetfocusOk();
	afx_msg void OnBnSetfocusCancel();
};
