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

  bool m_IsAnimate;//�Ƿ���ж���

  InterfaceManager();
 
  void Initialize();//��ʼ���ӿ�
  
  void Destroy();//���ٽӿڹ���

  SceneManager * GetSceneManager(){return m_SceneManager;}
 
  void SetScene(SceneManager * scene);

  SceneManager * m_SceneManager;//����������ָ��
 
  CWnd * m_Wnd;

 
};

#endif