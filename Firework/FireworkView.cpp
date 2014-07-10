// FireworkView.cpp : implementation of the CFireworkView class
//

#include "stdafx.h"
#include "Firework.h"

#include "FireworkDoc.h"
#include "FireworkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFireworkView

IMPLEMENT_DYNCREATE(CFireworkView, CView)

BEGIN_MESSAGE_MAP(CFireworkView, CView)

 ON_WM_KEYDOWN()
 ON_WM_MOUSEMOVE()
 ON_WM_ERASEBKGND()
 ON_WM_SIZE()
 ON_WM_CREATE()
 ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CFireworkView construction/destruction

CFireworkView::CFireworkView()
{
	// TODO: add construction code here
  m_PrevPoint = CPoint(0,0);
  m_CurPoint = CPoint(0,0);
  m_MouseControl = false;
}

CFireworkView::~CFireworkView()
{
}

BOOL CFireworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFireworkView drawing

void CFireworkView::OnDraw(CDC* /*pDC*/)
{
	CFireworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CFireworkView::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
  switch(nChar)
  {
   case 'w':
   case 'W':
	  m_SceneManager.m_Camera.Forword();//前进

	  break;
   case 's':
   case 'S':
	  m_SceneManager.m_Camera.Backword();//后退
	  break;

   case 'a':
   case 'A':
	  m_SceneManager.m_Camera.Left();//向左
	  break;

   case 'd':
   case 'D':
	  m_SceneManager.m_Camera.Right();//向右
	  break;
   case VK_ESCAPE:
      ShowCursor(true);
	  m_MouseControl = false;

	  break;
   case VK_SPACE:
	   m_SceneManager.CheckStop();
	   break;
   case 'u':
   case 'U':
	   m_SceneManager.SpeedUpCamera();
	   break;
   case 'i':
   case 'I':
       m_SceneManager.SpeedDownCamera();
	   break;

  }


}
void CFireworkView::OnMouseMove(UINT nFlags,CPoint point )
{
 
  if(!m_MouseControl) return;//未进入鼠标控制状态

  RECT rect;
  GetClientRect(&rect);
   
  CPoint screenPoint;
  m_CurPoint = point;

  //光标范围控制
  if(m_CurPoint.x>rect.right-rect.right/4)  
  {
    m_CurPoint.x =rect.right/4;
	screenPoint = m_CurPoint;
    ClientToScreen(&screenPoint);
	SetCursorPos(screenPoint.x,screenPoint.y);
	m_PrevPoint = m_CurPoint;
	return;
  }
  if(m_CurPoint.x<rect.right/4)
  {
    m_CurPoint.x = rect.right-rect.right/4;
	screenPoint = m_CurPoint;
    ClientToScreen(&screenPoint);
	SetCursorPos(screenPoint.x,screenPoint.y);
	m_PrevPoint = m_CurPoint;
	return;
  } 
  if(m_CurPoint.y>rect.bottom-rect.bottom/4)
  {
    m_CurPoint.y =rect.bottom/4;
	screenPoint = m_CurPoint;
    ClientToScreen(&screenPoint);
	SetCursorPos(screenPoint.x,screenPoint.y);
	m_PrevPoint = m_CurPoint;
	return;
  }
  if(m_CurPoint.y<rect.bottom/4)
  {
    m_CurPoint.y = rect.bottom-rect.bottom/4;
	screenPoint = m_CurPoint;
    ClientToScreen(&screenPoint);
	SetCursorPos(screenPoint.x,screenPoint.y);
	m_PrevPoint = m_CurPoint;
	return;

  }

  //鼠标控制照相机
  static float heading = 0.0;
  static float pitch = 0.0;

  heading -= ((float)m_CurPoint.x - (float)m_PrevPoint.x)/10;
  pitch   -=  ((float)m_CurPoint.y - (float)m_PrevPoint.y)/10;
	
  m_SceneManager.m_Camera.Heading(heading);//转向
  m_SceneManager.m_Camera.Pitch(pitch);
		
	
  m_PrevPoint = m_CurPoint;
}
void CFireworkView::OnLButtonDown(UINT nFlags,CPoint point)
{
  if(!m_MouseControl)//进入鼠标控制
  {
    RECT rect;
	CPoint screenPoint;
    GetClientRect(&rect);
   ShowCursor(false);
   m_MouseControl = true;
   m_PrevPoint.x = m_CurPoint.x = rect.right/2;//位置初始化
   m_PrevPoint.y = m_CurPoint.y = rect.bottom/2;

   screenPoint = m_CurPoint;
   ClientToScreen(&screenPoint);
   SetCursorPos(screenPoint.x,screenPoint.y);

  }
 }

BOOL CFireworkView::OnEraseBkgnd( CDC* pDC )
{
  return FALSE;
}
void CFireworkView::OnSize(UINT nType,int cx,int cy)
{
  CView::OnSize(nType, cx, cy);
 
  RECT rect;

  GetClientRect(&rect);

  m_SceneManager.Reset(rect);//重置场景
}
int CFireworkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 if(CView::OnCreate(lpCreateStruct) == -1)
		return -1;

 RECT rect;
 GetClientRect( &rect);

 m_SceneManager.CreateDevice(m_hWnd,rect);

	//初始化接口管理器
	m_Interface = &theApp.m_Interface;

	m_Interface->SetScene(&m_SceneManager);

	m_Interface->m_Wnd = this;
	return 0;
}
// CFireworkView diagnostics

#ifdef _DEBUG
void CFireworkView::AssertValid() const
{
   CView::AssertValid();
}

void CFireworkView::Dump(CDumpContext& dc) const
{
   CView::Dump(dc);
}

CFireworkDoc* CFireworkView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFireworkDoc)));
	return (CFireworkDoc*)m_pDocument;
}
#endif //_DEBUG


// CFireworkView message handlers



