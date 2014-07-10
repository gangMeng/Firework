#ifndef _TERRAIN_
#define _TERRAIN_


#include<vector>
#include "PerlinNoise.h"

#include"../Math/Point3.h"

#ifndef _CAMERA_
#include"Camera.h"
#endif


struct TerrVertex//地形顶点数据
{
  Point3f vertex;
  Point3f normal;
  float s, t;
};
struct Block//地形块
{
  TerrVertex *vertices;
  int x, z;//地形块坐标
  int numVertices;//顶点数量
};

class Terrain 
{
 public:

	Terrain();

	~Terrain();

    void  Create();//创建地形
	void  Render(Camera * camera);//绘制地形，pos是观察者位置
	void  CreateTerrainLevel();//创建地形LOD
	void  CreateTerrainGeometry(Block * block);//创建地形几何
	void  ReleaseBlock(Block * block);//释放地形数据
	float HeightFunc(float x,float z);//高度函数
	float BlockDistance(int x,int z,rsVec pos);//距离函数
	int   LevelSize(int size, int level);

	void CreateTexture(char *Filename);//创建纹理
	

	std::vector<Block *> m_TerrainBlock; //地形几何数据

	std::vector<unsigned int *> m_Indices;//索引 

	std::vector<int> m_TriangleSize;
	unsigned int m_TerrainTexture;//纹理

	unsigned int m_MaxBlocks;//最大地形块数
    float m_BlockSize;//地形块大小(边长)
	float m_ViewDistance;//可视范围 
	float m_LodFactor;//LOD参数
	int m_BlockDensity;//地形块网格密度(顶点数)
	int m_TexRepeat;//地形块纹理重复数
	
};

#endif 
