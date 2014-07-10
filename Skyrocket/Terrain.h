#ifndef _TERRAIN_
#define _TERRAIN_


#include<vector>
#include "PerlinNoise.h"

#include"../Math/Point3.h"

#ifndef _CAMERA_
#include"Camera.h"
#endif


struct TerrVertex//���ζ�������
{
  Point3f vertex;
  Point3f normal;
  float s, t;
};
struct Block//���ο�
{
  TerrVertex *vertices;
  int x, z;//���ο�����
  int numVertices;//��������
};

class Terrain 
{
 public:

	Terrain();

	~Terrain();

    void  Create();//��������
	void  Render(Camera * camera);//���Ƶ��Σ�pos�ǹ۲���λ��
	void  CreateTerrainLevel();//��������LOD
	void  CreateTerrainGeometry(Block * block);//�������μ���
	void  ReleaseBlock(Block * block);//�ͷŵ�������
	float HeightFunc(float x,float z);//�߶Ⱥ���
	float BlockDistance(int x,int z,rsVec pos);//���뺯��
	int   LevelSize(int size, int level);

	void CreateTexture(char *Filename);//��������
	

	std::vector<Block *> m_TerrainBlock; //���μ�������

	std::vector<unsigned int *> m_Indices;//���� 

	std::vector<int> m_TriangleSize;
	unsigned int m_TerrainTexture;//����

	unsigned int m_MaxBlocks;//�����ο���
    float m_BlockSize;//���ο��С(�߳�)
	float m_ViewDistance;//���ӷ�Χ 
	float m_LodFactor;//LOD����
	int m_BlockDensity;//���ο������ܶ�(������)
	int m_TexRepeat;//���ο������ظ���
	
};

#endif 
