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

   void Setup();//�������������
  
   void Update(std::vector<Particle> & particles,float frameTime,unsigned int last_particle);//���������

   void Reset(RECT rect);//���ݴ������������

   float m_Speed;

   //�û�����
   void Forword();//ǰ��
   void Backword();//����
   void Left();//����
   void Right();//����
   void Heading(float h);
   void Pitch(float p);
   
   //�Զ�����
   void randomLookFrom(int n);
   void randomLookAt(int n);
   void findHeadingAndPitch(rsVec lookFrom, rsVec lookAt, float& heading, float& pitch);
   void BeginAutoDrive();
   void AutoDrive(float time);//�Զ���ʻ

   //��ȡ�������
   void GetWorldMatrix(float * matWorld);
   void GetViewMatrix(float * matView);
   void GetProjMatrix(float * matProj);

   // Mouse variables
   float mouseIdleTime;
   int mouseButtons, mousex, mousey;
   float mouseSpeed;
   POINT mousePoint;

   //�����״̬
   bool m_AutoDrive;//�Ƿ��Զ��۲�
   int kNewCamera;
   int kCamera ;  // 0 = ��ͣ, 1 = �Զ�, 2 = ������
    
   // ���������
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

   Point3f m_FrontDir;//ǰ��
  
   Point3f m_SideDir;//����


};
#endif