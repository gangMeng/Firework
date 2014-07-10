#pragma once

#ifndef _INTERFACEMANAGER_
#include"../Interface/InterfaceManager.h"
#endif

#ifndef _SCENEMANAGER_
#include"../Skyrocket/SceneManager.h"
#endif

class CFireworkView : public CView
{
protected: // create from serialization only
	CFireworkView();
	DECLARE_DYNCREATE(CFireworkView)

// Attributes
public:

	InterfaceManager * m_Interface;//接口管理器
	SceneManager  m_SceneManager;//场景管理器


	CFireworkDoc* GetDocument() const;


   bool m_MouseControl;
   CPoint m_PrevPoint,m_CurPoint;//鼠标控制
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CFireworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
  afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
  afx_msg void OnMouseMove(UINT nFlags,CPoint point );
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
  afx_msg BOOL OnEraseBkgnd( CDC* pDC );
  afx_msg void OnSize(UINT nType,int cx,int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FireworkView.cpp
inline CFireworkDoc* CFireworkView::GetDocument() const
   { return reinterpret_cast<CFireworkDoc*>(m_pDocument); }
#endif

