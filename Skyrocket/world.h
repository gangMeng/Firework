#ifndef _WORLD_
#define _WORLD_

#define STARMESH 12
#define STARTEXSIZE 1024
#define MOONGLOWTEXSIZE 128
#define CLOUDMESH 70



class World
{
 public:

   World();

   
   ~World(){}

  void SetPara(int wind,int ambient,int stardensity,int moonglow,int moon,int clouds,int earth);

  void InitWorld(int wind,int ambient,int stardensity,int moonglow,int moon,int clouds,int earth);

  void update(float frameTime);//更新
	
  void draw();//绘制

  void makeHeights(int first, int last, int *h);//远山

   int dWind;
   int dAmbient;
   int dStardensity;
   int dMoonglow;
   int dMoon;
   int dClouds;
   int dEarth;

  
   unsigned char  starmap[STARTEXSIZE][STARTEXSIZE][3];


   int doSunset;
   float moonRotation, moonHeight;
   float cloudShift;
   float stars[STARMESH+1][STARMESH/2][6];  // 6 = x,y,z,u,v,bright
   float clouds[CLOUDMESH+1][CLOUDMESH+1][9];  // 9 = x,y,z,u,v,std bright,r,g,b
   
   //显示列表,纹理
   unsigned int starlist;
   unsigned int startex;

   unsigned int moonlist;
   unsigned int moontex;

   unsigned int moonglowlist;
   unsigned int moonglowtex;

   unsigned int cloudtex;
   
   unsigned int sunsettex;
   unsigned int sunsetlist;
   
   unsigned int earthneartex;
   unsigned int earthfartex;
   
   unsigned int earthlighttex;
   unsigned int earthlist;
   unsigned int earthnearlist;
   unsigned int earthfarlist;

};
#endif