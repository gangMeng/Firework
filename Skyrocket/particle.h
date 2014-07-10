#ifndef _PARTICLE_
#define _PARTICLE_

#include "rsMath.h"

#ifndef _FLARE_
#include "flare.h"//����
#endif
#ifndef _SMOKE_
#include "smoke.h"//����
#endif

#ifndef _SHOCKWAVE_
#include "shockwave.h"//�����
#endif

#ifndef _WORLD_
#include "world.h"//����
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

//��������
#define ROCKET 0//���
#define FOUNTAIN 1//Ȫ
#define SPINNER 2//�Ĵ�
#define SMOKE 3//����
#define EXPLOSION 4//��ը
#define STAR 5//����
#define STREAMER 6//����
#define METEOR 7//����
#define POPPER 8//����
#define BEE 9//�۷�
#define SUCKER 10
#define SHOCKWAVE 11//�����
#define STRETCHER 12
#define BIGMAMA 13
#define SHAPE 14//Լ������
#define SHAPED 15 //��״����
#define IGNITOR 16  //��ȼ��


//��ը����
/*
#define NORMALSPHERE    0 //��״��ɢ
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

#define CONSTRAINEDFIRE //��״Լ������
*/

class SceneManager;//����

class ParticleSystem;//����ϵͳ

class Particle
{
public:

  Particle();

  ~Particle(){};

  static ParticleSystem * m_ParticleSystem;//����ϵͳ
 
  static SceneManager * m_Scene;

  void randomColor(rsVec& color);// ѡ����ɫ
	 
  void findDepth(rsVec cameraPos,float * billboardMat);//��ѯ���
	
  void update(float frameTime,int dSmoke,int dClouds,int dWind,int dIllumination,int dAmbient,World* theWorld);//��������
		
  void draw(float * billboardMat,World * theWorld);//���Ƶ�������

  //���Ӳ�������
  void PullParticle(Particle* suck,float frameTime);
    
  void PushParticle(Particle* shock,float frameTime);

  void StretchParticle(Particle* stretch,float frameTime);//��ֱ���죨x,z���գ�y�����

  void Illuminate(Particle* ill,int dClouds,World * theWorld);//�������ը��������,��ը�����Ʋ�


   // ��ʼ������
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
	
  // ��ը�в���������
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


  //���ӱ�ʶ
  unsigned int type; //��������
  int explosiontype; // ��ը���� 
  
  unsigned int displayList; //���Ʒ���

  ConstrainedFire * m_Fire;//��״Լ���̻�

  rsVec xyz; //��ǰλ��
  rsVec lastxyz; //��һλ��
  rsVec vel; // �ٶ�ʸ��
  rsVec rgb; //������ɫ
	
  float drag; // ��������
  float t; //��������ʱ��
  float tr; //ʣ������ʱ��
  float bright; // ����
  float life; // life remaining (usually defined from 0.0 to 1.0)
  float size; //��ʾ����


  // �������
  float thrust; //����
  float endthrust; // point in rockets life at which to stop thrusting
  float spin, tilt; //��������
  rsVec tiltvec; // ��б
  int makeSmoke; //�Ƿ��������
  int smokeTimeIndex; //��������
  float smokeTrailLength; // ��β����
  float sparkTrailLength; //������
  
  float depth;//���

  //��״����


  bool	controlled;					//��״�ܿص�����
  Point3f C;				//��״�ܿ����ӵ��ٶȼ��㳣����
  float     k;		//����Ħ����ϵ��
  Point3f		force;						//�����׶ε�������
  float	deltAlpha1,deltAlpha2;		//͸����ֵ��С�����ٱ�С�Ľ���ֵ


  Particle* thisParticle(){return this;}

	// operators used by stl list sorting
  friend bool operator < (const Particle &p1, const Particle &p2){return(p2.depth < p1.depth);}
  friend bool operator > (const Particle &p1, const Particle &p2){return(p2.depth > p1.depth);}
  friend bool operator == (const Particle &p1, const Particle &p2){return(p1.depth == p2.depth);}
  friend bool operator != (const Particle &p1, const Particle &p2){return(p1.depth != p2.depth);}
};



#endif