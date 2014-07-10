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

  float smokeTime[SMOKETIMES];  // 连续烟雾粒子得生命期
  
  int whichSmoke[WHICHSMOKES];  //指示产生烟雾粒子的粒子

  unsigned int smokelist[5];//烟雾显示列表

  unsigned int  smoketex[5];//烟雾纹理
  
};


#endif  