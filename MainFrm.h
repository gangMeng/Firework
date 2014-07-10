// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#ifndef _INTERFACEMANAGER_
#include"../Interface/InterfaceManager.h"
#endif

#ifndef _CONFIG_
#include "Config.h"
#endif
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	InterfaceManager * m_Interface;//接口管理器
	Config m_Config;
// Operations
public:


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnReset();
    afx_msg void OnSlow();
    afx_msg void OnStop();
    afx_msg void OnScenePara();
    afx_msg void OnFireWorkPara();
    afx_msg void OnFullScreen();
    afx_msg void OnChangeView();
    afx_msg void OnStopCamera();
    afx_msg void OnAutoCamera();
    afx_msg void OnUserCamera();
	afx_msg void OnPlaySound();
	afx_msg void OnSaveSound();

	afx_msg void OnAvi();

 


	DECLARE_MESSAGE_MAP()
};


