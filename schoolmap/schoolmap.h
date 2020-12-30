
// schoolmap.h : main header file for the schoolmap application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CschoolmapApp:
// See schoolmap.cpp for the implementation of this class
//

class CschoolmapApp : public CWinApp
{
public:
	CschoolmapApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CschoolmapApp theApp;
