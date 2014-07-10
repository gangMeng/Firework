#ifndef _PARTICLE_
#define _PARTICLE_

#include "rsMath.h"

#ifndef _FLARE_
#include "flare.h"//光晕
#endif
#ifndef _SMOKE_
#include "smoke.h"//烟雾
#endif

#ifndef _SHOCKWAVE_
#include "shockwave.h"//冲击波
#endif

#ifndef _WORLD_
#include "world.h"//世界
#endif

#include"../Sound/SoundEngine.h"

extern SoundEngine * soundengine;

#ifndef _CONSTRAINEDFIRE_
#include"../Fire/ConstrainedFire.h"
#endif

#define PI 3.14159265359f
#define PIx2 6.28318530718f
#define D2R 0.0174532925f
#define R2D 57.2957795131f

//粒子类型
#define ROCKET 0//火箭
#define FOUNTAIN 1//泉
#define SPINNER 2//纺锤
#define SMOKE 3//烟雾
#define EXPLOSION 4//爆炸
#define STAR 5//星星
#define STREAMER 6//极光
#define METEOR 7//流星
#define POPPER 8//爆竹
#define BEE 9//蜜蜂
#define SUCKER 10
#define SHOCKWAVE 11//冲击波
#define STRETCHER 12
#define BIGMAMA 13
#define SHAPE 14//约束粒子
#define SHAPED 15 //形状保持
#define IGNITOR 16  //点燃器


//爆炸类型
/*
#define NORMALSPHERE    0 //球状扩散
#define SPLITSPHERE     1
#define MULTCOLORSPHERE 2
#define RING            3
#define DOUBLESPHERE    4   
#define SPHERERING      5
#define STREAMERSPHERE  8
#define METEORSPHERE    7
#define SUCKER          
#define STARPOPPER
#define SMALLSPHERELARGEMETEOR 9
#define STREAMERPOPPER
#define METEORPOPPER
#define LITTLEPOPPER
#define BEE
#define STRETCHER

#define CONSTRAINEDFIRE //形状约束类型
*/

class SceneManager;//场景

class ParticleSystem;//粒子系统

class Particle
{
public:

  Particle();

  ~Particle(){};

  static ParticleSystem * m_ParticleSystem;//粒子系统
 
  static SceneManager * m_Scene;

  void randomColor(rsVec& color);// 选择颜色
	 
  void findDepth(rsVec cameraPos,float * billboardMat);//查询深度
	
  void update(float frameTime,int dSmoke,int dClouds,int dWind,int dIllumination,int dAmbient,World* theWorld);//更新粒子
		
  void draw(float * billboardMat,World * theWorld);//绘制单个粒子

  //粒子操作函数
  void PullParticle(Particle* suck,float frameTime);
    
  void PushParticle(Particle* shock,float frameTime);

  void StretchParticle(Particle* stretch,float frameTime);//垂直拉伸（x,z吸收，y汇出）

  void Illuminate(Particle* ill,int dClouds,World * theWorld);//火箭，爆炸照明烟雾,爆炸照明云彩


   // 初始化粒子
  void initStar();
  void initStreamer();
  void initMeteor();
  void initStarPopper();
  void initStreamerPopper();
  void initMeteorPopper();
  void initLittlePopper();
  void initBee();
  void initRocket();
  void initFountain();
  void initSpinner();
  void initSmoke(rsVec pos, rsVec speed,int dAmbient);
  void initSucker(); // rare easter egg explosion which is immediately followed by...
  void initShockwave(); // a freakin' huge explosion
  void initStretcher(); // another rare explosion followed by...
  void initBigmama(); // this other massive bomb
  void initExplosion();
	
  // 爆炸中产生新粒子
  void popShape(int numParts,float v0,rsVec color,ConstrainedFire * fire);
  void popSphere(int numParts, float v0, rsVec color);
  void popSplitSphere(int numParts, float v0, rsVec color1);
  void popMultiColorSphere(int numParts, float v0);
  void popRing(int numParts, float v0, rsVec color);
  void popStreamers(int numParts, float v0, rsVec color);
  void popMeteors(int numParts, float v0, rsVec color);
  void popIgnitor(ConstrainedFire * fire,rsVec color);
  void popStarPoppers(int numParts, float v0, rsVec color);
  void popStreamerPoppers(int numParts, float v0, rsVec color);
  void popMeteorPoppers(int numParts, float v0, rsVec color);
  void popLittlePoppers(int numParts, float v0);
  void popBees(int numParts, float v0, rsVec color);


  //粒子标识
  unsigned int type; //粒子类型
  int explosiontype; // 爆炸类型 
  
  unsigned int displayList; //绘制方法

  ConstrainedFire * m_Fire;//形状约束烟花

  rsVec xyz; //当前位置
  rsVec lastxyz; //上一位置
  rsVec vel; // 速度矢量
  rsVec rgb; //粒子颜色
	
  float drag; // 空气阻力
  float t; //粒子生存时间
  float tr; //剩余生存时间
  float bright; // 亮度
  float life; // life remaining (usually defined from 0.0 to 1.0)
  float size; //显示缩放


  // 火箭参数
  float thrust; //动力
  float endthrust; // point in rockets life at which to stop thrusting
  float spin, tilt; //侧旋参数
  rsVec tiltvec; // 倾斜
  int makeSmoke; //是否产生烟雾
  int smokeTimeIndex; //烟雾索引
  float smokeTrailLength; // 烟尾长度
  float sparkTrailLength; //流长度
  
  float depth;//深度

  //形状控制


  bool	controlled;					//形状受控的粒子
  Point3f C;				//形状受控粒子的速度计算常数。
  float     k;		//空气摩擦的系数
  Point3f		force;						//上升阶段的作用力
  float	deltAlpha1,deltAlpha2;		//透明度值由小到大，再变小的渐变值


  Particle* thisParticle(){return this;}

	// operators used by stl list sorting
  friend bool operator < (const Particle &p1, const Particle &p2){return(p2.depth < p1.depth);}
  friend bool operator > (const Particle &p1, const Particle &p2){return(p2.depth > p1.depth);}
  friend bool operator == (const Particle &p1, const Particle &p2){return(p1.depth == p2.depth);}
  friend bool operator != (const Particle &p1, const Particle &p2){return(p1.depth != p2.depth);}
};



#endif