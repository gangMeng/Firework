// onfig.cpp : implementation file
//

#include "stdafx.h"
#include "Firework.h"
#include "onfig.h"


// Config dialog

IMPLEMENT_DYNAMIC(Config, CDialog)

Config::Config(CWnd* pParent /*=NULL*/)
	: CDialog(Config::IDD, pParent)
{

}

Config::~Config()
{
}

void Config::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Config, CDialog)
END_MESSAGE_MAP()


// Config message handlers
