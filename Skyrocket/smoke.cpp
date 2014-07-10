#include"stdafx.h"
#include "smoke.h"
#include "smoketex.h"

void Smoke::initSmoke(int smoke,int explosion)
{

   m_Smoke =smoke;
  
   m_Explosionsmoke = explosion;

	int i, j;

	unsigned char smoke1[SMOKETEXSIZE][SMOKETEXSIZE][2];
	unsigned char smoke2[SMOKETEXSIZE][SMOKETEXSIZE][2];
	unsigned char smoke3[SMOKETEXSIZE][SMOKETEXSIZE][2];
	unsigned char smoke4[SMOKETEXSIZE][SMOKETEXSIZE][2];
	unsigned char smoke5[SMOKETEXSIZE][SMOKETEXSIZE][2];

	for(i=0; i<SMOKETEXSIZE; i++)
	{
	 for(j=0; j<SMOKETEXSIZE; j++)
	 {
	   smoke1[i][j][0] = 255;
	   smoke2[i][j][0] = 255;
	   smoke3[i][j][0] = 255;
	   smoke4[i][j][0] = 255;
	   smoke5[i][j][0] = 255;
	   smoke1[i][j][1] = presmoke1[i][j];
	   smoke2[i][j][1] = presmoke2[i][j];
	   smoke3[i][j][1] = presmoke3[i][j];
	   smoke4[i][j][1] = presmoke4[i][j];
	   smoke5[i][j][1] = presmoke5[i][j];
	 }
	}
	
	glGenTextures(5, smoketex);

	glBindTexture(GL_TEXTURE_2D, smoketex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 2, SMOKETEXSIZE, SMOKETEXSIZE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, smoke1);

	glBindTexture(GL_TEXTURE_2D, smoketex[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 2, SMOKETEXSIZE, SMOKETEXSIZE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, smoke2);

	glBindTexture(GL_TEXTURE_2D, smoketex[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 2, SMOKETEXSIZE, SMOKETEXSIZE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, smoke3);

	glBindTexture(GL_TEXTURE_2D, smoketex[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 2, SMOKETEXSIZE, SMOKETEXSIZE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, smoke4);

	glBindTexture(GL_TEXTURE_2D, smoketex[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 2, SMOKETEXSIZE, SMOKETEXSIZE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, smoke5);

	smokelist[0] = glGenLists(5);
	smokelist[1] = smokelist[0] + 1;
	smokelist[2] = smokelist[0] + 2;
	smokelist[3] = smokelist[0] + 3;
	smokelist[4] = smokelist[0] + 4;

	for(i=0; i<5; i++)
	{	
	 glNewList(smokelist[i], GL_COMPILE);
	 glBindTexture(GL_TEXTURE_2D, smoketex[i]);
	 glBegin(GL_TRIANGLE_STRIP);
	 glTexCoord2f(0.0f, 0.0f);
	 glVertex3f(-0.5f, -0.5f, 0.0f);
	 glTexCoord2f(1.0f, 0.0f);
	 glVertex3f(0.5f, -0.5f, 0.0f);
	 glTexCoord2f(0.0f, 1.0f);
	 glVertex3f(-0.5f, 0.5f, 0.0f);
	 glTexCoord2f(1.0f, 1.0f);
	 glVertex3f(0.5f, 0.5f, 0.0f);
	 glEnd();	
	 glEndList();
	}

	// set smoke lifespans  ( 1 2 1 4 1 2 1 8 )
	// This deserves a little more explanation:  smoke particles in this saver expand
	// over time.  If they all have the same lifespans, then they overlap too much and
	// that looks bad.  If every other particle fades out, the remaining ones have more
	// room to expand into.  So we use these smoke times to halve the number of particles
	// a few times.
	smokeTime[0] = smokeTime[2] = smokeTime[4] = smokeTime[6] = 0.4f;
	smokeTime[1] = smokeTime[5] = 0.8f;
	smokeTime[3] = 2.0f;
	smokeTime[7] = 4.0f;
	for(i=0; i<SMOKETIMES; i++)
	{
	 if(smokeTime[i] > float(m_Smoke)) smokeTime[i] = float(m_Smoke);
	}

	if(smokeTime[7] < float(m_Smoke)) smokeTime[7] = float(m_Smoke);

	// create table describing which particles will emit smoke
	// 0 = don't emit smoke
	// 1 = emit smoke
	for(i=0; i<WHICHSMOKES; i++) whichSmoke[i] = 0;
	
	if(m_Explosionsmoke)
	{
	  float index = float(WHICHSMOKES) / float(m_Explosionsmoke);
	  for(i=0; i<m_Explosionsmoke; i++) whichSmoke[int(float(i) * index)] = 1;
	}
}