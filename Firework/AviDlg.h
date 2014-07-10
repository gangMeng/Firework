#pragma once


// AviDlg dialog

#ifndef _INTERFACEMANAGER_
#include"../Interface/InterfaceManager.h"
#endif

class AviDlg : public CDialog
{
	DECLARE_DYNAMIC(AviDlg)

public:

	InterfaceManager * m_Interface;

    void OnAvigenerationGenerate(void);
	AviDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AviDlg();

// Dialog Data
	enum { IDD = IDD_AVI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
public:
	long m_Fps;
	int m_Time;
	CString m_FileName;
};
