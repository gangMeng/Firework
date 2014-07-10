#include"stdafx.h"
#include "Terrain.h"
#include<gl\glaux.h>

#define roundf(x) floorf((x) + 0.5f)

Terrain::Terrain()
{	
  m_MaxBlocks=10000;
  m_BlockSize=16384*4;
  m_BlockDensity=257;
  m_TexRepeat=128;
  m_TerrainTexture = -1;
  m_ViewDistance = 10000.0;
  m_LodFactor = 0.0013;

  //��ʼ��perlin����
  initPerlin();

}
Terrain::~Terrain()
{
  for(int i=0;i<(int)m_TerrainBlock.size();i++)
  {
	ReleaseBlock(m_TerrainBlock[i]);
  }

  m_TerrainBlock.clear();
}

void  Terrain::Create()
{

  CreateTerrainLevel();//��������

  CreateTexture("Grass.bmp");//��������

}
void  Terrain::Render(Camera * camera)
{
   ////////////////// ��̬����vertex buffer
	
   int minx = (int) roundf((camera->lookFrom[0][0] - m_ViewDistance) / m_BlockSize);
   int maxx = (int) roundf((camera->lookFrom[0][0] + m_ViewDistance) / m_BlockSize);
   int minz = (int) roundf((camera->lookFrom[0][2] - m_ViewDistance) / m_BlockSize);
   int maxz = (int) roundf((camera->lookFrom[0][2] + m_ViewDistance) / m_BlockSize);

   Block *block = NULL;

 
    glDisable(GL_BLEND);
    float fogColor[4] = { 0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f };

    glFogfv(GL_FOG_COLOR, fogColor);    
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.0001f);
    glFogf(GL_FOG_END,1.5);
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightDen[] = {0.8f, 0.1f, 0.9f, 1.0f};
	float lightDir[] = { -1.0f, 0.5f, 0.2f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightDen);

   for(int z = minz; z <= maxz; z++)
   {
	for(int x = minx; x <= maxx; x++)
	{
	 float d = BlockDistance(x, z, camera->lookFrom[0]);//������ο����

	 if(d>m_ViewDistance) continue;//��������,������
	 
	 bool found = false;

	 //�������ο�
	 for(int i=0;i<(int)m_TerrainBlock.size();i++)
	 {
	   if (m_TerrainBlock[i]->x == x && m_TerrainBlock[i]->z == z)
	   {
		 block = m_TerrainBlock[i];
		 found = true;
		 break;
	   }
	 }
	
	  if (!found)//δ�ҵ����µ�������
	  {
		block = new Block;//�����µ��ο�
		block->x = x;
		block->z = z;
		CreateTerrainGeometry(block);//������������
	  }
			
	 glEnableClientState(GL_VERTEX_ARRAY);
	 glEnableClientState(GL_NORMAL_ARRAY);
	 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	 //glEnable(GL_CULL_FACE);
	 //glCullFace(GL_BACK);

	 glBindTexture(GL_TEXTURE_2D, m_TerrainTexture);

	 char *vertices = (char *) block->vertices;
	 glVertexPointer  (3, GL_FLOAT, sizeof(TerrVertex), vertices);//����
	 glNormalPointer  (GL_FLOAT, sizeof(TerrVertex), vertices + sizeof(Point3f));//����
	 glTexCoordPointer(2,GL_FLOAT, sizeof(TerrVertex), vertices + sizeof(Point3f) * 2);//����

	  /////////////////////////////    LOD����
	  int level = (int) (m_LodFactor * d);

	  if (level >= (int)m_Indices.size()) level = (int)m_Indices.size() - 1;
	 
	  int lSize = LevelSize(m_BlockDensity, level);//����LOD������

	  glDrawElements(GL_TRIANGLES, m_TriangleSize[level]*3, GL_UNSIGNED_INT, m_Indices[level]);//����

	  }
    }

    glDisable(GL_CULL_FACE);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

  ////////// ���� ��̬vertex buffer

   while(m_TerrainBlock.size()>m_MaxBlocks)//���ο鳬��
   {
	 int maxIndex = 0;
	 float maxDist = 0;
	 for (unsigned int i = 0; i <m_TerrainBlock.size(); i++)
	 {
		 float l = (camera->lookFrom[0][0] - m_BlockSize * m_TerrainBlock[i]->x)*(camera->lookFrom[0][0] - m_BlockSize * m_TerrainBlock[i]->x) +
		         (camera->lookFrom[0][2] - m_BlockSize * m_TerrainBlock[i]->z)*(camera->lookFrom[0][2] - m_BlockSize * m_TerrainBlock[i]->z);

	  if (l > maxDist)
	  {
	    maxIndex = i;
		maxDist = l;
	  }
	   }
	   
	   ReleaseBlock(m_TerrainBlock[maxIndex]);//�ͷŵ��ο�

	   //���µ������ݿ�
	   std::vector<Block *> tempBlock;

	   for(int i=0;i<(int)m_TerrainBlock.size();i++)
	   {
		 if(i!=maxIndex) tempBlock.push_back(m_TerrainBlock[i]);
	   }
       m_TerrainBlock.clear();
	   for(int i=0;i<(int)tempBlock.size();i++)
	   m_TerrainBlock.push_back(tempBlock[i]);
	   tempBlock.clear();
	 
	}
	
}

void  Terrain::CreateTerrainLevel()
{

   //���� LOD index buffer

   int level = 0;
   do{
     
   int lSize = LevelSize(m_BlockDensity, level);

   unsigned int *dest,*indices;
	 
   dest = indices  = new unsigned int[(lSize - 1) * (lSize - 1)*6];//�������б�

    
   int step = 1 << level;

   for(int i=0;i<lSize-1;i++)//һ���������
   {
    for (int j = 0; j<lSize-1;j++)
    { 
	  //��������������
	 
	   (*dest) = i*m_BlockDensity* step+j* step + step;
	   dest++;
	   (*dest) = (i+1)*m_BlockDensity* step+j* step+step;
	   dest++;
	    (*dest) = i*m_BlockDensity* step+j* step; 
       dest++;

	    (*dest) = (i+1)*m_BlockDensity* step+j* step+step;
	   dest++;
	    (*dest) = (i+1)*m_BlockDensity* step+j* step;
	   dest++;
	   (*dest)= i*m_BlockDensity* step+j* step; 
       dest++;
	  
	   
 	 }
    }
	
    m_TriangleSize.push_back((lSize - 1) * (lSize - 1)*2);
    m_Indices.push_back(indices);

	level++; //LOD�ȼ�����
   }while(LevelSize(m_BlockDensity,level)>8);
  
}
void  Terrain::CreateTerrainGeometry(Block *block)
{
	HRESULT hr;
	

	TerrVertex * vertices = new TerrVertex[m_BlockDensity * m_BlockDensity];//��������

	///////////////////////    �����������

	int pDensity = m_BlockDensity + 2;
	float *paddedHeights = new float[pDensity * pDensity];

	float *pDest = paddedHeights;

	float minX = (block->x - 0.5f) * m_BlockSize;
	float minZ = (block->z - 0.5f) * m_BlockSize;
	float w = m_BlockSize / (m_BlockDensity - 1);

    TerrVertex *dest = vertices;
	//�������꣬��������

	for(int z=-1; z<= m_BlockDensity; z++)
	{
	  float fz = minZ + z * w;
	  for (int x = -1; x <= m_BlockDensity; x++)
	  {
	    float fx = minX + x * w;
		float h = 1.5*HeightFunc(fx, fz) -20  ;

	    *pDest++ = h;

		if(x >= 0 && x < m_BlockDensity && z >= 0 && z < m_BlockDensity)
		{
		   dest->vertex = Point3f(fx, h, fz);
		   dest->s = m_TexRepeat * float(x) / (m_BlockDensity - 1);
		   dest->t = m_TexRepeat * float(z) / (m_BlockDensity - 1);
		   dest++;
		}
	 }
	}

	// Apply Sobel filter to find mesh normals ���㷨��

	dest = vertices;
	w *= w;
	int i, j;
	for(j = 1; j < pDensity-1; j++)
	{
	  for(i=1; i < pDensity-1; i++)
	  {
	     
	     dest->normal.X() = paddedHeights[(j - 1) * pDensity + (i - 1)]  - paddedHeights[(j - 1) * pDensity + (i + 1)]  +
				              2 * (paddedHeights[ j* pDensity + (i - 1)] - paddedHeights[ j* pDensity + (i + 1)]) +
					          paddedHeights[(j + 1) * pDensity + (i - 1)]  - paddedHeights[(j + 1) * pDensity + (i + 1)];

	     dest->normal.Y() = w;

		 dest->normal.Z() = paddedHeights[(j - 1) * pDensity + (i - 1)]  - paddedHeights[(j + 1) * pDensity + (i - 1)]  +
				2 * (paddedHeights[(j - 1) * pDensity + i]  - paddedHeights[(j + 1) * pDensity + i]) +
					 paddedHeights[(j - 1) * pDensity + (i + 1)]  - paddedHeights[(j + 1) * pDensity + (i + 1)];

		 dest->normal.Normalize();//��λ��

		 dest++;
		}
	}

	//���ƶ�������
    
   
	block->vertices = vertices;
	block->numVertices = m_BlockDensity*m_BlockDensity;

	m_TerrainBlock.push_back(block);//�洢���ο�


    //�ͷ���Դ
    delete paddedHeights;

}
void  Terrain::ReleaseBlock(Block *block)
{
   if(block->vertices!=NULL)	  
   delete block->vertices;
   delete block;
}

float Terrain::HeightFunc(const float x, const float z)
{
   return 256 * noise2(0.0031212312f * x, 0.004613212f * z) + 32 * noise2(0.017412412f * x, 0.01752341f * z);
}
float Terrain::BlockDistance(int x,int z, rsVec pos)
{
	// Find the square of the distance from the sphere to the box

	float d = 0;

	float minX = (x - 0.5f) * m_BlockSize;
	float maxX = (x + 0.5f) * m_BlockSize;
	float minZ = (z - 0.5f) * m_BlockSize;
	float maxZ = (z + 0.5f) * m_BlockSize;

	if (pos[0] < minX){
		d += (pos[0] - minX)*(pos[0] - minX);
	} else if (pos[0] > maxX){
		d += (pos[0] - maxX)*(pos[0] - maxX);
	}

	if (pos[2] < minZ)
	{
		d += (pos[2] - minZ)*(pos[2] - minZ);
	} 
	else if (pos[2] > maxZ){
		d += (pos[2] - maxZ)*(pos[2] - maxZ);
	}

	return sqrt(d);
}

int   Terrain::LevelSize(int size, int level)
{
  return ((size - 1) >> level) + 1;
}


void  Terrain::CreateTexture(char *Filename)
{	
   AUX_RGBImageRec *pBitmap = auxDIBImageLoadA(Filename);//����bmpͼƬ		
   glGenTextures(1,&m_TerrainTexture);
   glBindTexture(GL_TEXTURE_2D, m_TerrainTexture);
    
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, pBitmap->sizeX, pBitmap->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data); 
	  
	//�ͷ��ڴ�
	if (pBitmap)							
	{
	 if (pBitmap->data)					
	 {
	   free(pBitmap->data);				
	  }
	  free(pBitmap);							
	}
	

}
