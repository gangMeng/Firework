// FireWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Firework.h"
#include "FireWorkDlg.h"


// FireWorkDlg dialog

extern int m_K;
IMPLEMENT_DYNAMIC(FireWorkDlg, CDialog)

FireWorkDlg::FireWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FireWorkDlg::IDD, pParent)
	, m_Kvalue(1)
	, safasg(0)
{

}

FireWorkDlg::~FireWorkDlg()
{
}

void FireWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Kvalue);
	DDV_MinMaxInt(pDX, m_Kvalue, 1, 50);
}

void FireWorkDlg::OnOK()
{
  if(m_Kvalue>=1&&m_Kvalue<=50)
  m_K = m_Kvalue;
  CDialog::OnOK(); 

}

BEGIN_MESSAGE_MAP(FireWorkDlg, CDialog)
END_MESSAGE_MAP()


// FireWorkDlg message handlers
