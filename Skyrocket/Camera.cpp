#include "stdafx.h"
#include "Camera.h"

#define ZOOMROCKETINACTIVE 1000000000
#define PI 3.14159265359f
#define PIx2 6.28318530718f
#define D2R 0.0174532925f
#define R2D 57.2957795131f


Camera::Camera()
{
  //照像机状态
  kCamera = 1;
  kNewCamera = 0;

  //照像机参数

  lookAt[0] = rsVec(0.0f, -1.0f, 0.0f);
  lookAt[1] = rsVec(0.0f, -1.0f, 0.0f);
  lookAt[2] = rsVec(0.0f, -1.0f, 0.0f);

  lookFrom[0] = rsVec(0.0f, 700.0f, 2000.0f);
  lookFrom[1] = rsVec(0.0f, 700.0f, 2000.0f);
  lookFrom[2] = rsVec(0.0f, 700.0f, 2000.0f);

  m_FrontDir = Point3f(0,0,-1);

  m_SideDir = Point3f(-1,0,0);


  pitch = 0.0;
  heading = 0.0;
  
  cameraAngle = 0.0f;
  firstHeading = rsRandf(2.0f * PIx2);
  firstRadius = rsRandf(2000.0f);
  lastCameraMode = kCamera;
  cameraTime[0]=20.0f;
  cameraTime[1]=0.0f;
  cameraTime[2]=0.0f;
  fov = 60.0f;
  zoom = 0.0f;
  zoomTime[0]=300.0f;
  zoomTime[0]=0.0f;
  zoomHeading = 0.0f;
  zoomPitch = 0.0f;
  zoomRocket = ZOOMROCKETINACTIVE;

  m_AutoDrive = false;

  m_Speed = 20;

}

void Camera::Reset(RECT rect)
{
  xsize = rect.right - rect.left;
  ysize = rect.bottom - rect.top;
  centerx = rect.left + xsize / 2;
  centery = rect.top + ysize / 2;
  glViewport(rect.left, rect.top, xsize, ysize);
  glGetIntegerv(GL_VIEWPORT, viewport);
  aspectRatio = float(rect.right) / float(rect.bottom);
  
}
void Camera::Setup()
{
  //投影
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov,aspectRatio, 1.0f, 40000.0f);
  glGetDoublev(GL_PROJECTION_MATRIX,projMat);

  //模型变换
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
 
  glRotatef(-pitch, 1, 0, 0);
  glRotatef(-heading, 0, 1, 0);
  glTranslatef(-lookFrom[0][0], -lookFrom[0][1], -lookFrom[0][2]);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);

}

void Camera::Forword()
{
 Point3f dir;

 lookFrom[0][0] +=m_FrontDir.X()*50;
 lookFrom[0][1] +=m_FrontDir.Y()*50;
 lookFrom[0][2] +=m_FrontDir.Z()*50;


}
void Camera::Backword()
{

 lookFrom[0][0] +=-m_FrontDir.X()*50;
 lookFrom[0][1] +=-m_FrontDir.Y()*50;
 lookFrom[0][2] +=-m_FrontDir.Z()*50;
}
void Camera::Heading(float h)
{
  heading =h;
  
   //计算照像机方向

  Point3f originF = Point3f(0,0,-1);
  Point3f originS = Point3f(-1,0,0);
  Point3f result;

  float ch = cosf(D2R * heading);
  float sh = sinf(D2R * heading);
  float cp = cosf(D2R * pitch);
  float sp = sinf(D2R * pitch);

  //前向量
  result.X() = ch*originF.X() + sh*originF.Z();
  result.Y() = originF.Y();
  result.Z() = -sh*originF.X() + ch*originF.Z();

  m_FrontDir.X() = result.X();
  m_FrontDir.Y() = cp*result.Y() - sp*result.Z();
  m_FrontDir.Z() = sp*result.Y() + cp*result.Z();
  

  //侧向量
  result.X() = ch*originS.X() + sh*originS.Z();
  result.Y() = originS.Y();
  result.Z() = -sh*originS.X() + ch*originS.Z();

  m_SideDir.X() = result.X();
  m_SideDir.Y() = cp*result.Y() - sp*result.Z();
  m_SideDir.Z() = sp*result.Y() + cp*result.Z();

  
}
void Camera::Pitch(float p)
{
 
 pitch = p;

   //计算照像机方向

  Point3f originF = Point3f(0,0,-1);
  Point3f originS = Point3f(-1,0,0);
  Point3f result;

  float ch = cosf(D2R * heading);
  float sh = sinf(D2R * heading);
  float cp = cosf(D2R * pitch);
  float sp = sinf(D2R * pitch);

  //前向量
  result.X() = ch*originF.X() + sh*originF.Z();
  result.Y() = originF.Y();
  result.Z() = -sh*originF.X() + ch*originF.Z();

  m_FrontDir.X() = result.X();
  m_FrontDir.Y() = cp*result.Y() - sp*result.Z();
  m_FrontDir.Z() = sp*result.Y() + cp*result.Z();
  

  //侧向量
  result.X() = ch*originS.X() + sh*originS.Z();
  result.Y() = originS.Y();
  result.Z() = -sh*originS.X() + ch*originS.Z();

  m_SideDir.X() = result.X();
  m_SideDir.Y() = cp*result.Y() - sp*result.Z();
  m_SideDir.Z() = sp*result.Y() + cp*result.Z();

}
void Camera::Right()
{
 lookFrom[0][0] +=-m_SideDir.X()*50;
 lookFrom[0][1] +=-m_SideDir.Y()*50;
 lookFrom[0][2] +=-m_SideDir.Z()*50;

}
void Camera::Left()
{
 lookFrom[0][0] +=m_SideDir.X()*50;
 lookFrom[0][1] +=m_SideDir.Y()*50;
 lookFrom[0][2] +=m_SideDir.Z()*50;
}
void Camera::GetWorldMatrix(float * matWorld)
{

}
void Camera::GetViewMatrix(float * matView)
{
 
 
}
void Camera::GetProjMatrix(float * matProj)
{
  

}


void Camera::randomLookFrom(int n)
{
	lookFrom[n][0] = rsRandf(6000.0f) - 3000.0f;
	lookFrom[n][1] = rsRandf(1200.0f) + 5.0f;
	lookFrom[n][2] = rsRandf(6000.0f) - 3000.0f;
}
void Camera::randomLookAt(int n)
{
	lookAt[n][0] = rsRandf(1600.0f) - 800.0f;
	lookAt[n][1] = rsRandf(800.0f) + 200.0f;
	lookAt[n][2] = rsRandf(1600.0f) - 800.0f;
}


void Camera::findHeadingAndPitch(rsVec lookFrom, rsVec lookAt, float& heading, float& pitch)
{
	const float diffx(lookAt[0] - lookFrom[0]);
	const float diffy(lookAt[1] - lookFrom[1]);
	const float diffz(lookAt[2] - lookFrom[2]);
	const float radius(sqrtf(diffx * diffx + diffz * diffz));
	pitch = R2D * atan2f(diffy, radius);
	heading = R2D * atan2f(-diffx, -diffz);
}



void Camera::Update(std::vector<Particle> & particles,float frameTime,unsigned int last_particle)
{
   // change camera position and angle

	if(m_AutoDrive)AutoDrive(frameTime);

   float cameraStep = 0.5f * (1.0f - cosf(cameraTime[2] * PI));

   //场景中其他物体使用
   cameraVel = lookFrom[0] - cameraPos;
   cameraPos = lookFrom[0];
			
}
void Camera::AutoDrive(float time)
{
 
  static float t = 0;

  t+=3.1415927*time*m_Speed/360;

  float si,co;
  si  = sin(t);
  co = cos(t);


  lookFrom[0] = rsVec(3100*si, 700.0f, 3100*co);

  heading = t*180/3.1415927;
}
void Camera::BeginAutoDrive()
{

  m_AutoDrive = !m_AutoDrive;

  lookAt[0] = rsVec(0.0f, -1.0f, 0.0f);
  lookAt[1] = rsVec(0.0f, -1.0f, 0.0f);
  lookAt[2] = rsVec(0.0f, -1.0f, 0.0f);

  lookFrom[0] = rsVec(0.0f, 700.0f, 3100.0f);
  lookFrom[1] = rsVec(0.0f, 700.0f, 3100.0f);
  lookFrom[2] = rsVec(0.0f, 700.0f, 3100.0f);

  m_FrontDir = Point3f(0,0,-1);

  m_SideDir = Point3f(-1,0,0);


  pitch = 0.0;
  heading = 0.0;


}