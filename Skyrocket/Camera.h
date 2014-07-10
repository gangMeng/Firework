#ifndef _CAMERA_
#define _CAMERA_

#include "../Math/Point3.h"
#include"rsMath.h"
#include"particle.h"

#include<vector>

class Camera
{

 public:

   Camera();

   void Setup();//设置照像机参数
  
   void Update(std::vector<Particle> & particles,float frameTime,unsigned int last_particle);//更新照像机

   void Reset(RECT rect);//根据窗口重置照像机

   float m_Speed;

   //用户操纵
   void Forword();//前进
   void Backword();//后退
   void Left();//向左
   void Right();//向右
   void Heading(float h);
   void Pitch(float p);
   
   //自动操纵
   void randomLookFrom(int n);
   void randomLookAt(int n);
   void findHeadingAndPitch(rsVec lookFrom, rsVec lookAt, float& heading, float& pitch);
   void BeginAutoDrive();
   void AutoDrive(float time);//自动驾驶

   //获取矩阵参数
   void GetWorldMatrix(float * matWorld);
   void GetViewMatrix(float * matView);
   void GetProjMatrix(float * matProj);

   // Mouse variables
   float mouseIdleTime;
   int mouseButtons, mousex, mousey;
   float mouseSpeed;
   POINT mousePoint;

   //照像机状态
   bool m_AutoDrive;//是否自动观察
   int kNewCamera;
   int kCamera ;  // 0 = 暂停, 1 = 自动, 2 = 鼠标控制
    
   // 照像机参数
   int xsize, ysize, centerx, centery;
   float aspectRatio;
   rsVec lookFrom[3];  //position, target position, last position
   rsVec lookAt[3];// position, target position, last position
   rsVec cameraPos;  // used for positioning sounds (same as lookFrom[0])
   rsVec cameraVel;  // used for doppler shift
   double modelMat[16], projMat[16];
   int viewport[4];
   
 
   unsigned int zoomRocket;

   float cameraAngle;
   float firstHeading;
   float firstRadius;
   int   lastCameraMode;
   float cameraTime[3]; // time, elapsed time, step (1.0 - 0.0)
   float fov;
   float zoom;  // For interpolating from regular camera view to zoomed in view
   float zoomTime[2];  // time until next zoom, duration of zoom
   float heading, pitch;
   float zoomHeading;
   float zoomPitch ;

   Point3f m_FrontDir;//前向
  
   Point3f m_SideDir;//侧向


};
#endif