#pragma once


// FireWorkDlg dialog

class FireWorkDlg : public CDialog
{
	DECLARE_DYNAMIC(FireWorkDlg)

public:
	FireWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FireWorkDlg();

// Dialog Data
	enum { IDD = IDD__FIREWORK_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	int m_Kvalue;
	int safasg;
};
