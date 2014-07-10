#include"stdafx.h"
#include "flare.h"

#ifndef _CAMERA_
#include"Camera.h"
#endif

void Flare::initFlares()
{
   int i, j;
   float x, y;
   float temp;

	glGenTextures(4, flaretex);

	//flare 1: sphere
	for(i=0; i<FLARESIZE; i++)
	{
	  for(j=0; j<FLARESIZE; j++)
	  {
		flare1[i][j][0] = 255;
		flare1[i][j][1] = 255;
		flare1[i][j][2] = 255;
		x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
		y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
		temp = 1.0f - ((x * x) + (y * y));
		if(temp > 1.0f) temp = 1.0f;
		if(temp < 0.0f) temp = 0.0f;
		flare1[i][j][3] = (unsigned char)(255.0f * temp * temp);
	   }
	}
	glBindTexture(GL_TEXTURE_2D, flaretex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,GL_UNSIGNED_BYTE, flare1);

	//flare 2:  flattened sphere
	for(i=0; i<FLARESIZE; i++)
	{
	 for(j=0; j<FLARESIZE; j++)
	 {
	   flare2[i][j][0] = 255;
	   flare2[i][j][1] = 255;
	   flare2[i][j][2] = 255;
	   x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
	   y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
	   temp = 2.5f * (1.0f - ((x * x) + (y * y)));
	   if(temp > 1.0f) temp = 1.0f;
	   if(temp < 0.0f) temp = 0.0f;		
	   flare2[i][j][3] = (unsigned char)(255.0f * temp);
	  }
	}
	glBindTexture(GL_TEXTURE_2D, flaretex[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,GL_UNSIGNED_BYTE, flare2);

	//flare 3:  torus
	for(i=0; i<FLARESIZE; i++)
	{
	  for(j=0; j<FLARESIZE; j++)
	  {
		flare3[i][j][0] = 255;
		flare3[i][j][1] = 255;
		flare3[i][j][2] = 255;
		x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
		y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
		temp = 4.0f * ((x * x) + (y * y)) * (1.0f - ((x * x) + (y * y)));
		if(temp > 1.0f) temp = 1.0f;
		if(temp < 0.0f) temp = 0.0f;
		temp = temp * temp * temp * temp;
		flare3[i][j][3] = (unsigned char)(255.0f * temp);
		}
	}
	glBindTexture(GL_TEXTURE_2D, flaretex[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,GL_UNSIGNED_BYTE, flare3);

	//flare 4:  weird flare
	for(i=0; i<FLARESIZE; i++)
	{
	 for(j=0; j<FLARESIZE; j++)
	 {
		x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
		if(x < 0.0f) x = -x;
		y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
		if(y < 0.0f) y = -y;
		flare4[i][j][0] = 255;
		flare4[i][j][1] = 255;
		temp = 0.14f * (1.0f - max(x, y)) / max((x * y), 0.05f);
		if(temp > 1.0f) temp = 1.0f;
		if(temp < 0.0f) temp = 0.0f;
		flare4[i][j][2] = (unsigned char)(255.0f * temp);
		temp = 0.1f * (1.0f - max(x, y)) / max((x * y), 0.1f);
		if(temp > 1.0f) temp = 1.0f;
		if(temp < 0.0f) temp = 0.0f;
		flare4[i][j][3] = (unsigned char)(255.0f * temp);
	 }
	}
	glBindTexture(GL_TEXTURE_2D, flaretex[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,GL_UNSIGNED_BYTE, flare4);

	// Build display lists
	flarelist[0] = glGenLists(4);
	flarelist[1] = flarelist[0] + 1;
	flarelist[2] = flarelist[0] + 2;
	flarelist[3] = flarelist[0] + 3;
	for(i=0; i<4; i++)
	{
	  glNewList(flarelist[i], GL_COMPILE);
	  glBindTexture(GL_TEXTURE_2D, flaretex[i]);
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
}

void Flare::flare(Camera * camera,float x, float y, float red, float green, float blue, float alpha)
{
   float dx, dy;
   float fadewidth, temp;

   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glEnable(GL_BLEND);

	//����������Ļ����
	fadewidth = float(camera->xsize) / 10.0f;
	if(y < 0)
	{
	  temp = fadewidth + y;
	  if(temp < 0.0f)
	  return;
	  alpha *= temp / fadewidth;
	}
	if(y > camera->ysize)
	{
	  temp = fadewidth - y + camera->ysize;
	  if(temp < 0.0f)
	  return;
	  alpha *= temp / fadewidth;
	}
	if(x < 0)
	{
	  temp = fadewidth + x;
	  if(temp < 0.0f)
	  return;
	  alpha *= temp / fadewidth;
	}
	if(x > camera->xsize)
	{
	  temp = fadewidth - x + camera->xsize;
	  if(temp < 0.0f)
	  return;
	  alpha *= temp / fadewidth;
	}

	// ��Դ����Ļ��������
	
	dx = 0.5f * camera->aspectRatio - x;
	dy = 0.5f - y;
	
	// Setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, camera->aspectRatio, 0, 1.0f);

	// Draw stuff
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// wide flare
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(5.0f * alpha, 0.05f * alpha, 1.0f);
	glColor4f(red * 0.25f, green * 0.25f, blue, alpha);
	glCallList(flarelist[0]);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(0.5f, 0.2f, 1.0f);
	glColor4f(red, green * 0.4f, blue * 0.4f, alpha * 0.4f);
	glCallList(flarelist[2]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.15f, y + dy * 0.15f, 0.0f);
	glScalef(0.04f, 0.04f, 1.0f);
	glColor4f(red * 0.9f, green * 0.9f, blue, alpha * 0.9f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.25f, y + dy * 0.25f, 0.0f);
	glScalef(0.06f, 0.06f, 1.0f);
	glColor4f(red * 0.8f, green * 0.8f, blue, alpha * 0.9f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.35f, y + dy * 0.35f, 0.0f);
	glScalef(0.08f, 0.08f, 1.0f);
	glColor4f(red * 0.7f, green * 0.7f, blue, alpha * 0.9f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 1.25f, y + dy * 1.25f, 0.0f);
	glScalef(0.05f, 0.05f, 1.0f);
	glColor4f(red, green * 0.6f, blue * 0.6f, alpha * 0.9f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 1.65f, y + dy * 1.65f, 0.0f);
	glRotatef(x, 0, 0, 1);
	glScalef(0.3f, 0.3f, 1.0f);
	glColor4f(red, green, blue, alpha);
	glCallList(flarelist[3]);

	glLoadIdentity();
	glTranslatef(x + dx * 1.85f, y + dy * 1.85f, 0.0f);
	glScalef(0.04f, 0.04f, 1.0f);
	glColor4f(red, green * 0.6f, blue * 0.6f, alpha * 0.9f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 2.2f, y + dy * 2.2f, 0.0f);
	glScalef(0.3f, 0.3f, 1.0f);
	glColor4f(red, green, blue, alpha * 0.7f);
	glCallList(flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 2.5f, y + dy * 2.5f, 0.0f);
	glScalef(0.6f, 0.6f, 1.0f);
	glColor4f(red, green, blue, alpha * 0.8f);
	glCallList(flarelist[3]);

	glPopMatrix();

	// Unsetup projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}