#include "stdafx.h"

#include "InterfaceManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

InterfaceManager::InterfaceManager()
{
  m_SceneManager = NULL;
  m_IsAnimate = false;
 
}
void InterfaceManager::Initialize()//��������ܽӿ�
{
   
}
void InterfaceManager::Destroy()
{
  
}
void InterfaceManager::SetScene(SceneManager * scene)
{
 
   m_SceneManager =  scene;
 
}

