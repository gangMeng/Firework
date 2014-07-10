// Firework.h : main header file for the Firework application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#ifndef _INTERFACEMANAGER_
#include"../Interface/InterfaceManager.h"
#endif

// CFireworkApp:
// See Firework.cpp for the implementation of this class
//

class CFireworkApp : public CWinApp
{
public:
	CFireworkApp();

	InterfaceManager m_Interface;//�ӿڹ�����

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFireworkApp theApp;