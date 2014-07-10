#ifndef _SMOKE_
#define _SMOKE_

#include <gl/gl.h>
#include <gl/glu.h>

#define SMOKETIMES 8

#define WHICHSMOKES 100

class SceneManager;

class Smoke
{
 public:

  void initSmoke(int smoke,int explosion);

  int m_Smoke;
  
  int m_Explosionsmoke;

  float smokeTime[SMOKETIMES];  // �����������ӵ�������
  
  int whichSmoke[WHICHSMOKES];  //ָʾ�����������ӵ�����

  unsigned int smokelist[5];//������ʾ�б�

  unsigned int  smoketex[5];//��������
  
};


#endif  