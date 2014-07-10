#include"stdafx.h"
#include"ConstrainedFire.h"

#ifndef _PARTICLESYSTEM_
#include"../Skyrocket/ParticleSystem.h"
#endif
ParticleSystem * ConstrainedFire::m_ParticleSystem = NULL;

const int HALF_RAND = (RAND_MAX / 2);

GLuint     objectDisplayed = 0;


int m_K = 45;

int	numParticlesSecondExp;	//第二次爆炸的最大粒子数量
float RandomNum()
{
	int rn;
	rn = rand();	//产生伪随机数
	return ((float)(rn - HALF_RAND) / (float)HALF_RAND);
					//返回-1至1之间的随机数
}
ConstrainedFire::ConstrainedFire()
{
 m_VertexThreshhold = 3000;
 fireworkStage = 0;
 currentEmitter = 0;
 m_Exploded = false;
  m_Boom = false; 
 objVertexNum = 0;
 objectImported =false;
 for(int i=0;i<8;i++) m_MatchNum[i] = 0;
 m_Ignitor = 0;

}
void ConstrainedFire::InitI(Emitter * emitter)
{
 int i;


	//设置二次爆炸的粒子发射器
	for (i=0;i<SEC_EMITTER;i++)
	{
/*		expEmitter[i].explodePos.x=emitter->explodePos.x+RandomNum()*1.5;
		expEmitter[i].explodePos.y=emitter->explodePos.y+RandomNum();
		expEmitter[i].explodePos.z=emitter->explodePos.z+RandomNum()*0.1;
*/
		//注意，RandomNum()返回-1至1之间的随机数
//		expEmitter[i].explodeColor.r=(RandomNum()+1.0)*0.25+0.5;	//0.5到1
//		expEmitter[i].explodeColor.g=(RandomNum()+1.0)*0.25;		//0到0.5
//		expEmitter[i].explodeColor.b=(RandomNum()+1.0)*0.25+0.5;	//0.5到1
		expEmitter[i].explodeColor[0]=(RandomNum()+1.0)*0.5;	//0到1
		expEmitter[i].explodeColor[1]=(RandomNum()+1.0)*0.5;	//0到1
		expEmitter[i].explodeColor[2]=(RandomNum()+1.0)*0.5;	//0到1
		expEmitter[i].explodeColor[3]=0.5;

		expEmitter[i].explodeSpeed=(emitter->secondExpSpeed);

		expEmitter[i].secCount=0;	//每个发射器已发射粒子数

	}
}

void ConstrainedFire::Init(ObjReader * obj)
{
   FireVertex  vertex;
   FireTriangle triangle;
   int end;

   //存储模型顶点
   end = (int)obj->Positions.size();
   
   for(int i=0;i<end;i++)
   {
	vertex.position = obj->Positions[i]*2.6; 
	m_Vertices.push_back(vertex);
   }

   //存储模型面信息
   end = (int) obj->m_Object[0]->posIndices.size();
   for(int i=0;i<end;)
   {
	triangle.index[0] = obj->m_Object[0]->posIndices[i++];
	triangle.index[1] = obj->m_Object[0]->posIndices[i++];
	triangle.index[2] = obj->m_Object[0]->posIndices[i++];
	triangle.area = TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	m_Faces.push_back(triangle);
   }

   Normalize();//归一化顶点数据

   Subdivide();//细分模型

   objectImported =true;
   objVertexNum = (int)m_Vertices.size();

   numParticlesSecondExp=m_Vertices.size()*1.8;

   KMeans(m_K);//聚类操作

   GenerateQuadrantByKMean();//生成发射区域

   //8个二次爆炸的粒子发生器的位置限制,注意顺序要与对象顶点分区编号对应
	/*Point3f expP;
	expP=modelCenter(m_Quadrant0);
	expEmitter[0].pos.X()=expP.X();
	expEmitter[0].pos.Y()=expP.Y();
	expEmitter[0].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant1);
	expEmitter[1].pos.X()=expP.X();
	expEmitter[1].pos.Y()=expP.Y();
	expEmitter[1].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant2);
	expEmitter[2].pos.X()=expP.X();
	expEmitter[2].pos.Y()=expP.Y();
	expEmitter[2].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant3);
	expEmitter[3].pos.X()=expP.X();
	expEmitter[3].pos.Y()=expP.Y();
	expEmitter[3].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant4);
	expEmitter[4].pos.X()=expP.X();
	expEmitter[4].pos.Y()=expP.Y();
	expEmitter[4].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant5);
	expEmitter[5].pos.X()=expP.X();
	expEmitter[5].pos.Y()=expP.Y();
	expEmitter[5].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant6);
	expEmitter[6].pos.X()=expP.X();
	expEmitter[6].pos.Y()=expP.Y();
	expEmitter[6].pos.Z()=expP.Z();
	expP=modelCenter(m_Quadrant7);
	expEmitter[7].pos.X()=expP.X();
	expEmitter[7].pos.Y()=expP.Y();
	expEmitter[7].pos.Z()=expP.Z();*/
}
void ConstrainedFire::SetPosition(Point3f center)
{
 for(int i=0;i<m_K;i++) 
 {expEmitter[i].Reset(center);
  m_MatchNum[i]=0;
 }
 currentEmitter = 0;

 m_Center = center;

}
void ConstrainedFire::Normalize()
{
  //顶点数据归一化
	
  float maxx, minx, maxy, miny, maxz, minz;
  float cx, cy, cz, w, h, d;
  float scale;
  int i;
  int end = (int)m_Vertices.size();

  maxx = minx = m_Vertices[0].position.X();
  maxy = miny = m_Vertices[0].position.Y();
  maxz = minz = m_Vertices[0].position.Z();
  
  for (i=1;i<end;i++) 
  {
   if (maxx < m_Vertices[i].position.X()) maxx = m_Vertices[i].position.X();
    
   if (minx > m_Vertices[i].position.X()) minx = m_Vertices[i].position.X();

   if (maxy < m_Vertices[i].position.Y()) maxy = m_Vertices[i].position.Y();
   
   if (miny > m_Vertices[i].position.Y())  miny = m_Vertices[i].position.Y();
        
   if (maxz < m_Vertices[i].position.Z()) maxz = m_Vertices[i].position.Z();
        
   if (minz > m_Vertices[i].position.Z()) minz = m_Vertices[i].position.Z();  
  }
    
   w = maxx-minx;    
   h = maxy-miny;	
   d = maxz-minz;

   //计算模型中心	  
   cx = (maxx + minx) *0.5;
   cy = (maxy + miny) *0.5;
   cz = (maxz + minz) *0.5;

   //计算缩放系数

   if(h>=d&&h>=w) scale = 1.0/h;
   else if(d>=h&&d>=w)scale = 1.5/d;
   else if(w>=h&&w>=d)scale = 1.5/w;

   //变换模型

   for(i=0;i<end;i++) 
   {
     m_Vertices[i].position.X() -= cx;
     m_Vertices[i].position.Y() -= cy;
     m_Vertices[i].position.Z() -= cz;
     //m_Vertices[i].position.X() *= scale;
     //m_Vertices[i].position.Y() *= scale;
     //m_Vertices[i].position.Z() *= scale;
	 //m_Vertices[i].position.X() += center.X();
    // m_Vertices[i].position.Y() += center.Y();
     //m_Vertices[i].position.Z() += center.Z();
    }

}

void ConstrainedFire::Subdivide()
{
   int    j,i;
   int	oldMaxVertex,oldMaxTriangle;		
   FireVertex vertex;
   FireTriangle triangle;
   bool	pInV1,pInV2,pInV3;	//标记点是否已在模型的顶点数组中
   int  index1,index2,index3;	//新增加的顶点索引值
   Point3f tp1,tp2,tp3;

   while(m_Vertices.size()<m_VertexThreshhold)	
   {
	 //计算三角形面积的平均值
	 float	aveArea=0.0;
	
	 oldMaxVertex = (int)m_Vertices.size();	//细分之前的模型顶点数，用于顶点搜索的开始位置
	 oldMaxTriangle = (int)m_Faces.size();	//添加新三角形之前的三角形个数，对新增加的三角形不用细分

	 for (i=0;i<oldMaxTriangle;i++)	aveArea += m_Faces[i].area;
	 
	 aveArea /= (float)oldMaxTriangle;

	 for (i=0;i<oldMaxTriangle;i++)	//对新增加的三角形不进行判断和细化
	 {
	   //对面积大于平均值的三角形，将各边中点相连
	   if(m_Faces[i].area < aveArea) continue;
	   
		 //计算各边中点	
	   tp1=pointMid(m_Faces[i].index[0],m_Faces[i].index[1]);
	   tp2=pointMid(m_Faces[i].index[1],m_Faces[i].index[2]);
	   tp3=pointMid(m_Faces[i].index[2],m_Faces[i].index[0]);

	   //判断各中点是否已在顶点数组中。若没有，则将中点添加到顶点数组末
	   //由于（仅）可能会在相邻三角形的细化过程中已被添加，所以只需从oldMaxVertex开始搜索
		 
	   pInV1=false;	//初始状态为不在数组中
	   pInV2=false;
	   pInV3=false;
		 
	  //对第1个点tp1
	  for (j=oldMaxVertex;j<(int)m_Vertices.size();j++)
	  {
	   if(tp1==m_Vertices[j].position)
	   {
	    pInV1=true;
		index1=j;
		break;
	   }
	  }
				
	  if(!pInV1)
	  {
	   //tp1不在数组中，添加到数组末尾
	   vertex.position = tp1;
	   m_Vertices.push_back(vertex);
	   index1 = (int)m_Vertices.size()-1;
	  }
				
	  //对第2个点tp2
	  for(j=oldMaxVertex;j<(int)m_Vertices.size();j++)
	  {
	   if(tp2 == m_Vertices[j].position)
	   {
		 pInV2=true;
		 index2=j;
		 break;
	   }
	  }
				
	   if (!pInV2)
	   {			
		//tp2不在数组中，添加
		vertex.position =tp2;	
		m_Vertices.push_back(vertex);
	    index2 = (int)m_Vertices.size()-1;
	   }

	  //对第3个点tp3
	  for(j=oldMaxVertex;j<(int)m_Vertices.size();j++)
	  {
	   if (tp3 == m_Vertices[j].position)
	   {
		pInV3=true;
		index3=j;
		break;
		}
	   }
	   if (!pInV3)
	   {
		//tp3不在数组中，添加
		vertex.position = tp3;	
		m_Vertices.push_back(vertex);
		index3 = (int)m_Vertices.size()-1;
	   }
	
	  //修改当前三角形objTriangle[i]的顶点值，并在三角形数组末添加三个三角形
	  
	  //添加三角形index1,objTriangle[i].v2,index2			
	  triangle.index[0] = index1;
	  triangle.index[1] = m_Faces[i].index[1];
	  triangle.index[2] = index2;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

      //添加三角形index2,objTriangle[i].v3,index3
				
	  triangle.index[0]=index2;
	  triangle.index[1]=m_Faces[i].index[2];			
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

	  //添加三角形index1,index2,index3
				
	  triangle.index[0]=index1;
	  triangle.index[1]=index2;
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

	  //修改当前三角形objTriangle[i]的顶点值为objTriangle[i].v1,index1,index3
	  triangle.index[0]=m_Faces[i].index[0];
	  triangle.index[1]=index1;
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces[i].index[0]=triangle.index[0];
	  m_Faces[i].index[1]=triangle.index[1];
	  m_Faces[i].index[2]=triangle.index[2];
	  m_Faces[i].area=triangle.area;

	 }	//（所有三角形遍历）			
  }
}

void ConstrainedFire::GenerateQuadrant()
{
  //将模型顶点分区（用矩形包围盒，三层分割）
  //第一次分区
  
  std::vector<int> s0,s1,s2,s3,s4,s5,s6;

  int end = (int)m_Vertices.size();
  for(int i=0;i<end;i++) s0.push_back(i);

  modelSplit(s0,s1,s2);

  //第二、三次分区
  modelSplit(s1,s3,s4);
  modelSplit(s3,m_Quadrant0,m_Quadrant1);
  modelSplit(s4,m_Quadrant2,m_Quadrant3);

  modelSplit(s2,s5,s6);
  modelSplit(s5,m_Quadrant4,m_Quadrant5);
  modelSplit(s6,m_Quadrant6,m_Quadrant7);

}


void ConstrainedFire::GenerateQuadrantByKMean()
{
  int end = (int)m_Vertices.size();

  int k= m_K;
  m_Quadrant.resize(k);

  for(int i=0;i<end;i++)
  { 
   m_Quadrant[m_Vertices[i].m_Id].push_back(i);
 }
}
bool ConstrainedFire::GenerateParticle(Particle * & particle)
{
 bool control = false;

 int k =m_K;

	int	objectIndex;	//当前欲定位的模型顶点索引
		
	//选取形状受控的粒子（为模型顶点分配粒子）
    
	if(m_MatchNum[currentEmitter]<(int)m_Quadrant[currentEmitter].size())
	 {
	   //设当前粒子为受控粒子
	   control = true;	
	   objectIndex = m_Quadrant[currentEmitter][m_MatchNum[currentEmitter]];
	   m_MatchNum[currentEmitter]++;  
	 }
	 else 
		 currentEmitter++;

	if(currentEmitter==m_K)currentEmitter=0;
	
	
	

	 //设置当前发射器
	

 	 if(!control)
	 {
		
	// if(currentEmitter==SEC_EMITTER) currentEmitter=0;
		 return false;//非约束粒子
	 }
	 //设置约束粒子属性
	if(control)
	particle = m_ParticleSystem->AddParticle();

	particle->controlled = true;
	
	
	//粒子生命值
	particle->t =  particle->tr =  1.2f +rsRandf(0.6)*0.5;
	particle->life = 1.0f;
	  
	particle->type = SHAPE;

	 //爆炸位置
	particle->xyz[0] = expEmitter[currentEmitter].explodePos.X();
	particle->xyz[1] = expEmitter[currentEmitter].explodePos.Y();
	particle->xyz[2] = expEmitter[currentEmitter].explodePos.Z();

	//爆炸物理属性
	particle->force = expEmitter[currentEmitter].force;//作用力(重力)
    particle->k = expEmitter[currentEmitter].k;//空气阻力

 	 ///对于x和z方向，为匀速运动，s=vt。
	 //对于y方向，加速度是重力加速度g, s=v0*t+1/2*gt^2
	 
	//基于物理，为新添加的形状受控粒子计算常量C
	 float speed,explodespeed;
     float d;	//XY平面上的角度，0-360
     float fi;	//与Z的夹角，0-180
     float ts;
	 float sx,sy,sz;
	 float rt,t2;

	 rt=1.0/particle->t;		//粒子生命值的倒数
	 t2=particle->t*particle->t;	//生命值的平方
	 
	 float T = particle->t;
	 float T0 = particle->t - particle->t*rsRandf(0.2)*0.5;//成形时间
	 float ktp1=particle->k*T+1.0;
     float w = 	(pow(T,ktp1) - pow(T - T0, ktp1));		

	 sx = m_Vertices[objectIndex].position.X()+m_Center.X()-particle->xyz[0];
	 sy = m_Vertices[objectIndex].position.Y()+m_Center.Y()-particle->xyz[1]-20;
	 sz = m_Vertices[objectIndex].position.Z()+m_Center.Z()-particle->xyz[2];

	 

	 particle->C.X()=sx*ktp1/w;
	 
     particle->C.Y()=(sy- particle->force.Y()*(2*T-T0)*T/(2*(ktp1-2.0)))  * (ktp1/w);

	 particle->C.Z()=sz*ktp1/w;
			
	 //为新添加的形状受控粒子计算速度的初始值
	 particle->vel[0] = particle->C.X();
	 particle->vel[1] = particle->force.Y()*T/(particle->k*T-1.0)+particle->C.Y()*pow(T,particle->k*T);
	 particle->vel[2] = particle->C.Z();
		
	 return true;
}

void ConstrainedFire::KMeans(int k)
{
 
  m_K = k;
  double t = 0.0001;
  double old_error, error = DBL_MAX;
  int n = m_Vertices.size();//顶点数目

  std::vector<Point3f> center;
  std::vector<Point3f> c1;//中间结果
  std::vector<int>  count;//聚类计数
  
  center.resize(k);
  c1.resize(k);
  count.resize(k);

  float diffX,diffY,diffZ;

  //初始化重心点
  for (int h =0,i = 0; i < k; h += n / k, i++) 
  { 
	center[i] = m_Vertices[h].position;//初始化中心点
  }

   do{
      old_error = error, error = 0;

      //清除聚类信息
      for(int i = 0; i < k;  i++) 
	  {
	    c1[i] = Point3f(0.0,0.0,0.0);
        count[i] = 0;
      }

	  //识别聚类顶点
      for (int h=0;h<n; h++) 
	  {
        double min_distance = DBL_MAX;

        for (int i =0; i<k; i++) //分析每个重心点
		{
          double distance = 0;
     
		  diffX = m_Vertices[h].position.X() - center[i].X();
		  diffY = m_Vertices[h].position.Y() - center[i].Y();
		  diffZ = m_Vertices[h].position.Z() - center[i].Z();

		  distance = diffX*diffX + diffY*diffY + diffZ*diffZ;
		   
		  if (distance < min_distance) 
		  {
            m_Vertices[h].m_Id = i;//标识顶点
            min_distance = distance;
           }
		}
         
		//记录数据
         c1[m_Vertices[h].m_Id].X() += m_Vertices[h].position.X();
		 c1[m_Vertices[h].m_Id].Y() += m_Vertices[h].position.Y();
		 c1[m_Vertices[h].m_Id].Z() += m_Vertices[h].position.Z();

         count[m_Vertices[h].m_Id]++;

         error += min_distance;
	  }

	  //更新k中心
      for (int i = 0; i < k; i++) 
	  {   
        center[i].X() = count[i] ? c1[i].X() / count[i] : c1[i].X();
		center[i].Y() = count[i] ? c1[i].Y() / count[i] : c1[i].Y();
		center[i].Z() = count[i] ? c1[i].Z() / count[i] : c1[i].Z(); 
      }

   } while (fabs(error - old_error) > t);


   for(int i=0;i<k;i++)
   {
    expEmitter[i].pos = center[i];
	
   }
}
//将模型按最长方向进行平均分割
void ConstrainedFire::modelSplit(std::vector<int> & source, std::vector<int> & des1,std::vector<int> & des2)
{
  float	centerX,centerY,centerZ;	//原始模型的中点坐标值
  float	wid,hei,dep;			//原始模型的长度、宽度、深度
  int  i;					//循环控制变量
		
  float maxx, minx, maxy, miny, maxz, minz;
	
  //计算范围
  maxx = minx = m_Vertices[source[0]].position.X();
  maxy = miny = m_Vertices[source[0]].position.Y();  
  maxz = minz = m_Vertices[source[0]].position.Z();

  for (i=1;i<(int)source.size();i++)
  {
   if(maxx < m_Vertices[source[i]].position.X())  maxx = m_Vertices[source[i]].position.X();
   if(minx > m_Vertices[source[i]].position.X())  minx = m_Vertices[source[i]].position.X();
   
   if(maxy < m_Vertices[source[i]].position.Y())  maxy = m_Vertices[source[i]].position.Y();
   if(miny > m_Vertices[source[i]].position.Y())  miny = m_Vertices[source[i]].position.Y();
        
   if(maxz < m_Vertices[source[i]].position.Z())  maxz = m_Vertices[source[i]].position.Z();
   if(minz > m_Vertices[source[i]].position.Z())  minz = m_Vertices[source[i]].position.Z(); 
  }
	
   wid = maxx-minx;   
   hei = maxy-miny;	
   dep = maxz-minz;

  //计算区域中心
	
   centerX = (maxx + minx) *0.5;
   centerY = (maxy + miny) *0.5;
   centerZ = (maxz + minz) *0.5;
	
	if(wid>=hei&&wid>=dep)
	{
	  //在x方向进行对半分割
	 for(i=0;i<(int)source.size(); i++)
	 {
	  if(m_Vertices[source[i]].position.X()<=centerX) des1.push_back(source[i]);
	  else  des2.push_back(source[i]);
	 }
	  return;
	}
	else if(hei>=wid&&hei>=dep)
	{
	  //在y方向进行对半分割
	 for(i=0;i<(int)source.size(); i++)
	 {
	if(m_Vertices[source[i]].position.Y()<=centerY) des1.push_back(source[i]);
	  else  des2.push_back(source[i]);
	  }
	  return;
	}
	else if(dep>=hei&&dep>=wid)
	{
	 //在z方向进行对半分割
	 for(i=0;i<(int)source.size(); i++)
	 {
	  if(m_Vertices[source[i]].position.Z()<=centerZ) des1.push_back(source[i]);
	  else  des2.push_back(source[i]);
	 }
	 return;
	}
}

Point3f ConstrainedFire::modelCenter(std::vector<int> & source)
{
	Point3f p;
	float	centerX,centerY,centerZ;	//模型的中点坐标值
	float	wid,hei,dep;			//原始模型的长度、宽度、深度
    int  i;					//循环控制变量
	GLfloat maxx, minx, maxy, miny, maxz, minz;

    /* get the max/mins */
	maxx = minx = m_Vertices[source[0]].position.X(); // not begin at 1+0,1+2,1+3
	maxy = miny = m_Vertices[source[0]].position.Y();
	maxz = minz = m_Vertices[source[0]].position.Z();
    for (i = 1; i < (int)source.size();i++)
	{
        if (maxx < m_Vertices[source[i]].position.X())
            maxx = m_Vertices[source[i]].position.X();
        if (minx > m_Vertices[source[i]].position.X())
            minx = m_Vertices[source[i]].position.X();

        if (maxy < m_Vertices[source[i]].position.Y())
            maxy = m_Vertices[source[i]].position.Y();
        if (miny > m_Vertices[source[i]].position.Y())
            miny = m_Vertices[source[i]].position.Y();
        
        if (maxz < m_Vertices[source[i]].position.Z())
            maxz = m_Vertices[source[i]].position.Z();
        if (minz > m_Vertices[source[i]].position.Z())
            minz = m_Vertices[source[i]].position.Z();
    }
	wid = maxx-minx;   
	hei = maxy-miny;	
	dep = maxz-minz;

	/* calculate center of the model */
	centerX = (maxx + minx) *0.5;
	centerY = (maxy + miny) *0.5;
	centerZ = (maxz + minz) *0.5;

	p.X()=centerX;
	p.Y()=centerY;
	p.Z()=centerZ;

	return p;
}

float ConstrainedFire::TriangleArea(int p1, int p2, int p3)
{
	Point3f point1,point2,point3;
	float	a,b,c,s;

	point1 = m_Vertices[p1].position;
	point2 = m_Vertices[p2].position;
	point3 = m_Vertices[p3].position;

	a = pointDist(point1,point2);	//三角形的边长
	b = pointDist(point2,point3);
	c = pointDist(point3,point1);
	s = (a+b+c)*0.5;	//周长的一半
	
	return sqrt(s*(s-a)*(s-b)*(s-c));	//计算面积
}
float ConstrainedFire::pointDist(Point3f point1, Point3f point2)
{
  return sqrt((point1.X()-point2.X())*(point1.X()-point2.X())+(point1.Y()-point2.Y())*(point1.Y()-point2.Y())+(point1.Z()-point2.Z())*(point1.Z()-point2.Z()));
}
Point3f ConstrainedFire::pointMid(int p1, int p2)
{
	Point3f point;
	Point3f point1,point2;
	point1.X() = m_Vertices[p1].position.X();
	point1.Y() = m_Vertices[p1].position.Y();
	point1.Z() = m_Vertices[p1].position.Z();
	point2.X() = m_Vertices[p2].position.X();
	point2.Y() = m_Vertices[p2].position.Y();
	point2.Z() = m_Vertices[p2].position.Z();

	point.X() = (point1.X() + point2.X())*0.5;
	point.Y() = (point1.Y() + point2.Y())*0.5;
	point.Z() = (point1.Z() + point2.Z())*0.5;
	
	return point;
}



