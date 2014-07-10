#ifndef _SCENEMANAGER_
#define _SCENEMANAGER_

#include "rsMath.h"

#include<vector>

#include "../Firework/Resource.h"

#ifndef _CONSTRAINEDFIRE_
#include"../Fire/ConstrainedFire.h"
#endif

#ifndef _PARTICLESYSTEM_
#include"ParticleSystem.h"
#endif

#ifndef _WORLD_
#include "world.h"
#endif

#ifndef _CAMERA_
#include"Camera.h"
#endif

#ifndef _TIMER_
#include "Timer.h"
#endif

#ifndef _FLARE_
#include "Flare.h"//ҫ��
#endif

#ifndef _SMOKE_
#include"Smoke.h"//����
#endif

#ifndef _TERRAIN_
#include"Terrain.h"
#endif

#define ZOOMROCKETINACTIVE 1000000000

#ifndef _OBJREADER_
#include"../Fire/ObjReader.h"
#endif




class SceneManager
{
 public:

	 std::vector<int> m_PlayTime;
	 std::vector<int> m_PlayStyle;
	 std::vector<rsVec> m_PlayPos;
	 std::vector<rsVec> m_PlaySource;


    SceneManager(void);
	
	~SceneManager();

	void CreateDevice(HWND hWnd,RECT rect);//�����豸

	void Render();//���Ƴ���
	
	void FrameMove();//��������

	void InitScene(HWND hwnd);//��ʼ������

	void Reset(RECT rect);//���ó���

	void Clear(HWND hwnd);//�������

	void SetBestPixelFormat(HDC hdc);

	void makeFlareList();

	void SetMaxRockets(int d);
	void SetStarDensity(int d);
	void SetSmoke(int d);
	void SetFlare(int d);
	void SetExplosionSmoke(int d);
	void SetMoonGlow(int d);
	void SetWind(int d);
	void SetAmbient(int d);
	void SetFrameRate(int d);
	void CheckMoon();
	void CheckClouds();
	void CheckEarth();
	void CheckIllumination();
  
	void CheckStop();//��ͣ

	void BeginAutoDrive();

	void SpeedUpCamera();

    void SpeedDownCamera();

	bool IsRecord(){return m_IsRecord;}

    void SetRecord(){m_IsRecord = true;}
  
	void ClearRecord(){m_IsRecord = false;}
  
	bool m_IsRecord;

  
	void SetFrame(int frame){m_Frame = frame;}

 
	int m_Frame;

	float m_f;

	bool m_Stop;
	//�豸
    HDC hdc;

    HGLRC hglrc;

	int m_Fps;//֡��

	void SetFPS(int fps);
    
	//����
	Camera m_Camera;//�����

	Timer m_Timer;//������ʱ��

    World theWorld;//����

	Flare m_Flare;//��ͷҫ��

	Smoke m_Smoke;//����

	ParticleSystem m_ParticleSystem;//����ϵͳ

	Terrain m_Terrain;//����

	ConstrainedFire  m_CFire[16];//��Լ���̻�

	
	ObjReader m_ObjReader;

   
	//��������
	float billboardMat[16];// transformation needed for rendering particles
	float smokeTime[SMOKETIMES];  // lifespans of consecutive smoke particles

    int whichSmoke[WHICHSMOKES];  // table to indicate which particles produce smoke
	float frameTime;
    int numFlares ;
    int numRockets ;
   
    std::vector<flareData> lensFlares;
    
    int dMaxrockets;
    int dStardensity;
    int dFlare;
    int dMoonglow;
    int dSmoke;
    int dExplosionsmoke;
    int dWind;
	int dAmbient;
    unsigned int dFrameRateLimit;

	int dClouds;
    int dIllumination;
    int dMoon;
    int dEarth;
   
   
 
    //����״̬����
    int kFireworks;
    
   
    bool kSlowMotion;
    int userDefinedExplosion ;

   
};
#endif