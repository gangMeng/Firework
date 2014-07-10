// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Firework.h"

#include "MainFrm.h"

#include"avidlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<stdio.h>
#include<mmsystem.h>
#include"FireWorkDlg.h"
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	
	ON_WM_CREATE()
	ON_COMMAND(ID_RESET,OnReset)
	ON_COMMAND(ID_SLOW,OnSlow)
	ON_COMMAND(ID_STOP,OnStop)
	ON_COMMAND(ID_SCENE_PARA,OnScenePara)
	ON_COMMAND(ID_FIREWORK_PARA,OnFireWorkPara)
	ON_COMMAND(ID_FULL_SCREEN,OnFullScreen)
	ON_COMMAND(ID_CHANGE_VIEW,OnChangeView)
	ON_COMMAND(ID_STOP_CAMERA,OnStopCamera)
	ON_COMMAND(ID_AUTO_CAMERA,OnAutoCamera)
	ON_COMMAND(ID_USER_CAMERA,OnUserCamera)

	ON_COMMAND(ID_SAVE_SOUND,OnSaveSound)
	ON_COMMAND(ID_PLAY_SOUND,OnPlaySound)

	ON_COMMAND(ID_AVI,OnAvi)
	

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_Interface = &theApp.m_Interface;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


void CMainFrame::OnReset()
{
}
void CMainFrame::OnSlow()
{
}
void CMainFrame::OnStop()
{
 m_Interface->GetSceneManager()->CheckStop();
}
void CMainFrame::OnScenePara()
{
	m_Config.DoModal();//显示场景参数对话框
}
void CMainFrame::OnFireWorkPara()
{
 

}
void CMainFrame::OnFullScreen()
{
}
void CMainFrame::OnChangeView()
{
}
void CMainFrame::OnStopCamera()
{
}
void CMainFrame::OnAutoCamera()
{
	m_Interface->GetSceneManager()->BeginAutoDrive();
}
void CMainFrame::OnUserCamera()
{
}
void CMainFrame::OnAvi()
{
  AviDlg *pDlg = new AviDlg(this);

  pDlg->Create(IDD_AVI);

  pDlg->ShowWindow(SW_RESTORE);
}
void CMainFrame::OnSaveSound()
{
 int end = m_Interface->GetSceneManager()->m_PlayTime.size();

  FILE * fp = fopen("c:\\声音.txt","w");

  fprintf(fp,"以下是声音：          第几侦             播放声音类型\n");
  SceneManager * manager;
  manager = m_Interface->GetSceneManager();
  for(int i=0;i<end;i++)
  {
   fprintf(fp,"%d     %d \n",manager->m_PlayTime[i],manager->m_PlayStyle[i]);

  }

  fclose(fp);

}
void CMainFrame::OnPlaySound()
{
  int end = m_Interface->GetSceneManager()->m_PlayTime.size();

  DWORD begin, now;

  m_Interface->GetSceneManager()->m_Stop = true;

  SceneManager * manager = m_Interface->GetSceneManager();


  float listenerOri[6];
   listenerOri[0] = float(-manager->m_Camera.modelMat[2]);
   listenerOri[1] = float(-manager->m_Camera.modelMat[6]);
   listenerOri[2] = float(-manager->m_Camera.modelMat[10]);
   listenerOri[3] = float(manager->m_Camera.modelMat[1]);
   listenerOri[4] = float(manager->m_Camera.modelMat[5]);
   listenerOri[5] = float(manager->m_Camera.modelMat[9]);
   float v[3]={0,0,0};
   float pos[3] = {0,700,1300};

  begin = timeGetTime();

  for(int i=0;i<end;i++)
  {
   now = timeGetTime() - begin;
   while(now<manager->m_PlayTime[i])
   {
    now = timeGetTime() - begin;
	soundengine->update(pos, v, listenerOri, manager->m_f/1000, false);
   }

   //播放声音
   soundengine->insertSoundNode(manager->m_PlayStyle[i], manager->m_PlaySource[i], manager->m_PlayPos[i]);

   
  
   
   

  }




   MessageBoxA("OVER","OVER",MB_OK);
   m_Interface->GetSceneManager()->m_Stop = false;





}
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers



