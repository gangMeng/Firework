#include "stdafx.h"
#include "SceneManager.h"
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SceneManager::SceneManager()
{ 
   frameTime = 0.0f;
   numFlares = 0;
   numRockets = 0;

   m_f = 0.0;

   //控制参数初始化
   kFireworks = 1;
    
   m_Stop = false;
   kSlowMotion = false;
   userDefinedExplosion = -1;

   //场景参数初始化
   dMaxrockets = 1;
   dStardensity = 38;
   dFlare = 10;
   dMoonglow = 85;
   dSmoke = 2;
   dExplosionsmoke = 0;
   dWind = 30;
   dAmbient = 8;
   dFrameRateLimit = 0;
   dClouds = 10;
   dIllumination = 1;
   dMoon = 1;
   dEarth = 1;
 
   m_Fps = 0;

   Particle::m_Scene = this;  

   //初始化烟花
   m_ObjReader.LoadObj("bunny.obj");   //formica rufa.obj  bunny.obj bird2.obj
   m_CFire[0].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("CASA2009.obj");
   m_CFire[1].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("head.obj");
   m_CFire[3].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("dinosaur.obj");
   m_CFire[4].Init(&m_ObjReader);
   m_ObjReader.Release();
    m_ObjReader.LoadObj("hands.obj");
   m_CFire[6].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("venus.obj");
   m_CFire[7].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("lion.obj");
   m_CFire[9].Init(&m_ObjReader);
   m_ObjReader.Release();
   m_ObjReader.LoadObj("dragon.obj");
   m_CFire[10].Init(&m_ObjReader);
   m_ObjReader.Release();

   m_IsRecord = false;
   m_Fps = 0;
   m_Frame = 0;

   m_ParticleSystem.particles.resize(200000);
}
SceneManager::~SceneManager()
{
 
}
void SceneManager::CreateDevice(HWND hwnd,RECT rect)
{
   srand((unsigned)time(NULL));//初始化随机数生成器
   
   // Window initialization

   hdc = GetDC(hwnd);

   SetBestPixelFormat(hdc);
   
   hglrc = wglCreateContext(hdc);
   
   wglMakeCurrent(hdc, hglrc);
 
   InitScene(hwnd);//初始化场景

   soundengine = new SoundEngine(float(150) * 0.01f);
}
void SceneManager::BeginAutoDrive()
{
	m_Camera.BeginAutoDrive();


}
void SceneManager::FrameMove()
{
   //更新时间
  static int index = 0; 
	
	if(m_Stop)return;//暂停动画

   frameTime = m_Timer.tick();

   
   //更新粒子系统
   // Slows fireworks, but not camera
   if(kSlowMotion) frameTime *= 0.5f;
  

   if(m_Fps!=0)
   {
    frameTime = 1/(float)m_Fps; 

   }
   //更新照像机
   m_Camera.Update(m_ParticleSystem.particles,frameTime,m_ParticleSystem.last_particle);


   m_ParticleSystem.AllocNewParticle();//增加新粒子空间
  
   //是否暂停动画
   if(!kFireworks) return; //如果暂停动画返回
	
   //更新世界
   theWorld.update(frameTime);
	
   //减淡
   static float ambientlight = float(dAmbient) * 0.01f;
   for(unsigned int i=0; i<m_ParticleSystem.last_particle; ++i)
   {
	 Particle* darkener(&(m_ParticleSystem.particles[i]));
	 if(darkener->type == SMOKE) darkener->rgb[0] = darkener->rgb[1] = darkener->rgb[2] = ambientlight;
   }

	//改变火箭发射速率
    static float rocketTimer[6] = {0,0,0,0,0,0};
	static float rocketTimeConst[6] = {5.0f,25,40,35,50,55};
	static float changeRocketTimeConst[6] ={25,50,65,65,50,50};
	

	for(int i=0;i<6;i++)
	{
     changeRocketTimeConst[i] -= frameTime;
     if(changeRocketTimeConst[i] <= 0.0f)
	{
	 float temp = rsRandf(4.0f);
	 //rocketTimeConst[i] = (temp * temp) + (10.0f / float(dMaxrockets));
	 changeRocketTimeConst[i] = rsRandf(30.0f) + 10.0f;
	}
     rocketTimer[i] -= frameTime;
	}

	//发射火箭
	
	if(rocketTimer[0] <= 0.0f)//生存期结束
	{
	   Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(20.0f);// - 900.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(20.0f);// - 900.0f;
	   rock->initRocket();
       int y =rsRandi(8);
	  

	   if(index==2)
	   {
         rock->explosiontype =6; 
	   }
	   else if(index==5)
	   {
         rock->explosiontype =6; 
	   }
	   else if(index==8)
	   {
         rock->explosiontype =6; 
	   }
	   else
	   {
	   rock->m_Fire = &m_CFire[index];
	   m_CFire[index].m_Exploded = false;//未爆炸
	    m_CFire[index].m_Boom = false; 
	   rock->explosiontype =22; 
	   }
	   index++;
	   index = index%11;
	   
	   
	 
	   if(dMaxrockets) rocketTimer[0] = rocketTimeConst[0];//火箭时间
		else rocketTimer[0] = 60.0f;  // arbitrary number since no rockets ever fire	
	}
   /*if(rocketTimer[1] <= 0.0f)//生存期结束
	{
	   Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(200.0f);// + 800.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(200.0f);// + 500.0f;
	   rock->initRocket();
	    if(rsRandi(2))
	   rock->m_Fire = &m_CFire[1];
		else rock->m_Fire = &m_CFire[5];
	   rock->explosiontype = 22; 
       if(dMaxrockets) rocketTimer[1] = rsRandf(rocketTimeConst[1]);//火箭时间
	   else rocketTimer[1] = 60.0f;  // arbitrary number since no rockets ever fire	

	   }
	if(rocketTimer[2] <= 0.0f)//生存期结束
	{
	  Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(200.0f) + 700.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(200.0f) - 500.0f;
	   rock->initRocket();
	   if(rsRandi(2))
	   rock->m_Fire = &m_CFire[2];
	   else rock->m_Fire = &m_CFire[6];
	   rock->explosiontype = 22; 
	   		 
	  
		if(dMaxrockets) rocketTimer[2] = rsRandf(rocketTimeConst[2]);//火箭时间
		else rocketTimer[2] = 60.0f;  // arbitrary number since no rockets ever fire	
	}
	if(rocketTimer[3] <= 0.0f)//生存期结束
	{
	   Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(200.0f) - 500.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(200.0f) + 700.0f;
	   rock->initRocket();
	   if(rsRandi(2))
	   rock->m_Fire = &m_CFire[3];
	   else rock->m_Fire = &m_CFire[7];
	   rock->explosiontype = 22; 
	   if(dMaxrockets) rocketTimer[3] = rsRandf(rocketTimeConst[3]);//火箭时间
		else rocketTimer[3] = 60.0f;  // arbitrary number since no rockets ever fire	
	}
	if(rocketTimer[4] <= 0.0f)//生存期结束
	{
	   Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(200.0f) - 1000.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(200.0f) + 500.0f;
	   rock->initRocket();
	   rock->explosiontype = 3; 
	   if(dMaxrockets) rocketTimer[4] = rsRandf(rocketTimeConst[4]);//火箭时间
		else rocketTimer[4] = 60.0f;  // arbitrary number since no rockets ever fire	
	}
	if(rocketTimer[5] <= 0.0f)//生存期结束
	{
	   Particle* rock = m_ParticleSystem.AddParticle();//火箭粒子

	   //初始化火箭 //确定火箭类型
	   rock->xyz[0] = rsRandf(200.0f) - 1300.0f;
	   rock->xyz[1] = 5.0f;
	   rock->xyz[2] = rsRandf(200.0f) + 500.0f;
	   rock->initRocket();
	   rock->explosiontype = 4; 
	   if(dMaxrockets) rocketTimer[5] = rsRandf(rocketTimeConst[5]);//火箭时间
		else rocketTimer[5] = 60.0f;  // arbitrary number since no rockets ever fire	
	}
	*/
	

		//更新粒子
		numRockets = 0;
		for(unsigned int i=0; i<m_ParticleSystem.last_particle; i++)
		{
		  Particle* curpart(&(m_ParticleSystem.particles[i]));
		  m_ParticleSystem.particles[i].update(frameTime,dSmoke,dClouds,dWind,dAmbient,dIllumination,&theWorld);
		  if(curpart->type == ROCKET) numRockets++;
		 
		  
		  // curpart->findDepth(m_Camera.cameraPos,billboardMat);
		  


		  if(curpart->type==IGNITOR&&curpart->life <0.93)
		  {
            if(!curpart->m_Fire->m_Exploded)//未爆炸
		{
     if(IsRecord())
	{
	  m_PlayTime.push_back(m_Frame*m_f);//记录时间
	  m_PlayStyle.push_back(BOOM1SOUND );//记录类型
	  m_PlayPos.push_back(m_Camera.cameraPos);
	  m_PlaySource.push_back(curpart->xyz);
	  soundengine->insertSoundNode(BOOM1SOUND , curpart->xyz, m_Camera.cameraPos);
      
	  m_PlayTime.push_back(m_Frame*m_f);//记录时间
	  m_PlayStyle.push_back(SUCKSOUND);//记录类型
	  m_PlayPos.push_back(m_Camera.cameraPos);
	  m_PlaySource.push_back(curpart->xyz);
      soundengine->insertSoundNode(SUCKSOUND, curpart->xyz,m_Camera.cameraPos);

	}
	else
	{
   //播放声音
  soundengine->insertSoundNode(BOOM1SOUND, curpart->xyz, m_Camera.cameraPos);
  soundengine->insertSoundNode(SUCKSOUND , curpart->xyz, m_Camera.cameraPos);
   } 
    curpart->m_Fire->m_Exploded = true;
   }
}

     if(curpart->m_Fire!=NULL&&curpart->type==ROCKET&&curpart->life <0.43)
	 {
       if(!curpart->m_Fire->m_Boom)//未爆炸
			  {
               if(IsRecord())
	        {
	         m_PlayTime.push_back(m_Frame*m_f);//记录时间
	         m_PlayStyle.push_back(BOOM1SOUND );//记录类型
	         m_PlayPos.push_back(m_Camera.cameraPos);
	         m_PlaySource.push_back(curpart->xyz);
	         soundengine->insertSoundNode(BOOM1SOUND , curpart->xyz, m_Camera.cameraPos);
     
	         }
	        else
	        {
             //播放声音
             soundengine->insertSoundNode(BOOM1SOUND, curpart->xyz, m_Camera.cameraPos);
 
           } 
                 curpart->m_Fire->m_Boom = true;
          }


	 }


		  //点燃烟花
		  if(curpart->life <= 0.0f || curpart->xyz[1] < 0.0f)
		  {
		   switch(curpart->type)
		   {
			case ROCKET:
			if(curpart->xyz[1] <= 0.0f)
			{
			  // move above ground for explosion so new particles aren't removed
			  curpart->xyz[1] = 0.1f;
			  curpart->vel[1] *= -0.7f;
			}
					
			if(curpart->explosiontype == 18)
			curpart->initSpinner();
			else
			{
			 
				
				curpart->initExplosion();
			


			}
			break;
				
			case IGNITOR:
            
	
			curpart->m_Fire->m_Ignitor++;
		    if(curpart->m_Fire->m_Ignitor==8)
			{
             curpart->m_Fire->m_Ignitor = 0;
			 curpart->explosiontype = 0;
			  curpart->initExplosion();		
			 curpart->explosiontype = 21;
			 curpart->initExplosion();
			}
			else
			{
              curpart->explosiontype = 0;
			  curpart->initExplosion();			
           
			}

			break;
			case POPPER:
			switch(curpart->explosiontype)
			{
			case 21:
            curpart->initExplosion();

				break;
			  case STAR:
			  curpart->explosiontype = 100;
			  curpart->initExplosion();
			  break;
					
			  case STREAMER:
			  curpart->explosiontype = 101;
			  curpart->initExplosion();
			  break;
					
			  case METEOR:
			  curpart->explosiontype = 102;
			  curpart->initExplosion();			
			  break;
		
			  case POPPER:
			  curpart->type = STAR;
			  curpart->rgb.set(1.0f, 0.8f, 0.6f);
			  curpart->t = m_ParticleSystem.particles[i].tr = m_ParticleSystem.particles[i].life = 0.2f;		
			}
			break;
				
			case SUCKER:
			  curpart->initShockwave();
			  break;
				
			case STRETCHER:
			  curpart->initBigmama();		
		   }
		  }
		}

   m_ParticleSystem.DeallocParticle();//删除粒子
		
}
void SceneManager::Render()
{
 
   // super fast easter egg
   static int superFast = rsRandi(1000);
   if(!superFast) frameTime *= 5.0f;


   m_Camera.Setup();//设置照像机

   // 更新 billboard矩阵
   glPushMatrix();
   glLoadIdentity();
   glRotatef(m_Camera.heading, 0, 1, 0);
   glRotatef(m_Camera.pitch, 1, 0, 0);
   glGetFloatv(GL_MODELVIEW_MATRIX, billboardMat);
   glPopMatrix();

   // clear the screen
   glClear(GL_COLOR_BUFFER_BIT);

   //绘制世界
   theWorld.draw();

   m_Terrain.Render(&m_Camera);

   //绘制粒子
   glEnable(GL_BLEND);

   for(unsigned int i=0; i<m_ParticleSystem.last_particle; i++) m_ParticleSystem.particles[i].draw(billboardMat,&theWorld);

   //绘制光晕
   if(dFlare)
   {
	makeFlareList();
	for(int i=0; i<numFlares; ++i)
	m_Flare.flare(&m_Camera,lensFlares[i].x, lensFlares[i].y, lensFlares[i].r,lensFlares[i].g, lensFlares[i].b, lensFlares[i].a);
	numFlares = 0;
   }
	
   wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);

   //更新声音

   float listenerOri[6];
		listenerOri[0] = float(-m_Camera.modelMat[2]);
		listenerOri[1] = float(-m_Camera.modelMat[6]);
		listenerOri[2] = float(-m_Camera.modelMat[10]);
		listenerOri[3] = float(m_Camera.modelMat[1]);
		listenerOri[4] = float(m_Camera.modelMat[5]);
		listenerOri[5] = float(m_Camera.modelMat[9]);
		soundengine->update(m_Camera.cameraPos.v, m_Camera.cameraVel.v, listenerOri, frameTime, false);
}
void SceneManager::SetFPS(int fps)
{
 m_Fps = fps;


 m_f = (float)1000/(float)fps;
}
void SceneManager::CheckStop()
{
 m_Stop = !m_Stop;
 m_Timer.Reset();
}
void SceneManager::Reset(RECT rect)
{
	m_Camera.Reset(rect);
}
void SceneManager::SetBestPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	int m_iGLPixelIndex = ChoosePixelFormat(hdc,&pixelDesc);
	if(m_iGLPixelIndex == 0) 
	{
		m_iGLPixelIndex = 1;
		DescribePixelFormat(hdc,m_iGLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc);
			
	}
	
	SetPixelFormat(hdc,m_iGLPixelIndex,&pixelDesc);
		
}
void SceneManager::InitScene(HWND hwnd)
{
   //初始化 OpenGL状态
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_TEXTURE_2D);
   glFrontFace(GL_CCW);
   glEnable(GL_CULL_FACE);

   //初始化场景
   m_Flare.initFlares();

   m_Smoke.initSmoke(dSmoke,dExplosionsmoke);
	
   theWorld.InitWorld(dWind,dAmbient,dStardensity,dMoonglow,dMoon,dClouds,dEarth);

   initShockwave();

   m_Terrain.Create();
}
void SceneManager::Clear(HWND hwnd)
{
	m_ParticleSystem.particles.clear();
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}

void SceneManager::makeFlareList()
{
	rsVec cameraDir, partDir;
	const float shine(float(dFlare) * 0.01f);

	cameraDir = m_Camera.lookAt[0] - m_Camera.lookFrom[0];
	cameraDir.normalize();
	for(unsigned int i=0; i<m_ParticleSystem.last_particle; ++i){
		Particle* curlight = &(m_ParticleSystem.particles[i]);
		if(curlight->type == EXPLOSION || curlight->type == SUCKER
			|| curlight->type == SHOCKWAVE || curlight->type == STRETCHER
			|| curlight->type == BIGMAMA){
			double winx, winy, winz;
			gluProject(curlight->xyz[0], curlight->xyz[1], curlight->xyz[2],
				m_Camera.modelMat, m_Camera.projMat, m_Camera.viewport,
				&winx, &winy, &winz);
			partDir = curlight->xyz - m_Camera.cameraPos;
			if(partDir.dot(cameraDir) > 1.0f){  // is light source in front of camera?
				if(numFlares == lensFlares.size())
					lensFlares.resize(lensFlares.size() + 10);
				lensFlares[numFlares].x = (float(winx) / float(m_Camera.xsize)) * m_Camera.aspectRatio;
				lensFlares[numFlares].y = float(winy) / float(m_Camera.ysize);
				rsVec vec = curlight->xyz - m_Camera.cameraPos;  // find distance attenuation factor
				if(curlight->type == EXPLOSION){
					lensFlares[numFlares].r = curlight->rgb[0];
					lensFlares[numFlares].g = curlight->rgb[1];
					lensFlares[numFlares].b = curlight->rgb[2];
					float distatten = (10000.0f - vec.length()) * 0.0001f;
					if(distatten < 0.0f)
						distatten = 0.0f;
					lensFlares[numFlares].a = curlight->bright * shine * distatten;
				}
				else{
					lensFlares[numFlares].r = 1.0f;
					lensFlares[numFlares].g = 1.0f;
					lensFlares[numFlares].b = 1.0f;
					float distatten = (20000.0f - vec.length()) * 0.00005f;
					if(distatten < 0.0f)
						distatten = 0.0f;
					lensFlares[numFlares].a = curlight->bright * 2.0f * shine * distatten;
				}
				numFlares++;
			}
		}
	}
}








void SceneManager::SetMaxRockets(int d)
{
}
void SceneManager::SetStarDensity(int d)
{
}
void SceneManager::SetSmoke(int d)
{
}
void SceneManager::SetFlare(int d)
{
}
void SceneManager::SetExplosionSmoke(int d)
{
}
void SceneManager::SetMoonGlow(int d)
{
}
void SceneManager::SetWind(int d)
{
}
void SceneManager::SetAmbient(int d)
{
}
void SceneManager::SetFrameRate(int d)
{
}
void SceneManager::CheckMoon()
{
}
void SceneManager::CheckClouds()
{
}
void SceneManager::CheckEarth()
{
}
void SceneManager::CheckIllumination()
{
}
void SceneManager::SpeedUpCamera()
{
 m_Camera.m_Speed +=5;

}
void SceneManager::SpeedDownCamera()
{
 m_Camera.m_Speed -=5;
}