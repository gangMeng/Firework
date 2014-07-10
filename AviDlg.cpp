// AviDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Firework.h"
#include "AviDlg.h"
#include"AviGenerator.h"
#include"FireworkDoc.h"
#include"FireworkView.h"
// AviDlg dialog

IMPLEMENT_DYNAMIC(AviDlg, CDialog)

AviDlg::AviDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AviDlg::IDD, pParent)
	, m_Fps(15)
	, m_Time(120)
	, m_FileName(_T("Output"))
{
	m_Interface = &theApp.m_Interface;
	m_Interface->GetSceneManager()->m_Stop = true;
}

AviDlg::~AviDlg()
{
}

void AviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AVI_FPS, m_Fps);
	DDX_Text(pDX, IDC_AVI_TIME, m_Time);
	DDX_Text(pDX, IDC_AVI_FILENAME, m_FileName);
}


BEGIN_MESSAGE_MAP(AviDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void AviDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(true);
	UpdateData(false);

	RECT rect;
	m_Interface->m_Wnd->GetWindowRect(&rect);
	m_Interface->m_Wnd->SetWindowPos(NULL,0,0,640,480,0);
	OnAvigenerationGenerate();

	m_Interface->m_Wnd->SetWindowPos(NULL,0,0,rect.right,rect.bottom,0);
	OnOK();
}
void AviDlg::OnAvigenerationGenerate(void)
{
	UINT i,nFrames;
	nFrames = m_Time*m_Fps;
	CAVIGenerator AviGen;
	LPBITMAPINFOHEADER lpbih;
	BYTE* bmBits;	
	CFireworkView* pView=(CFireworkView *) m_Interface->m_Wnd;
	HRESULT hr;

	m_Interface->GetSceneManager()->SetFPS(m_Fps);//设置帧率
	m_Interface->GetSceneManager()->SetRecord();
	BeginWaitCursor();
	
	// set 15fps
	AviGen.SetRate(m_Fps);

	// give info about bitmap
	AviGen.SetBitmapHeader(pView);		

	// set filename, extension ".avi" is appended if necessary
	AviGen.SetFileName(m_FileName);

	// retreiving size of image
	lpbih=AviGen.GetBitmapHeader();

	// allocating memory
	bmBits=new BYTE[lpbih->biSizeImage];

	hr=AviGen.InitEngine();
	if (FAILED(hr))
	{
		AfxMessageBox( AviGen.GetLastErrorMessage());
		goto Cleaning;
	}

	m_Interface->GetSceneManager()->m_Stop = false;
	// reading back buffer
	glReadBuffer(GL_BACK);

	for(i=0;i<nFrames;i++)
	{
		m_Interface->GetSceneManager()->SetFrame(i);//设置第几侦
		m_Interface->GetSceneManager()->FrameMove();
		// render frame
		m_Interface->GetSceneManager()->Render();
		// Copy from OpenGL to buffer
		glReadPixels(0,0,lpbih->biWidth,lpbih->biHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmBits); 
		// send to avi engine
		hr=AviGen.AddFrame(bmBits);
		if (FAILED(hr))
		{
			AfxMessageBox( AviGen.GetLastErrorMessage());
			goto Cleaning;
		}
		
		CClientDC dc(pView);
		CRect * rect = new CRect();
		pView->GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(154,181,252));
		SetTextAlign(dc,TA_CENTER);
		dc.TextOut(rect->Width()/2,rect->Height()/2,_T("Generating movie,please wait for a moment."));
	}

Cleaning:
	// releasing engine and memory
	AviGen.ReleaseEngine();
	delete[] bmBits;

	glReadBuffer(GL_FRONT);
	

	
	m_Interface->GetSceneManager()->ClearRecord();
	EndWaitCursor();
}


// AviDlg message handlers
