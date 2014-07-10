#include"stdafx.h"
#include"Emitter.h"


Emitter::Emitter()
{
 HALF_RAND = (RAND_MAX / 2);
}
float Emitter::RandomNum()
{
  int rn;
  rn = rand();	//产生伪随机数
  return ((float)(rn - HALF_RAND) / (float)HALF_RAND);
					//返回-1至1之间的随机数
}

void Emitter::Reset(Point3f explodePosition)
{

   k=0.01;     //空气摩擦力的计算系数
	
   //爆炸位置固定
   explodePos.X()= pos.X() + explodePosition.X();
   explodePos.Y()= pos.Y() + explodePosition.Y();
   explodePos.Z()= pos.Z() + explodePosition.Z();

	speed = 0.05f;		//上升初始速度
	speedVar = 0.003f;

	force.X() = 0.000f;
	force.Y() = -50.7132f;		//仅有向下的重力
	force.Z() = 0.0f;

	//将爆炸成形后的重力另设一变量，以便于调整烟花往下掉的速度和效果
	shapedForce.X()=0.000f;
	shapedForce.Y()=-50.7032f;	
	shapedForce.Z()=0.0f;

	explodeSpeed=0.015f;//0.04;//0.02f;			//第1次爆炸后的初始速度
	explodeSpeedVar=0.005f;

	secondExpSpeed=0.015f;//0.08f;	//0.03f;	//第2次爆炸后的初始速度
	
	emitsPerFrame	= 300;	//每帧发射的粒子数

	life = 40;//20;//40;			//第一次爆炸的粒子的生命值
	lifeVar = 10;
	secLife = 40;//20;//40;		//第2次爆炸的粒子的生命值
 
	//颜色设置,爆炸的颜色

	explodeColor[0]=(RandomNum()+1.0)*0.5;
	explodeColor[1]=(RandomNum()+1.0)*0.5;
	explodeColor[2]=(RandomNum()+1.0)*0.5;

	explodeColor[3]=0.7;//1.0;	//不透明

	//上升阶段的颜色
	starColor[0]=0.8;
	starColor[1]=0.8;
	starColor[2]=0.8;
	starColor[3]=1.0;
	
	//爆炸后的小部分粒子（如4%）的变化颜色
	rndColor[0]=0.8;
	rndColor[1]=0.8;
	rndColor[2]=0.8;
	rndColor[3]=0.7;//1.0;

	//粒子成型后的颜色

	shapedColor[0]=1.0;//0.98;//0.92;
	shapedColor[1]=0.3;//0.42;//0.14;
	shapedColor[2]=0.7;//0.3;//0.1;
	
	shapedColor[3]=0.7;//0.5;



}