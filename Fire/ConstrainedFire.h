#ifndef _CONSTRAINEDFIRE_
#define _CONSTRAINEDFIRE_

#include"Point3.h"

#ifndef _OBJREADER_
#include"ObjReader.h"
#endif

#ifndef _EMITTER_
#include"Emitter.h"
#endif


#define SEC_EMITTER		45	//���α�ը�ķ���������,��״�����ܿ�
class ParticleSystem;
class Particle;
class FireVertex
{
public:
 FireVertex()
 {
  displayed = false;
 }

 Point3f position;//λ��
 bool displayed;//�Ƿ��Ѿ���ʾ
 int m_Id;//�����ʶ
};

struct FireTriangle
{
 int index[3];//��������

 float area;//���������
};

class ConstrainedFire
{
  public:

	  bool m_Exploded;//�Ƿ��Ѿ���ը
	  bool m_Boom;

  static ParticleSystem * m_ParticleSystem;//����ϵͳ

  ConstrainedFire();
  
  void InitI(Emitter * emitter);

  void SetPosition(Point3f center);//�趨��ըλ��

  void Init(ObjReader * obj);//��ʼ���̻�����

  bool GenerateParticle(Particle * & particle);//����������

  void Normalize();//��һ����������

  void Subdivide();//ϸ��

  void GenerateQuadrant();//���ɱ�ը����

  void GenerateQuadrantByKMean();

  void KMeans(int k);//k-means����

  void modelSplit(std::vector<int> & source, std::vector<int> & des1,std::vector<int> & des2);

  Point3f modelCenter(std::vector<int> & source);

  Point3f m_Center;

  Point3f rgb;//��ɫ

  int m_Ignitor;

  //��������
  float TriangleArea(int p1, int p2, int p3);//�������������
  float pointDist(Point3f point1, Point3f point2);//��������
  Point3f pointMid(int p1, int p2);//������е�

  int m_VertexThreshhold;//������Ŀ��ֵ

  std::vector<FireVertex> m_Vertex;

  unsigned int		objVertexNum;			//�������Ķ�����Ŀ

  bool	objectImported;			//����Ƿ��Ѿ������ⲿ����


  int 	fireworkStage;//�̻����������״̬
							//0���������׶�
							//1������һ�α�ը
							//2�����ڶ��α�ը
							//3�������ͽ׶�
							//4�����м䱬ը�׶�

  int		currentEmitter;		//2�α�ը�ĵ�ǰ���������
  Emitter	expEmitter[SEC_EMITTER];	//��ը������
  int	  m_MatchNum[SEC_EMITTER];		//��������ĵ�ǰƥ�䶥��ֵ

  std::vector<FireVertex> m_Vertices;//����
  std::vector<FireTriangle> m_Faces;//��

  //Ŀ��λ�÷����޴洢
  std::vector<int> m_Quadrant0;//����0
  std::vector<int> m_Quadrant1;//����1
  std::vector<int> m_Quadrant2;//����2
  std::vector<int> m_Quadrant3;//����3
  std::vector<int> m_Quadrant4;//����4
  std::vector<int> m_Quadrant5;//����5
  std::vector<int> m_Quadrant6;//����6
  std::vector<int> m_Quadrant7;//����7



  std::vector< std::vector<int> > m_Quadrant;//�������

};

#endif