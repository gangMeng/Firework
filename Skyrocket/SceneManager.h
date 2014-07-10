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
#include "Flare.h"//耀斑
#endif

#ifndef _SMOKE_
#include"Smoke.h"//烟雾
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

	void CreateDevice(HWND hWnd,RECT rect);//创建设备

	void Render();//绘制场景
	
	void FrameMove();//动画函数

	void InitScene(HWND hwnd);//初始化场景

	void Reset(RECT rect);//重置场景

	void Clear(HWND hwnd);//清除场景

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
  
	void CheckStop();//暂停

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
	//设备
    HDC hdc;

    HGLRC hglrc;

	int m_Fps;//帧率

	void SetFPS(int fps);
    
	//场景
	Camera m_Camera;//照像机

	Timer m_Timer;//场景计时器

    World theWorld;//世界

	Flare m_Flare;//镜头耀斑

	Smoke m_Smoke;//烟雾

	ParticleSystem m_ParticleSystem;//粒子系统

	Terrain m_Terrain;//地形

	ConstrainedFire  m_CFire[16];//带约束烟花

	
	ObjReader m_ObjReader;

   
	//场景参数
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
   
   
 
    //场景状态控制
    int kFireworks;
    
   
    bool kSlowMotion;
    int userDefinedExplosion ;

   
};
#endif