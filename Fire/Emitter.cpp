#include"stdafx.h"
#include"Emitter.h"


Emitter::Emitter()
{
 HALF_RAND = (RAND_MAX / 2);
}
float Emitter::RandomNum()
{
  int rn;
  rn = rand();	//����α�����
  return ((float)(rn - HALF_RAND) / (float)HALF_RAND);
					//����-1��1֮��������
}

void Emitter::Reset(Point3f explodePosition)
{

   k=0.01;     //����Ħ�����ļ���ϵ��
	
   //��ըλ�ù̶�
   explodePos.X()= pos.X() + explodePosition.X();
   explodePos.Y()= pos.Y() + explodePosition.Y();
   explodePos.Z()= pos.Z() + explodePosition.Z();

	speed = 0.05f;		//������ʼ�ٶ�
	speedVar = 0.003f;

	force.X() = 0.000f;
	force.Y() = -50.7132f;		//�������µ�����
	force.Z() = 0.0f;

	//����ը���κ����������һ�������Ա��ڵ����̻����µ����ٶȺ�Ч��
	shapedForce.X()=0.000f;
	shapedForce.Y()=-50.7032f;	
	shapedForce.Z()=0.0f;

	explodeSpeed=0.015f;//0.04;//0.02f;			//��1�α�ը��ĳ�ʼ�ٶ�
	explodeSpeedVar=0.005f;

	secondExpSpeed=0.015f;//0.08f;	//0.03f;	//��2�α�ը��ĳ�ʼ�ٶ�
	
	emitsPerFrame	= 300;	//ÿ֡�����������

	life = 40;//20;//40;			//��һ�α�ը�����ӵ�����ֵ
	lifeVar = 10;
	secLife = 40;//20;//40;		//��2�α�ը�����ӵ�����ֵ
 
	//��ɫ����,��ը����ɫ

	explodeColor[0]=(RandomNum()+1.0)*0.5;
	explodeColor[1]=(RandomNum()+1.0)*0.5;
	explodeColor[2]=(RandomNum()+1.0)*0.5;

	explodeColor[3]=0.7;//1.0;	//��͸��

	//�����׶ε���ɫ
	starColor[0]=0.8;
	starColor[1]=0.8;
	starColor[2]=0.8;
	starColor[3]=1.0;
	
	//��ը���С�������ӣ���4%���ı仯��ɫ
	rndColor[0]=0.8;
	rndColor[1]=0.8;
	rndColor[2]=0.8;
	rndColor[3]=0.7;//1.0;

	//���ӳ��ͺ����ɫ

	shapedColor[0]=1.0;//0.98;//0.92;
	shapedColor[1]=0.3;//0.42;//0.14;
	shapedColor[2]=0.7;//0.3;//0.1;
	
	shapedColor[3]=0.7;//0.5;



}