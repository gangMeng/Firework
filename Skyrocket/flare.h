#ifndef _FLARE_
#define _FLARE_

class Camera;

#include <gl/gl.h>
#include <gl/glu.h>


#define FLARESIZE 128

struct flareData
{
  float x, y, r, g, b, a;
};


class Flare
{
	
public:

  void initFlares();//��ʼ����ͷҫ��

  // ���ƾ�ͷҫ����Ļ����(x,y) (0,0) (1,1)֮��,alpha = 0.0 ~ 1.0 ��ʾǿ�� 
  void flare(Camera * camera,float x, float y, float red, float green, float blue, float alpha);

  unsigned char flare1[FLARESIZE][FLARESIZE][4];
  unsigned char flare2[FLARESIZE][FLARESIZE][4];
  unsigned char flare3[FLARESIZE][FLARESIZE][4];
  unsigned char flare4[FLARESIZE][FLARESIZE][4];

  unsigned int flaretex[4];//ҫ������

  unsigned int  flarelist[4];//ҫ����ʾ�б�

};
#endif  