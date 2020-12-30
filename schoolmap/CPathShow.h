#pragma once

#include "structs.h"
#include <vector>

// pathShow get pairs of start end points and line them up

class CPathShow : public CWnd
{
	DECLARE_DYNAMIC(CPathShow)

// signals
public:
	bool isShow = false;

// members
public:
	std::vector<int> pointSequence;

public:
	CPathShow();
	CPathShow(std::vector<int> pointSequence);
	virtual ~CPathShow();

public:

protected:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	// flick every time interval
	DECLARE_MESSAGE_MAP()
};


