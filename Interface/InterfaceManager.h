#ifndef _INTERFACEMANAGER_
#define _INTERFACEMANAGER_

#ifndef _SCENEMANAGER_
#include"../Skyrocket/SceneManager.h"
#endif

#ifndef _PARTICLE_
#include"../Skyrocket/Particle.h"
#endif



class InterfaceManager
{
public:

  bool m_IsAnimate;//是否进行动画

  InterfaceManager();
 
  void Initialize();//初始化接口
  
  void Destroy();//销毁接口功能

  SceneManager * GetSceneManager(){return m_SceneManager;}
 
  void SetScene(SceneManager * scene);

  SceneManager * m_SceneManager;//场景管理器指针
 
  CWnd * m_Wnd;

 
};

#endif