#include"stdafx.h"
#include"ConstrainedFire.h"

#ifndef _PARTICLESYSTEM_
#include"../Skyrocket/ParticleSystem.h"
#endif
ParticleSystem * ConstrainedFire::m_ParticleSystem = NULL;

const int HALF_RAND = (RAND_MAX / 2);

GLuint     objectDisplayed = 0;


int m_K = 45;

int	numParticlesSecondExp;	//�ڶ��α�ը�������������
float RandomNum()
{
	int rn;
	rn = rand();	//����α�����
	return ((float)(rn - HALF_RAND) / (float)HALF_RAND);
					//����-1��1֮��������
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


	//���ö��α�ը�����ӷ�����
	for (i=0;i<SEC_EMITTER;i++)
	{
/*		expEmitter[i].explodePos.x=emitter->explodePos.x+RandomNum()*1.5;
		expEmitter[i].explodePos.y=emitter->explodePos.y+RandomNum();
		expEmitter[i].explodePos.z=emitter->explodePos.z+RandomNum()*0.1;
*/
		//ע�⣬RandomNum()����-1��1֮��������
//		expEmitter[i].explodeColor.r=(RandomNum()+1.0)*0.25+0.5;	//0.5��1
//		expEmitter[i].explodeColor.g=(RandomNum()+1.0)*0.25;		//0��0.5
//		expEmitter[i].explodeColor.b=(RandomNum()+1.0)*0.25+0.5;	//0.5��1
		expEmitter[i].explodeColor[0]=(RandomNum()+1.0)*0.5;	//0��1
		expEmitter[i].explodeColor[1]=(RandomNum()+1.0)*0.5;	//0��1
		expEmitter[i].explodeColor[2]=(RandomNum()+1.0)*0.5;	//0��1
		expEmitter[i].explodeColor[3]=0.5;

		expEmitter[i].explodeSpeed=(emitter->secondExpSpeed);

		expEmitter[i].secCount=0;	//ÿ���������ѷ���������

	}
}

void ConstrainedFire::Init(ObjReader * obj)
{
   FireVertex  vertex;
   FireTriangle triangle;
   int end;

   //�洢ģ�Ͷ���
   end = (int)obj->Positions.size();
   
   for(int i=0;i<end;i++)
   {
	vertex.position = obj->Positions[i]*2.6; 
	m_Vertices.push_back(vertex);
   }

   //�洢ģ������Ϣ
   end = (int) obj->m_Object[0]->posIndices.size();
   for(int i=0;i<end;)
   {
	triangle.index[0] = obj->m_Object[0]->posIndices[i++];
	triangle.index[1] = obj->m_Object[0]->posIndices[i++];
	triangle.index[2] = obj->m_Object[0]->posIndices[i++];
	triangle.area = TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	m_Faces.push_back(triangle);
   }

   Normalize();//��һ����������

   Subdivide();//ϸ��ģ��

   objectImported =true;
   objVertexNum = (int)m_Vertices.size();

   numParticlesSecondExp=m_Vertices.size()*1.8;

   KMeans(m_K);//�������

   GenerateQuadrantByKMean();//���ɷ�������

   //8�����α�ը�����ӷ�������λ������,ע��˳��Ҫ����󶥵������Ŷ�Ӧ
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
  //�������ݹ�һ��
	
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

   //����ģ������	  
   cx = (maxx + minx) *0.5;
   cy = (maxy + miny) *0.5;
   cz = (maxz + minz) *0.5;

   //��������ϵ��

   if(h>=d&&h>=w) scale = 1.0/h;
   else if(d>=h&&d>=w)scale = 1.5/d;
   else if(w>=h&&w>=d)scale = 1.5/w;

   //�任ģ��

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
   bool	pInV1,pInV2,pInV3;	//��ǵ��Ƿ�����ģ�͵Ķ���������
   int  index1,index2,index3;	//�����ӵĶ�������ֵ
   Point3f tp1,tp2,tp3;

   while(m_Vertices.size()<m_VertexThreshhold)	
   {
	 //���������������ƽ��ֵ
	 float	aveArea=0.0;
	
	 oldMaxVertex = (int)m_Vertices.size();	//ϸ��֮ǰ��ģ�Ͷ����������ڶ��������Ŀ�ʼλ��
	 oldMaxTriangle = (int)m_Faces.size();	//�����������֮ǰ�������θ������������ӵ������β���ϸ��

	 for (i=0;i<oldMaxTriangle;i++)	aveArea += m_Faces[i].area;
	 
	 aveArea /= (float)oldMaxTriangle;

	 for (i=0;i<oldMaxTriangle;i++)	//�������ӵ������β������жϺ�ϸ��
	 {
	   //���������ƽ��ֵ�������Σ��������е�����
	   if(m_Faces[i].area < aveArea) continue;
	   
		 //��������е�	
	   tp1=pointMid(m_Faces[i].index[0],m_Faces[i].index[1]);
	   tp2=pointMid(m_Faces[i].index[1],m_Faces[i].index[2]);
	   tp3=pointMid(m_Faces[i].index[2],m_Faces[i].index[0]);

	   //�жϸ��е��Ƿ����ڶ��������С���û�У����е���ӵ���������ĩ
	   //���ڣ��������ܻ������������ε�ϸ���������ѱ���ӣ�����ֻ���oldMaxVertex��ʼ����
		 
	   pInV1=false;	//��ʼ״̬Ϊ����������
	   pInV2=false;
	   pInV3=false;
		 
	  //�Ե�1����tp1
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
	   //tp1���������У���ӵ�����ĩβ
	   vertex.position = tp1;
	   m_Vertices.push_back(vertex);
	   index1 = (int)m_Vertices.size()-1;
	  }
				
	  //�Ե�2����tp2
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
		//tp2���������У����
		vertex.position =tp2;	
		m_Vertices.push_back(vertex);
	    index2 = (int)m_Vertices.size()-1;
	   }

	  //�Ե�3����tp3
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
		//tp3���������У����
		vertex.position = tp3;	
		m_Vertices.push_back(vertex);
		index3 = (int)m_Vertices.size()-1;
	   }
	
	  //�޸ĵ�ǰ������objTriangle[i]�Ķ���ֵ����������������ĩ�������������
	  
	  //���������index1,objTriangle[i].v2,index2			
	  triangle.index[0] = index1;
	  triangle.index[1] = m_Faces[i].index[1];
	  triangle.index[2] = index2;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

      //���������index2,objTriangle[i].v3,index3
				
	  triangle.index[0]=index2;
	  triangle.index[1]=m_Faces[i].index[2];			
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

	  //���������index1,index2,index3
				
	  triangle.index[0]=index1;
	  triangle.index[1]=index2;
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces.push_back(triangle);

	  //�޸ĵ�ǰ������objTriangle[i]�Ķ���ֵΪobjTriangle[i].v1,index1,index3
	  triangle.index[0]=m_Faces[i].index[0];
	  triangle.index[1]=index1;
	  triangle.index[2]=index3;
	  triangle.area=TriangleArea(triangle.index[0],triangle.index[1],triangle.index[2]);
	  m_Faces[i].index[0]=triangle.index[0];
	  m_Faces[i].index[1]=triangle.index[1];
	  m_Faces[i].index[2]=triangle.index[2];
	  m_Faces[i].area=triangle.area;

	 }	//�����������α�����			
  }
}

void ConstrainedFire::GenerateQuadrant()
{
  //��ģ�Ͷ���������þ��ΰ�Χ�У�����ָ
  //��һ�η���
  
  std::vector<int> s0,s1,s2,s3,s4,s5,s6;

  int end = (int)m_Vertices.size();
  for(int i=0;i<end;i++) s0.push_back(i);

  modelSplit(s0,s1,s2);

  //�ڶ������η���
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

	int	objectIndex;	//��ǰ����λ��ģ�Ͷ�������
		
	//ѡȡ��״�ܿص����ӣ�Ϊģ�Ͷ���������ӣ�
    
	if(m_MatchNum[currentEmitter]<(int)m_Quadrant[currentEmitter].size())
	 {
	   //�赱ǰ����Ϊ�ܿ�����
	   control = true;	
	   objectIndex = m_Quadrant[currentEmitter][m_MatchNum[currentEmitter]];
	   m_MatchNum[currentEmitter]++;  
	 }
	 else 
		 currentEmitter++;

	if(currentEmitter==m_K)currentEmitter=0;
	
	
	

	 //���õ�ǰ������
	

 	 if(!control)
	 {
		
	// if(currentEmitter==SEC_EMITTER) currentEmitter=0;
		 return false;//��Լ������
	 }
	 //����Լ����������
	if(control)
	particle = m_ParticleSystem->AddParticle();

	particle->controlled = true;
	
	
	//��������ֵ
	particle->t =  particle->tr =  1.2f +rsRandf(0.6)*0.5;
	particle->life = 1.0f;
	  
	particle->type = SHAPE;

	 //��ըλ��
	particle->xyz[0] = expEmitter[currentEmitter].explodePos.X();
	particle->xyz[1] = expEmitter[currentEmitter].explodePos.Y();
	particle->xyz[2] = expEmitter[currentEmitter].explodePos.Z();

	//��ը��������
	particle->force = expEmitter[currentEmitter].force;//������(����)
    particle->k = expEmitter[currentEmitter].k;//��������

 	 ///����x��z����Ϊ�����˶���s=vt��
	 //����y���򣬼��ٶ����������ٶ�g, s=v0*t+1/2*gt^2
	 
	//��������Ϊ����ӵ���״�ܿ����Ӽ��㳣��C
	 float speed,explodespeed;
     float d;	//XYƽ���ϵĽǶȣ�0-360
     float fi;	//��Z�ļнǣ�0-180
     float ts;
	 float sx,sy,sz;
	 float rt,t2;

	 rt=1.0/particle->t;		//��������ֵ�ĵ���
	 t2=particle->t*particle->t;	//����ֵ��ƽ��
	 
	 float T = particle->t;
	 float T0 = particle->t - particle->t*rsRandf(0.2)*0.5;//����ʱ��
	 float ktp1=particle->k*T+1.0;
     float w = 	(pow(T,ktp1) - pow(T - T0, ktp1));		

	 sx = m_Vertices[objectIndex].position.X()+m_Center.X()-particle->xyz[0];
	 sy = m_Vertices[objectIndex].position.Y()+m_Center.Y()-particle->xyz[1]-20;
	 sz = m_Vertices[objectIndex].position.Z()+m_Center.Z()-particle->xyz[2];

	 

	 particle->C.X()=sx*ktp1/w;
	 
     particle->C.Y()=(sy- particle->force.Y()*(2*T-T0)*T/(2*(ktp1-2.0)))  * (ktp1/w);

	 particle->C.Z()=sz*ktp1/w;
			
	 //Ϊ����ӵ���״�ܿ����Ӽ����ٶȵĳ�ʼֵ
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
  int n = m_Vertices.size();//������Ŀ

  std::vector<Point3f> center;
  std::vector<Point3f> c1;//�м���
  std::vector<int>  count;//�������
  
  center.resize(k);
  c1.resize(k);
  count.resize(k);

  float diffX,diffY,diffZ;

  //��ʼ�����ĵ�
  for (int h =0,i = 0; i < k; h += n / k, i++) 
  { 
	center[i] = m_Vertices[h].position;//��ʼ�����ĵ�
  }

   do{
      old_error = error, error = 0;

      //���������Ϣ
      for(int i = 0; i < k;  i++) 
	  {
	    c1[i] = Point3f(0.0,0.0,0.0);
        count[i] = 0;
      }

	  //ʶ����ඥ��
      for (int h=0;h<n; h++) 
	  {
        double min_distance = DBL_MAX;

        for (int i =0; i<k; i++) //����ÿ�����ĵ�
		{
          double distance = 0;
     
		  diffX = m_Vertices[h].position.X() - center[i].X();
		  diffY = m_Vertices[h].position.Y() - center[i].Y();
		  diffZ = m_Vertices[h].position.Z() - center[i].Z();

		  distance = diffX*diffX + diffY*diffY + diffZ*diffZ;
		   
		  if (distance < min_distance) 
		  {
            m_Vertices[h].m_Id = i;//��ʶ����
            min_distance = distance;
           }
		}
         
		//��¼����
         c1[m_Vertices[h].m_Id].X() += m_Vertices[h].position.X();
		 c1[m_Vertices[h].m_Id].Y() += m_Vertices[h].position.Y();
		 c1[m_Vertices[h].m_Id].Z() += m_Vertices[h].position.Z();

         count[m_Vertices[h].m_Id]++;

         error += min_distance;
	  }

	  //����k����
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
//��ģ�Ͱ���������ƽ���ָ�
void ConstrainedFire::modelSplit(std::vector<int> & source, std::vector<int> & des1,std::vector<int> & des2)
{
  float	centerX,centerY,centerZ;	//ԭʼģ�͵��е�����ֵ
  float	wid,hei,dep;			//ԭʼģ�͵ĳ��ȡ���ȡ����
  int  i;					//ѭ�����Ʊ���
		
  float maxx, minx, maxy, miny, maxz, minz;
	
  //���㷶Χ
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

  //������������
	
   centerX = (maxx + minx) *0.5;
   centerY = (maxy + miny) *0.5;
   centerZ = (maxz + minz) *0.5;
	
	if(wid>=hei&&wid>=dep)
	{
	  //��x������ж԰�ָ�
	 for(i=0;i<(int)source.size(); i++)
	 {
	  if(m_Vertices[source[i]].position.X()<=centerX) des1.push_back(source[i]);
	  else  des2.push_back(source[i]);
	 }
	  return;
	}
	else if(hei>=wid&&hei>=dep)
	{
	  //��y������ж԰�ָ�
	 for(i=0;i<(int)source.size(); i++)
	 {
	if(m_Vertices[source[i]].position.Y()<=centerY) des1.push_back(source[i]);
	  else  des2.push_back(source[i]);
	  }
	  return;
	}
	else if(dep>=hei&&dep>=wid)
	{
	 //��z������ж԰�ָ�
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
	float	centerX,centerY,centerZ;	//ģ�͵��е�����ֵ
	float	wid,hei,dep;			//ԭʼģ�͵ĳ��ȡ���ȡ����
    int  i;					//ѭ�����Ʊ���
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

	a = pointDist(point1,point2);	//�����εı߳�
	b = pointDist(point2,point3);
	c = pointDist(point3,point1);
	s = (a+b+c)*0.5;	//�ܳ���һ��
	
	return sqrt(s*(s-a)*(s-b)*(s-c));	//�������
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



