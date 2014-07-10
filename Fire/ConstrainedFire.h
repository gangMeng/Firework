#ifndef _CONSTRAINEDFIRE_
#define _CONSTRAINEDFIRE_

#include"Point3.h"

#ifndef _OBJREADER_
#include"ObjReader.h"
#endif

#ifndef _EMITTER_
#include"Emitter.h"
#endif


#define SEC_EMITTER		45	//二次爆炸的发射器数量,形状可以受控
class ParticleSystem;
class Particle;
class FireVertex
{
public:
 FireVertex()
 {
  displayed = false;
 }

 Point3f position;//位置
 bool displayed;//是否已经显示
 int m_Id;//聚类标识
};

struct FireTriangle
{
 int index[3];//顶点索引

 float area;//三角形面积
};

class ConstrainedFire
{
  public:

	  bool m_Exploded;//是否已经爆炸
	  bool m_Boom;

  static ParticleSystem * m_ParticleSystem;//粒子系统

  ConstrainedFire();
  
  void InitI(Emitter * emitter);

  void SetPosition(Point3f center);//设定爆炸位置

  void Init(ObjReader * obj);//初始化烟花对象

  bool GenerateParticle(Particle * & particle);//产生新粒子

  void Normalize();//归一化顶点数据

  void Subdivide();//细分

  void GenerateQuadrant();//生成爆炸象限

  void GenerateQuadrantByKMean();

  void KMeans(int k);//k-means聚类

  void modelSplit(std::vector<int> & source, std::vector<int> & des1,std::vector<int> & des2);

  Point3f modelCenter(std::vector<int> & source);

  Point3f m_Center;

  Point3f rgb;//颜色

  int m_Ignitor;

  //辅助函数
  float TriangleArea(int p1, int p2, int p3);//计算三角形面积
  float pointDist(Point3f point1, Point3f point2);//两点间距离
  Point3f pointMid(int p1, int p2);//计算边中点

  int m_VertexThreshhold;//顶点数目阈值

  std::vector<FireVertex> m_Vertex;

  unsigned int		objVertexNum;			//导入对象的顶点数目

  bool	objectImported;			//标记是否已经读入外部对象


  int 	fireworkStage;//烟花对象的总体状态
							//0——上升阶段
							//1——第一次爆炸
							//2——第二次爆炸
							//3——成型阶段
							//4——中间爆炸阶段

  int		currentEmitter;		//2次爆炸的当前发射器编号
  Emitter	expEmitter[SEC_EMITTER];	//爆炸发射器
  int	  m_MatchNum[SEC_EMITTER];		//各子区域的当前匹配顶点值

  std::vector<FireVertex> m_Vertices;//顶点
  std::vector<FireTriangle> m_Faces;//面

  //目标位置分象限存储
  std::vector<int> m_Quadrant0;//象限0
  std::vector<int> m_Quadrant1;//象限1
  std::vector<int> m_Quadrant2;//象限2
  std::vector<int> m_Quadrant3;//象限3
  std::vector<int> m_Quadrant4;//象限4
  std::vector<int> m_Quadrant5;//象限5
  std::vector<int> m_Quadrant6;//象限6
  std::vector<int> m_Quadrant7;//象限7



  std::vector< std::vector<int> > m_Quadrant;//网格分区

};

#endif