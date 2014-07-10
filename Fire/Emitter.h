#ifndef _EMITTER_
#define _EMITTER_

#include "../Math/Point3.h"

class Emitter//粒子发射器,管理粒子属性
{
 public:	

	Emitter();

	void Reset(Point3f explodePosition);//重置发射器


	float RandomNum();
	
	int HALF_RAND ;

	Point3f		pos;						// XYZ POSITION
	Point3f		explodePos;					//爆炸的位置（相对于pos的位移）

	int			pId;						//已发射粒子的ID号
	int			liftCount;					//上升阶段粒子计数
	int         explodeCount;				//1次爆炸计数
	int			secondCount;				//2次爆炸计数
	int			midCount;					//中间爆炸计数

	float		speed,speedVar;				//上升阶段的速度
	float		explodeSpeed,explodeSpeedVar;	//爆炸后的速度
	float		secondExpSpeed;				//2次爆炸的速度

	float        k;		//空气摩擦的系数

	int			emitsPerFrame;//, emitVar;		// EMITS PER FRAME AND VARIATION
	int			life, lifeVar;				// LIFE COUNT AND VARIATION
	int			secLife;					//2次爆炸的粒子的生命值	

	float		explodeColor[4];				//粒子第1次爆炸时的颜色, 
	float		starColor[4];					//粒子上升阶段的颜色
	float		rndColor[4];					//爆炸后的小部分随机粒子颜色
	float		shapedColor[4];				//成型后的颜色

	Point3f		force;						//上升阶段的作用力

	Point3f		shapedForce;				//爆炸成形后的作用力

	int			secCount;		//二次爆炸发射器已发射的粒子数
};
#endif
