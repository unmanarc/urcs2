// installer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CinstallerApp:
// See installer.cpp for the implementation of this class
//

class CinstallerApp : public CWinApp
{
public:
	CinstallerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CinstallerApp theApp;