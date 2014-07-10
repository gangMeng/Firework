#ifndef _EMITTER_
#define _EMITTER_

#include "../Math/Point3.h"

class Emitter//���ӷ�����,������������
{
 public:	

	Emitter();

	void Reset(Point3f explodePosition);//���÷�����


	float RandomNum();
	
	int HALF_RAND ;

	Point3f		pos;						// XYZ POSITION
	Point3f		explodePos;					//��ը��λ�ã������pos��λ�ƣ�

	int			pId;						//�ѷ������ӵ�ID��
	int			liftCount;					//�����׶����Ӽ���
	int         explodeCount;				//1�α�ը����
	int			secondCount;				//2�α�ը����
	int			midCount;					//�м䱬ը����

	float		speed,speedVar;				//�����׶ε��ٶ�
	float		explodeSpeed,explodeSpeedVar;	//��ը����ٶ�
	float		secondExpSpeed;				//2�α�ը���ٶ�

	float        k;		//����Ħ����ϵ��

	int			emitsPerFrame;//, emitVar;		// EMITS PER FRAME AND VARIATION
	int			life, lifeVar;				// LIFE COUNT AND VARIATION
	int			secLife;					//2�α�ը�����ӵ�����ֵ	

	float		explodeColor[4];				//���ӵ�1�α�ըʱ����ɫ, 
	float		starColor[4];					//���������׶ε���ɫ
	float		rndColor[4];					//��ը���С�������������ɫ
	float		shapedColor[4];				//���ͺ����ɫ

	Point3f		force;						//�����׶ε�������

	Point3f		shapedForce;				//��ը���κ��������

	int			secCount;		//���α�ը�������ѷ����������
};
#endif
