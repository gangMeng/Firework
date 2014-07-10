#include"stdafx.h"
#include "particle.h"


#include<math.h>

#ifndef _PARTICLESYSTEM_
#include"ParticleSystem.h"
#endif

#ifndef _SCENEMANAGER_
#include"SceneManager.h"
#endif

SceneManager * Particle::m_Scene = NULL;

extern int m_K;
Particle::Particle()
{
	type = STAR;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;  // terminal velocity of 20 ft/s
	t = 2.0f;
	tr = t;
	bright = 1.0f;
	life = bright;
	size = 30.0f;
	makeSmoke = 0;

	controlled=false;	
	smokeTimeIndex = 0;
	smokeTrailLength = 0.0f;
	sparkTrailLength = 0.0f;
	depth = 0.0f;
	m_Fire = NULL;
}



void Particle::randomColor(rsVec& color)
{
	int i, j, k;
	//rsVec color;

	switch(rsRandi(6))
	{
	case 0:
		i=0; j=1, k=2;
		break;
	case 1:
		i=0; j=2, k=1;
		break;
	case 2:
		i=1; j=0, k=2;
		break;
	case 3:
		i=1; j=2, k=0;
		break;
	case 4:
		i=2; j=0, k=1;
		break;
	case 5:
		i=2; j=1, k=0;
	}

	color[i] = 1.0f;
	color[j] = rsRandf(1.0f);
	color[k] = rsRandf(0.2f);
}

void Particle::initRocket()
{
	// Thrust, initial velocity, and t (time) should send
	// rockets to about 800 to 1200 feet before exploding.

	if(m_Scene->IsRecord())//正在录制
	{

	 m_Scene->m_PlayTime.push_back(m_Scene->m_Frame*m_Scene->m_f);//记录时间
	 m_Scene->m_PlayStyle.push_back(WHISTLESOUND);//记录类型
	 m_Scene->m_PlayPos.push_back(m_Scene->m_Camera.cameraPos);
	  m_Scene->m_PlaySource.push_back(xyz);
     
	 soundengine->insertSoundNode(WHISTLESOUND, xyz, m_Scene->m_Camera.cameraPos);
	 
	 if(rsRandi(2))
	 {
	  m_Scene->m_PlayTime.push_back(m_Scene->m_Frame*m_Scene->m_f);//记录时间
	  m_Scene->m_PlayStyle.push_back(LAUNCH1SOUND);//记录类型
	  m_Scene->m_PlayPos.push_back(m_Scene->m_Camera.cameraPos);
	  m_Scene->m_PlaySource.push_back(xyz);
	  soundengine->insertSoundNode(LAUNCH1SOUND, xyz, m_Scene->m_Camera.cameraPos);
	 }
	 else
	 {
	  m_Scene->m_PlayTime.push_back(m_Scene->m_Frame*m_Scene->m_f);//记录时间
	  m_Scene->m_PlayStyle.push_back(LAUNCH2SOUND);//记录类型
	  m_Scene->m_PlayPos.push_back(m_Scene->m_Camera.cameraPos);
	  m_Scene->m_PlaySource.push_back(xyz);
	  soundengine->insertSoundNode(LAUNCH2SOUND, xyz, m_Scene->m_Camera.cameraPos);
	 }

	}
	else
	{
	 soundengine->insertSoundNode(WHISTLESOUND, xyz, m_Scene->m_Camera.cameraPos);
	 if(rsRandi(2))
		soundengine->insertSoundNode(LAUNCH1SOUND, xyz, m_Scene->m_Camera.cameraPos);
		else
			soundengine->insertSoundNode(LAUNCH2SOUND, xyz, m_Scene->m_Camera.cameraPos);
	}
	type = ROCKET;
	
	//初始化发射位置
	
	lastxyz[0] = xyz[0];
	lastxyz[1] = 4.0f;
	lastxyz[2] = xyz[2];
	//初始化速度
	vel.set(0.0f, 100.0f, 0.0f);
	//颜色
	rgb.set(rsRandf(0.7f) + 0.3f, rsRandf(0.7f) + 0.3f, 0.3f);
	size = 1.0f;
	drag = 0.281f;  // terminal velocity of 50 ft/s
	t = tr = rsRandf(2.0f) + 5.0f;
	life = 1.0f;
	bright = 0.0f;
	thrust = 185.0f;
	endthrust = rsRandf(0.1f) + 0.3f;
	spin = rsRandf(40.0f) - 20.0f;
	tilt = rsRandf(30.0f * float(fabs(spin)));
	tiltvec.set(cosf(spin), 0.0f, sinf(spin));
	if(!rsRandi(200))
	{  // crash the occasional rocket
	//偶尔发射失败
		spin = 0.0f;
		tilt = rsRandf(100.0f) + 75.0f;
		float temp = rsRandf(PIx2);
		tiltvec.set(cosf(temp), 0.0f, sinf(temp));
	}

	makeSmoke = 1;
	smokeTrailLength = 0.0f;
	sparkTrailLength = 0.0f;
	explosiontype = 0;

	

}

void Particle::initFountain()
{
	type = FOUNTAIN;
	//displayList = m_Scene->m_Flare.flarelist[0];
	size = 30.0f;
	// position can be defined here because these are always on the ground
	xyz[0] = rsRandf(300.0f) - 150.0f;
	xyz[1] = 5.0f;
	xyz[2] = rsRandf(300.0f) - 150.0f;
	randomColor(rgb);
	t = tr = rsRandf(5.0f) + 10.0f;
	bright = 0.0f;
	life = 1.0f;
	makeSmoke = 0;

	
}

void Particle::initSpinner()
{
	type = SPINNER;
	//displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;  // terminal velocity of 20 ft/s
	randomColor(rgb);
	spin = rsRandf(3.0f) + 12.0f;  // radial velocity
	tilt = rsRandf(PIx2);  // original rotation
	tiltvec.set(rsRandf(2.0f) - 1.0f, rsRandf(2.0f) - 1.0f, rsRandf(2.0f) - 1.0f);
	tiltvec.normalize();  // vector around which this spinner spins
	t = tr = rsRandf(2.0f) + 6.0f;
	bright = 0.0f;
	life = 1.0f;
	size = 20.0f;
	makeSmoke = 1;
	sparkTrailLength = 0.0f;
}

void Particle::initSmoke(rsVec pos, rsVec speed,int dAmbient)
{
	type = SMOKE;
	displayList = m_Scene->m_Smoke.smokelist[rsRandi(5)];
	xyz = pos;
	vel = speed;
	rgb[0] = rgb[1] = rgb[2] = 0.01f * float(dAmbient);
	drag = 2.0f;
	// time for each smoke particle varies and must be assigned by the particle that produces the smoke
	size = 0.1f;
	makeSmoke = 0;
}

void Particle::initStar()
{
	type = STAR;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;  // terminal velocity of 20 ft/s
	size = 30.0f;
	t = tr = rsRandf(1.0f) + 2.0f;
	life = 1.0f;
	static int someSmoke = 0;
	makeSmoke = m_Scene->m_Smoke.whichSmoke[someSmoke];
	smokeTrailLength = 0.0f;
	++someSmoke;
	if(someSmoke >= WHICHSMOKES)
		someSmoke = 0;
}

void Particle::initStreamer()
{
	type = STREAMER;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;  // terminal velocity of 20 ft/s
	size = 30.0f;
	t = tr = rsRandf(1.0f) + 3.0f;
	life = 1.0f;
	sparkTrailLength = 0.0f;
}

void Particle::initMeteor()
{
	type = METEOR;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;  // terminal velocity of 20 ft/s
	t = tr = rsRandf(1.0f) + 0.5f;
	life = 1.0f;
	size = 20.0f;
	sparkTrailLength = 0.0f;
}

void Particle::initStarPopper()
{
	type = POPPER;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.4f;
	t = tr = rsRandf(1.5f) + 3.0f;
	life = 1.0f;
	makeSmoke = 1;
	explosiontype = STAR;
	size = 0.0f;
	smokeTrailLength = 0.0f;
}

void Particle::initStreamerPopper()
{
	type = POPPER;
	displayList = m_Scene->m_Flare.flarelist[0];
	size = 0.0f;
	drag = 0.4f;
	t = tr = rsRandf(1.5f) + 3.0f;
	life = 1.0f;
	makeSmoke = 1;
	explosiontype = STREAMER;
	smokeTrailLength = 0.0f;
}

void Particle::initMeteorPopper()
{
	type = POPPER;
	displayList = m_Scene->m_Flare.flarelist[0];
	size = 14.0f;
	drag = 0.4f;
	t = tr = rsRandf(1.5f) + 3.0f;
	life = 1.0f;
	makeSmoke = 0;
	explosiontype = METEOR;
	smokeTrailLength = 0.0f;
}

void Particle::initLittlePopper()
{
	type = POPPER;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.4f;
	t = tr = 4.0f * (0.5f - sinf(rsRandf(PI))) + 4.5f;
	life = 1.0f;
	size = rsRandf(3.0f) + 7.0f;
	makeSmoke = 0;
	explosiontype = POPPER;
}

void Particle::initBee()
{
	type = BEE;
	displayList = m_Scene->m_Flare.flarelist[0];
	size = 10.0f;
	drag = 0.5f;
	t = tr = rsRandf(2.5f) + 2.5f;
	life = 1.0f;
	makeSmoke = 0;
	sparkTrailLength = 0.0f;

	// these variables will be misused to describe bee acceleration vector
	thrust = rsRandf(PIx2) + PI;
	endthrust = rsRandf(PIx2) + PI;
	spin = rsRandf(PIx2) + PI;
	tiltvec.set(rsRandf(PIx2), rsRandf(PIx2), rsRandf(PIx2));
}

void Particle::initSucker()
{
	int i;
	Particle* newp;
	rsVec color;
	float temp1, temp2, ch, sh, cp, sp;

	type = SUCKER;
	drag = 0.612f;  // terminal velocity of 20 ft/s
	displayList = m_Scene->m_Flare.flarelist[2];
	rgb.set(1.0f, 1.0f, 1.0f);
	size = 300.0f;
	t = tr = 4.0f;
	life = 1.0f;
	makeSmoke = 0;

	// make explosion
	newp = m_ParticleSystem->AddParticle();
	newp->type = EXPLOSION;
	newp->xyz = xyz;
	newp->vel = vel;
	newp->rgb.set(1.0f, 1.0f, 1.0f);
	newp->size = 200.0f;
	newp->t = newp->tr = 4.0f;

	// Make double ring to go along with sucker
	randomColor(color);
	temp1 = rsRandf(PI);  // heading
	temp2 = rsRandf(PI);  // pitch
	ch = cosf(temp1);
	sh = sinf(temp1);
	cp = cosf(temp2);
	sp = sinf(temp2);
	for(i=0; i<90; i++)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		// pitch
		newp->vel[1] = sp * newp->vel[2];
		newp->vel[2] = cp * newp->vel[2];
		// heading
		temp1 = newp->vel[0];
		newp->vel[0] = ch * temp1 + sh * newp->vel[1];
		newp->vel[1] = -sh * temp1 + ch * newp->vel[1];
		// multiply velocity
		newp->vel[0] *= 350.0f + rsRandf(30.0f);
		newp->vel[1] *= 350.0f + rsRandf(30.0f);
		newp->vel[2] *= 350.0f + rsRandf(30.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}
	randomColor(color);
	temp1 = rsRandf(PI);  // heading
	temp2 = rsRandf(PI);  // pitch
	ch = cosf(temp1);
	sh = sinf(temp1);
	cp = cosf(temp2);
	sp = sinf(temp2);
	for(i=0; i<90; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		// pitch
		newp->vel[1] = sp * newp->vel[2];
		newp->vel[2] = cp * newp->vel[2];
		// heading
		temp1 = newp->vel[0];
		newp->vel[0] = ch * temp1 + sh * newp->vel[1];
		newp->vel[1] = -sh * temp1 + ch * newp->vel[1];
		// multiply velocity
		newp->vel[0] *= 600.0f + rsRandf(50.0f);
		newp->vel[1] *= 600.0f + rsRandf(50.0f);
		newp->vel[2] *= 600.0f + rsRandf(50.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}

	
}

void Particle::initShockwave()
{
	int i;
	Particle* newp;
	rsVec color;

	type = SHOCKWAVE;
	drag = 0.612f;  // terminal velocity of 20 ft/s
	rgb.set(1.0f, 1.0f, 1.0f);
	size = 0.01f;
	t = tr = 8.0f;
	life = 1.0f;
	bright = life;

	// make explosion
	newp = m_ParticleSystem->AddParticle();
	newp->type = EXPLOSION;
	newp->xyz = xyz;
	newp->vel = vel;
	newp->rgb.set(1.0f, 1.0f, 1.0f);
	newp->size = 300.0f;
	newp->t = newp->tr = 2.0f;
	life = 1.0f;
	makeSmoke = 0;

	// Little sphere without smoke
	randomColor(color);
	for(i=0; i<75; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel *= (rsRandf(10.0f) + 100.0f);
		newp->vel += vel;
		newp->rgb = color;
		newp->size = 100.0f;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}

	// Disk of stars without smoke
	randomColor(color);
	for(i=0; i<150; i++)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->drag = 0.2f;
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(0.03f) - 0.005f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		// multiply velocity
		newp->vel *= (rsRandf(30.0f) + 500.0f);
		newp->vel += vel;
		newp->rgb = color;
		newp->size = 50.0f;
		newp->t = newp->tr = rsRandf(2.0f) + 3.0f;
		newp->makeSmoke = 0;
	}

	
}

void Particle::initStretcher()
{
	int i;
	Particle* newp;
	rsVec color;

	type = STRETCHER;
	drag = 0.612f;  // terminal velocity of 20 ft/s
	displayList = m_Scene->m_Flare.flarelist[3];
	rgb.set(1.0f, 1.0f, 1.0f);
	size = 0.0f;
	t = tr = 4.0f;
	life = 1.0f;
	makeSmoke = 0;

	// explosion
	newp = m_ParticleSystem->AddParticle();
	newp->type = EXPLOSION;
	newp->displayList = m_Scene->m_Flare.flarelist[0];
	newp->xyz = xyz;
	newp->vel = vel;
	newp->rgb.set(1.0f, 0.8f, 0.6f);
	newp->size = 400.0f;
	newp->t = newp->tr = 4.0f;
	life = 1.0f;
	newp->makeSmoke = 0;

	// Make triple ring to go along with stretcher
	randomColor(color);
	for(i=0; i<80; i++)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel[0] *= 400.0f + rsRandf(30.0f);
		newp->vel[1] += rsRandf(70.0f) - 35.0f;
		newp->vel[2] *= 400.0f + rsRandf(30.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}
	randomColor(color);
	for(i=0; i<80; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel[0] *= 550.0f + rsRandf(40.0f);
		newp->vel[1] += rsRandf(70.0f) - 35.0f;
		newp->vel[2] *= 550.0f + rsRandf(40.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}
	randomColor(color);
	for(i=0; i<80; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel[0] *= 700.0f + rsRandf(50.0f);
		newp->vel[1] += rsRandf(70.0f) - 35.0f;
		newp->vel[2] *= 700.0f + rsRandf(50.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}

	
}

void Particle::initBigmama()
{
	int i;
	Particle* newp;
	rsVec color;
	float temp;

	type = BIGMAMA;
	drag = 0.612f;  // terminal velocity of 20 ft/s
	displayList = m_Scene->m_Flare.flarelist[2];
	rgb.set(0.6f, 0.6f, 1.0f);
	size = 0.0f;
	t = tr = 5.0f;
	life = 1.0f;
	bright = life;
	makeSmoke = 0;

	// explosion
	newp = m_ParticleSystem->AddParticle();
	newp->type = EXPLOSION;
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->rgb.set(0.8f, 0.8f, 1.0f);
	newp->size = 200.0f;
	newp->t = newp->tr = 2.5f;
	life = 1.0f;
	newp->makeSmoke = 0;

	// vertical stars
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] += 15.0f;
	newp->rgb.set(1.0f, 1.0f, 0.9f);
	newp->size = 400.0f;
	newp->t = newp->tr = 3.0f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] -= 15.0f;
	newp->rgb.set(1.0f, 1.0f, 0.9f);
	newp->size = 400.0f;
	newp->t = newp->tr = 3.0f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] += 45.0f;
	newp->rgb.set(1.0f, 1.0f, 0.6f);
	newp->size = 400.0f;
	newp->t = newp->tr = 3.5f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] -= 45.0f;
	newp->rgb.set(1.0f, 1.0f, 0.6f);
	newp->size = 400.0f;
	newp->t = newp->tr = 3.5f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] += 75.0f;
	newp->rgb.set(1.0f, 0.5f, 0.3f);
	newp->size = 400.0f;
	newp->t = newp->tr = 4.0f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] -= 75.0f;
	newp->rgb.set(1.0f, 0.5f, 0.3f);
	newp->size = 400.0f;
	newp->t = newp->tr = 4.0f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] += 105.0f;
	newp->rgb.set(1.0f, 0.0f, 0.0f);
	newp->size = 400.0f;
	newp->t = newp->tr = 4.5f;
	newp->makeSmoke = 0;
	newp = m_ParticleSystem->AddParticle();
	newp->initStar();
	newp->xyz = xyz;
	newp->vel = vel;
	newp->drag = 0.0f;
	newp->vel[1] -= 105.0f;
	newp->rgb.set(1.0f, 0.0f, 0.0f);
	newp->size = 400.0f;
	newp->t = newp->tr = 4.5f;
	newp->makeSmoke = 0;

	// Sphere without smoke
	randomColor(color);
	for(i=0; i<75; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		temp = 600.0f + rsRandf(100.0f);
		newp->vel[0] *= temp;
		newp->vel[1] *= temp;
		newp->vel[2] *= temp;
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->t = newp->tr = rsRandf(2.0f) + 2.0f;
		newp->makeSmoke = 0;
	}

	// disk of big streamers
	randomColor(color);
	for(i=0; i<50; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStreamer();
		newp->drag = 0.3f;
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel[0] *= 1000.0f + rsRandf(100.0f);
		newp->vel[1] += rsRandf(100.0f) - 50.0f;
		newp->vel[2] *= 1000.0f + rsRandf(100.0f);
		newp->vel[0] += vel[0];
		newp->vel[1] += vel[1];
		newp->vel[2] += vel[2];
		newp->rgb = color;
		newp->size = 100.0f;
		newp->t = newp->tr = rsRandf(6.0f) + 3.0f;
		newp->makeSmoke = 0;
	}
	
}
void Particle::initExplosion()
{
	type = EXPLOSION;
	displayList = m_Scene->m_Flare.flarelist[0];
	drag = 0.612f;
	t = tr = 0.5f;
	bright = 1.0f;
	life = 1.0f;
	size = 100.0f;
	makeSmoke = 0;

	// Don't do massive explosions too close to the ground
	if((explosiontype == 19 || explosiontype == 20) && (xyz[1] < 600.0f))
		explosiontype = 0;


	/*if(m_Scene->IsRecord())
	{
	  m_Scene->m_PlayTime.push_back(m_Scene->m_Frame*m_Scene->m_f);//记录时间
	  m_Scene->m_PlayStyle.push_back(BOOM3SOUND);//记录类型
	  m_Scene->m_PlayPos.push_back(m_Scene->m_Camera.cameraPos);
	  m_Scene->m_PlaySource.push_back(xyz);
	  soundengine->insertSoundNode(BOOM3SOUND, xyz, m_Scene->m_Camera.cameraPos);
      
	
	}
	else
	{
		soundengine->insertSoundNode(BOOM3SOUND, xyz, m_Scene->m_Camera.cameraPos);
	
	
	}*/
	rsVec rgb2;
	switch(explosiontype)
	{
	case 0:
		randomColor(rgb);
		rgb[0] = rgb[0]/4;
		rgb[1] = rgb[1]/4;
		rgb[2] = rgb[2]/4;
		if(!rsRandi(10))  // big sphere
			popSphere(50, 200.0f, rgb);
		else  // regular sphere
			popSphere(50, rsRandf(200.0f), rgb);
		break;
	case 1:
		randomColor(rgb);
		if(!rsRandi(10))  // big split sphere
			popSplitSphere(225, 1000.0f, rgb);
		else  // regular split sphere
			popSplitSphere(175, rsRandf(100.0f) + 400.0f, rgb);
		break;
	case 2:
		rgb.set(1.0f, 1.0f, 1.0f);
		if(!rsRandi(10))  // big multicolored sphere
			popMultiColorSphere(225, 1000.0f);
		else  // regular multicolored sphere
			popMultiColorSphere(175, rsRandf(100.0f) + 400.0f);
		break;
	case 3:  // ring
		randomColor(rgb);
		popRing(80, rsRandf(100.0f) + 400.0f, rgb);
		break;
	case 4:  // double sphere
		randomColor(rgb);
		randomColor(rgb2);
		popSphere(90, rsRandf(50.0f) + 200.0f, rgb2);
		popSphere(150, rsRandf(100.0f) + 500.0f, rgb);
		break;
	case 5:  // sphere and ring
		randomColor(rgb);
		randomColor(rgb2);
		popRing(80, rsRandf(100.0f) + 500.0f, rgb2);
		popSphere(150, rsRandf(50.0f) + 200.0f, rgb);
		break;
	case 6:  // Sphere of streamers
		randomColor(rgb);
		popStreamers(40, rsRandf(100.0f) + 400.0f, rgb);
		break;
	case 7:  // Sphere of meteors
		randomColor(rgb);
		popMeteors(40, rsRandf(100.0f) + 400.0f, rgb);
		break;
	case 8:  // Small sphere of stars and large sphere of streamers
		randomColor(rgb);
		randomColor(rgb2);
		popStreamers(30, rsRandf(100.0f) + 500.0f, rgb);
		popSphere(90, rsRandf(50.0f) + 200.0f, rgb2);
		break;
	case 9:  // Small sphere of stars and large sphere of meteors
		randomColor(rgb);
		randomColor(rgb2);
		popMeteors(30, rsRandf(100.0f) + 500.0f, rgb);
		popSphere(90, rsRandf(50.0f) + 200.0f, rgb2);
		break;
	case 10:  // Sphere of streamers inside sphere of stars
		randomColor(rgb);
		randomColor(rgb2);
		popStreamers(30, rsRandf(100.0f) + 450.0f, rgb);
		popSphere(150, rsRandf(50.0f) + 500.0f, rgb2);
		break;
	case 11:  // Sphere of meteors inside sphere of stars
		randomColor(rgb);
		randomColor(rgb2);
		popMeteors(30, rsRandf(100.0f) + 450.0f, rgb);
		popSphere(150, rsRandf(50.0f) + 500.0f, rgb2);
		break;
	case 12:  // a few bombs that fall and explode into stars
		randomColor(rgb);
		popStarPoppers(8, rsRandf(100.0f) + 300.0f, rgb);
		break;
	case 13:  // a few bombs that fall and explode into streamers
		randomColor(rgb);
		popStreamerPoppers(8, rsRandf(100.0f) + 300.0f, rgb);
		break;
	case 14:  // a few bombs that fall and explode into meteors
		randomColor(rgb);
		popMeteorPoppers(8, rsRandf(100.0f) + 300.0f, rgb);
		break;
	case 15:  // lots of little falling firecrackers
		popLittlePoppers(250, rsRandf(50.0f) + 150.0f);
		break;
	case 16:
		randomColor(rgb);
		popBees(50, 10.0f, rgb);
		break;
	case 17:  // Boom!  (loud noise and flash of light)
		rgb.set(1.0f, 1.0f, 1.0f);
		size = 150.0f;
		break;
	// 18 is a spinner, which doesn't require explosion
	case 19:
		rgb.set(1.0f, 1.0f, 1.0f);
		initSucker();
		break;
	case 20:
		rgb.set(1.0f, 1.0f, 1.0f);
		initStretcher();
		break;

	case 21:



		randomColor(rgb);
		popShape(m_Fire->m_Vertices.size(),50.0f,rgb,m_Fire);
        break;
	case 22:
        randomColor(rgb);
		m_Fire->rgb.X() = rgb[0];
		m_Fire->rgb.Y() = rgb[1];
		m_Fire->rgb.Z() = rgb[2];

        popIgnitor(m_Fire,rgb);
		break;
	case 100:  // these three are little explosions for poppers
		popSphere(30, 100.0f, rgb);
		break;
	case 101:
		popStreamers(10, 100.0f, rgb);
		break;
	case 102:
		popMeteors(10, 100.0f, rgb);
	}	

	//if(explosiontype == 17)  // extra resounding boom
		//soundengine->insertSoundNode(BOOM4SOUND, xyz, m_Scene->m_Camera.cameraPos);
		// make bees and big booms whistle sometimes
	//	if(explosiontype == 16 || explosiontype == 17)
		//	if(rsRandi(2))
			//	
		// regular booms
		//if(explosiontype <= 16 || explosiontype >= 100)
	
}
void Particle::popSphere(int numParts, float v0, rsVec color)
{
	Particle* newp;

	for(int i=0; i<numParts; ++i)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		t = tr = rsRandf(0.005f);
		life =1.0;
		size = 0.5f;
		newp->xyz[0] = xyz[0];
		newp->xyz[1] = xyz[1]-200;
		newp->xyz[2] = xyz[2];


		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel *= v0 + rsRandf(50.0f);
		newp->vel += vel;
		newp->rgb = color;
	}

	if(!rsRandi(100))
		newp->t = newp->tr = rsRandf(20.0f) + 5.0f;
}
void Particle::popShape(int numParts,float v0,rsVec color,ConstrainedFire * fire)
{
   Particle* newp;

   Point3f explodePosition;

   /*if(m_Scene->IsRecord())
	{
      m_Scene->m_PlayTime.push_back(m_Scene->m_Frame*m_Scene->m_f);//记录时间
	  m_Scene->m_PlayStyle.push_back(BOOM3SOUND );//记录类型
	  m_Scene->m_PlayPos.push_back(m_Scene->m_Camera.cameraPos);
	  m_Scene->m_PlaySource.push_back(xyz);
	  soundengine->insertSoundNode(BOOM3SOUND, xyz, m_Scene->m_Camera.cameraPos);
	}
	else
	{
    soundengine->insertSoundNode(BOOM3SOUND, xyz, m_Scene->m_Camera.cameraPos);
   }*/
   explodePosition.X() = xyz[0];
   explodePosition.Y() = xyz[1];
   explodePosition.Z() = xyz[2];

   //fire->SetPosition(explodePosition);//设定爆炸位置

	for(int i=0; i<numParts; ++i)
	{
	  if(fire->GenerateParticle(newp))
	  {
	   //newp->makeSmoke = 1;
	  
	   newp->smokeTrailLength = 0.0f;
	   
	   newp->displayList = m_Scene->m_Flare.flarelist[0];
	   newp->drag = 0.612f;  // terminal velocity of 20 ft/s
	   newp->size = 28.0f;
	   newp->rgb = color;
	  }
	}

	//if(!rsRandi(100))
   // newp->t = newp->tr = rsRandf(6.0f) + 1.0f;


  //播放声音
 


}
void Particle::popSplitSphere(int numParts, float v0, rsVec color1)
{
	Particle* newp;
	rsVec color2;
	rsVec planeNormal;

	randomColor(color2);
	planeNormal[0] = rsRandf(1.0f) - 0.5f;
	planeNormal[1] = rsRandf(1.0f) - 0.5f;
	planeNormal[2] = rsRandf(1.0f) - 0.5f;
	planeNormal.normalize();
	for(int i=0; i<numParts; i++){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		if(planeNormal.dot(newp->vel) > 0.0f)
			newp->rgb = color1;
		else
			newp->rgb = color2;
		newp->vel *= v0 + rsRandf(50.0f);
		newp->vel += vel;
	}

	if(!rsRandi(100))
		newp->t = newp->tr = rsRandf(20.0f) + 5.0f;
}

void Particle::popMultiColorSphere(int numParts, float v0)
{
	Particle* newp;
	rsVec color[3];

	randomColor(color[0]);
	randomColor(color[1]);
	randomColor(color[2]);
	int j(0);
	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel *= v0 + rsRandf(30.0f);
		newp->vel += vel;
		newp->rgb = color[j];
		++j;
		if(j >= 3)
			j = 0;
	}

	if(!rsRandi(100))
		newp->t = newp->tr = rsRandf(20.0f) + 5.0f;
}

void Particle::popRing(int numParts, float v0, rsVec color)
{
	Particle* newp;

	float temph = rsRandf(PI);  // heading
	float tempp = rsRandf(PI);  // pitch
	const float ch(cosf(temph));
	const float sh(sinf(temph));
	const float cp(cosf(tempp));
	const float sp(sinf(tempp));
	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initStar();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = 0.0f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		// pitch
		newp->vel[1] = sp * newp->vel[2];
		newp->vel[2] = cp * newp->vel[2];
		// heading
		const float temp(newp->vel[0]);
		newp->vel[0] = ch * temp + sh * newp->vel[1];
		newp->vel[1] = -sh * temp + ch * newp->vel[1];
		// multiply velocity
		newp->vel[0] *= v0 + rsRandf(50.0f);
		newp->vel[1] *= v0 + rsRandf(50.0f);
		newp->vel[2] *= v0 + rsRandf(50.0f);
		newp->vel += vel;
		newp->rgb = color;
	}

	if(!rsRandi(100))
		newp->t = newp->tr = rsRandf(20.0f) + 5.0f;
}

void Particle::popStreamers(int numParts, float v0, rsVec color)
{
	Particle* newp;

	for(int i=0; i<numParts; ++i)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initStreamer();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel *= v0 + rsRandf(50.0f);
		newp->vel += vel;
		newp->rgb = color;
	}
}

void Particle::popMeteors(int numParts, float v0, rsVec color)
{
	Particle* newp;

	for(int i=0; i<numParts; ++i)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initMeteor();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel.normalize();
		newp->vel *= v0 + rsRandf(50.0f);
		newp->vel += vel;
		newp->rgb = color;
	}
}

void Particle::popStarPoppers(int numParts, float v0, rsVec color)
{
	Particle* newp;
	const float v0x2(v0 * 2.0f);

	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initStarPopper();
		newp->xyz = xyz;
		newp->vel[0] = vel[0] + rsRandf(v0x2) - v0;
		newp->vel[1] = vel[1] + rsRandf(v0x2) - v0;
		newp->vel[2] = vel[2] + rsRandf(v0x2) - v0;
		newp->rgb = color;
	}
}

void Particle::popStreamerPoppers(int numParts, float v0, rsVec color)
{
	Particle* newp;
	const float v0x2(v0 * 2.0f);

	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initStreamerPopper();
		newp->xyz = xyz;
		newp->vel[0] = vel[0] + rsRandf(v0x2) - v0;
		newp->vel[1] = vel[1] + rsRandf(v0x2) - v0;
		newp->vel[2] = vel[2] + rsRandf(v0x2) - v0;
		newp->rgb = color;
	}
}

void Particle::popMeteorPoppers(int numParts, float v0, rsVec color)
{
	Particle* newp;
	const float v0x2(v0 * 2.0f);

	for(int i=0; i<numParts; ++i)
	{
		newp = m_ParticleSystem->AddParticle();
		newp->initMeteorPopper();
		newp->xyz = xyz;
		newp->vel[0] = vel[0] + rsRandf(v0x2) - v0;
		newp->vel[1] = vel[1] + rsRandf(v0x2) - v0;
		newp->vel[2] = vel[2] + rsRandf(v0x2) - v0;
		newp->rgb = color;
	}
}
void Particle::popIgnitor(ConstrainedFire * fire,rsVec color)
{
  Particle* particle;
  

   Point3f explodePosition;//点燃位置



   explodePosition.X() = xyz[0];
   explodePosition.Y() = xyz[1];
   explodePosition.Z() = xyz[2];

   fire->SetPosition(explodePosition);//设定爆炸位置

	for(int i=0; i<m_K; ++i)
	{
	  particle = m_ParticleSystem->AddParticle();
	  particle->initMeteorPopper();
	  particle->xyz = xyz;

	  particle->rgb[0] = color[0]*10;
	  particle->rgb[1] = color[1]*10;
	  particle->rgb[2] = color[2]*10;
	  
	  
	  particle->m_Fire = fire;
	  particle->type = IGNITOR;
	  particle->t = particle->tr = 2.5f;
	  particle->life = 1.0f;

	
	particle->controlled = true;
	
	//粒子生命值
	
	
	//爆炸物理属性
	particle->force = Point3f(0,-200,0);//作用力(重力)
    particle->k = fire->expEmitter[0].k;//空气阻力

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

	 sx = fire->expEmitter[i].explodePos.X()-particle->xyz[0];
	 sy = fire->expEmitter[i].explodePos.Y()-particle->xyz[1]+350;
	 sz = fire->expEmitter[i].explodePos.Z()-particle->xyz[2];

	 particle->C.X()=sx*ktp1/w;
	 
     particle->C.Y()=(sy- particle->force.Y()*(2*T-T0)*T/(2*(ktp1-2.0)))  * (ktp1/w);

	 particle->C.Z()=sz*ktp1/w;
			
	 //为新添加的形状受控粒子计算速度的初始值
	 particle->vel[0] = particle->C.X();
	 particle->vel[1] = particle->force.Y()*T/(particle->k*T-1.0)+particle->C.Y()*pow(T,particle->k*T);
	 particle->vel[2] = particle->C.Z();
		
	}
}
void Particle::popLittlePoppers(int numParts, float v0)
{
	Particle* newp;
	const float v0x2(v0 * 2.0f);

	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initLittlePopper();
		newp->xyz = xyz;
		newp->vel[0] = vel[0] + rsRandf(v0x2) - v0;
		newp->vel[1] = vel[1] + rsRandf(v0x2) - v0;
		newp->vel[2] = vel[2] + rsRandf(v0x2) - v0;
	}


}

void Particle::popBees(int numParts, float v0, rsVec color)
{
	Particle* newp;

	for(int i=0; i<numParts; ++i){
		newp = m_ParticleSystem->AddParticle();
		newp->initBee();
		newp->xyz = xyz;
		newp->vel[0] = rsRandf(1.0f) - 0.5f;
		newp->vel[1] = rsRandf(1.0f) - 0.5f;
		newp->vel[2] = rsRandf(1.0f) - 0.5f;
		newp->vel *= v0;
		newp->vel += vel;
		newp->rgb = color;
	}
}

void Particle::findDepth(rsVec cameraPos,float * billboardMat)
{
	// This isn't the actual distance from the camera.  It is the the
	// distance along the view vector coming straight out of the camera.
	// This is calculated with a simple dot product.  The billboards don't
	// actually face the camera; they all face the same direction (straight
	// down the view vector of the camera, so sorting is done a little
	// differently than one might expect).
	depth = (cameraPos[0] - xyz[0]) * float(billboardMat[8])
			+ (cameraPos[1] - xyz[1]) * float(billboardMat[9])
			+ (cameraPos[2] - xyz[2]) * float(billboardMat[10]);
}


void Particle::update(float frameTime,int dSmoke,int dClouds,int dWind,int dIllumination,int dAmbient,World* theWorld)
{
	int i;
	float temp;
	rsVec dir, crossvec;
	rsQuat spinquat;
	rsMatrix spinmat;
	Particle *newp;
	rsVec rocketEjection;
    ///////////////////////////////   烟雾        //////////////////////////////

	if(type!=SMOKE&&type!=ROCKET&&type!=IGNITOR)
	{

      if(life<0.02)
	  {
      makeSmoke = 1;
	  
	  smokeTrailLength = 16.0f;
	  }
	}

	if(type==IGNITOR)
	{
     if(life<0.4)
	 {
      makeSmoke = 1;
	  
	  smokeTrailLength = 10.0f;


	 }
	}

	///////////////////////////////    速度   ///////////////////////////////////////////////
	if(type == ROCKET && life > endthrust)
	{
	   dir = vel;
	   dir.normalize();
	   crossvec.cross(dir, tiltvec);  // correct sidevec
	   tiltvec.cross(crossvec, dir);
	   tiltvec.normalize();
	   spinquat.make(spin * frameTime, dir[0], dir[1], dir[2]);  // twist tiltvec
	   spinmat.fromQuat(spinquat);
	   tiltvec.transVec(spinmat);
	   vel += dir * (thrust * frameTime);  // apply thrust
	   vel += tiltvec * (tilt * frameTime);  // apply tilt
	}

	if(type==SHAPE||type==IGNITOR)
	{	 
	  float kT;
	  kT=k*t;
	  //根据运动方城确定速度
	  vel[0] = (C.X()*pow(tr,kT));
	  vel[1] = (force.Y()*(tr)/(kT-1.0f)+C.Y()*pow(tr,kT));
	  vel[2] = (C.Z()*pow(tr,kT));
	}

	if(type == BEE)
	{
	  vel[0] += 500.0f * cosf(tiltvec[0]) * frameTime;
	  vel[1] += 500.0f *(cosf(tiltvec[1]) - 0.2f) * frameTime;
	  vel[2] += 500.0f * cosf(tiltvec[2]) * frameTime;
	}
	
	if(type != SMOKE&&type!=SHAPED&&type!=ROCKET) vel[1] -= frameTime * 200.0f;  //重力
	if(type==ROCKET)vel[1] -= frameTime * 6.0f;  //重力
	
	if(type==SHAPED)vel[1] -= frameTime * 220.0f;  //重力
	//空气阻力
	temp = 1.0f / (1.0f + drag * frameTime);
	vel *= temp * temp;

	
	//////////////////////////////////  位置 /////////////////////////////////////////
	// (Fountains don't move)
	if(type != FOUNTAIN)
	{
	  lastxyz = xyz;
	  xyz += vel * frameTime;
	  // Wind:  1/10 wind on ground; -1/2 wind at 500 feet; full wind at 2000 feet;
	  // This value is calculated to coincide with movement of the clouds in world.h
	  // Here's the polynomial wind equation that simulates windshear:
	  xyz[0] += (0.1f - 0.00175f * xyz[1] + 0.0000011f * xyz[1] * xyz[1]) * dWind * frameTime;
	}
   
	if(type ==SHAPED)
    {
     lastxyz = xyz;
	 xyz[0] += (0.1f - 0.00175f * xyz[1] + 0.0000011f * xyz[1] * xyz[1]) * dWind * frameTime;
    }

	///////////////////////////////  亮度，生命值 ///////////////////////////////////////
	tr -= frameTime;//时间减少
	switch(type)
	{
	 case SHAPED:
     life = tr / t;//剩余时间单位化
	 temp = (t - tr) / t;
	 temp = temp * temp;
	 bright = 1.0f - (temp * temp);
	 life = bright;
	 break;

	 case SHAPE:
	 
	 life = tr / t;//剩余时间单位化
	 if(life<0)
	 {
       //vel[0] = 0;
	   //vel[1] = 0;
	   //vel[2] = 0;
	   type = SHAPED;//形状保持	
	   tr =t =1;
	   life =1;
	 }
     temp = (t - tr) / t;
	 temp = temp * temp;
	 bright = 1.0f - (temp * temp);
	 life = bright;
	 break;


	 case ROCKET:	
	  life = tr / t;
	  if(life > endthrust)
	  {  // Light up rocket gradually after it is launched//增亮
		bright += 2.0f * frameTime;
		if(bright > 0.5f) bright = 0.5f;
	   }
	  else
	  {  // Darken rocket after it stops thrusting
		bright -= frameTime;
			
		if(bright < 0.0f) bright = 0.0f;
	  }
	
	  break;
	case SMOKE:
		life = tr / t;
		bright = life * 0.7f;
		size += (30.0f - size) * (1.2f * frameTime);
		break;
	case FOUNTAIN:
	case SPINNER:
		life = tr / t;
		bright = life * life;
		// dim newborn fountains and spinners
		temp = t - tr;
		if(temp < 0.5f)
			bright *= temp * 2.0f;
		break;
	case EXPLOSION:
		life = tr / t;
		bright = life * life;
		break;
	case STAR:
	case STREAMER:
	case METEOR:
		temp = (t - tr) / t;
		temp = temp * temp;
		bright = 1.0f - (temp * temp);
		life = bright;
		break;
	case POPPER:
		life = tr;
		break;
	case IGNITOR:
		life = tr/t;
		break;
	case BEE:
		temp = (t - tr) / t;
		temp = temp * temp;
		bright = 1.0f - (temp * temp);
		life = bright;
		// Update bee acceleration (tiltvec) using misused variables
		tiltvec[0] += thrust * frameTime;
		tiltvec[1] += endthrust * frameTime;
		tiltvec[2] += spin * frameTime;
		break;
	case SUCKER:
		life = tr / t;
		bright = life;
		size = 250.0f * life;
		break;
	case SHOCKWAVE:
		life = tr / t;
		bright = life;
		rgb[2] = life * 0.5f + 0.5f;  // get a little yellow
		size += 400.0f * frameTime;
		break;
	case STRETCHER:
		life = tr / t;
		bright = 1.0f - ((1.0f - life) * (1.0f - life));
		size = 400.0f * bright;
		break;
	case BIGMAMA:
		life = tr / t;
		bright = life * 2.0f - 1.0f;
		if(bright < 0.0f)
			bright = 0.0f;
		size += 1500.0f * frameTime;
	}

	//产生烟雾粒子
	static rsVec velvec;
	if(makeSmoke && dSmoke)
	{
	  rsVec diff = xyz - lastxyz;
	  // distance rocket traveled since last frame
	  temp = diff.length();
		smokeTrailLength += temp;
		// number of smoke puffs to release (1 every 2 feet)
		int puffs = int(smokeTrailLength * 0.5f);
		float multiplier = 2.0f / smokeTrailLength;
		smokeTrailLength -= float(puffs) * 2.0f;
		rsVec smkpos = lastxyz;
		if((type == ROCKET) && (life > endthrust))
		{  // eject the smoke forcefully
		  rocketEjection = vel;
		  rocketEjection.normalize();
		  rocketEjection *= -2.0f * thrust * (life - endthrust);
		  for(i=0; i<puffs; ++i)
		  {  // make puffs of smoke
			smkpos += diff * multiplier;
			newp = m_ParticleSystem->AddParticle();
			velvec[0] = rocketEjection[0] + rsRandf(20.0f) - 10.0f;
			velvec[1] = rocketEjection[1] + rsRandf(20.0f) - 10.0f;
			velvec[2] = rocketEjection[2] + rsRandf(20.0f) - 10.0f;
			newp->initSmoke(smkpos, velvec,dClouds);
			newp->t = newp->tr = m_Scene->m_Smoke.smokeTime[smokeTimeIndex];
			++smokeTimeIndex;
			if(smokeTimeIndex >= SMOKETIMES)
			smokeTimeIndex = 0;
		  }
		}
		else{  // just form smoke in place
			for(i=0; i<puffs; ++i)
			{
				smkpos += diff * multiplier;
				newp = m_ParticleSystem->AddParticle();
				velvec[0] = rsRandf(20.0f) - 10.0f;
				velvec[1] = rsRandf(20.0f) - 10.0f;
				velvec[2] = rsRandf(20.0f) - 10.0f;
				
				if(type==SHAPE||type==SHAPED)
				{
					newp->initSmoke(smkpos, velvec,dAmbient*0.0001);
					newp->t = newp->tr = m_Scene->m_Smoke.smokeTime[smokeTimeIndex]*0.4;
					newp->size =0.00003;
					newp->rgb[3]=0.2;
				}
				else
				{
					newp->initSmoke(smkpos, velvec,dAmbient);
				newp->t = newp->tr = m_Scene->m_Smoke.smokeTime[smokeTimeIndex];
				}
				++smokeTimeIndex;
				if(smokeTimeIndex >= SMOKETIMES)
					smokeTimeIndex = 0;
			}
		}
	}

	// Sparks thrusting from rockets
	if((type == ROCKET) && (life > endthrust))
	{
		rsVec diff = xyz - lastxyz;
		// distance rocket traveled since last frame
		temp = diff.length();
		sparkTrailLength += temp;
		// number of sparks to release
		int sparks = int(sparkTrailLength * 0.4f);
		sparkTrailLength -= float(sparks) * 2.5f;
		rocketEjection = vel;
		rocketEjection.normalize();
		rocketEjection *= -thrust * (life - endthrust);
		for(i=0; i<sparks; ++i)
		{  // 产生火花
			newp = m_ParticleSystem->AddParticle();
			newp->initStar();
			newp->xyz = xyz - (diff * rsRandf(1.0f));
			newp->vel[0] = rocketEjection[0] + rsRandf(60.0f) - 30.0f;
			newp->vel[1] = rocketEjection[1] + rsRandf(60.0f) - 30.0f;
			newp->vel[2] = rocketEjection[2] + rsRandf(60.0f) - 30.0f;
			newp->rgb = rgb;
			newp->t = rsRandf(0.2f) + 0.1f;
			newp->tr = newp->t;
			newp->size = 8.0f * life;
			newp->displayList = m_Scene->m_Flare.flarelist[3];
			newp->makeSmoke = 0;
		}
	}
	if(type == SHAPE||type ==SHAPED)
	{
      for(i=0; i<1; ++i)
	  {  // 产生火花
	   newp = m_ParticleSystem->AddParticle();
	   newp->initStar();
	   newp->xyz[0] = xyz[0] + rsRandf(0.5f);
	    newp->xyz[1] = xyz[1] + rsRandf(0.5f);
		 newp->xyz[2] = xyz[2] + rsRandf(0.5f);

	   newp->vel[0] = 0;
	   newp->vel[1] = 0;
	   newp->vel[2] = 0;
		newp->rgb = rgb;
		newp->t =newp->tr= rsRandf(0.5f) + 0.1f;
		 
		newp->life =1;
		newp->size = 14.0f * life;
		newp->displayList = m_Scene->m_Flare.flarelist[3];
		newp->makeSmoke = 0;
		}

	}

	// Stars shooting up from fountain
	if(type == FOUNTAIN)
	{
		// spew 10-20 particles per second at maximum brightness
		sparkTrailLength += frameTime * bright * (rsRandf(10.0f) + 10.0f);
		int sparks = int(sparkTrailLength);
		sparkTrailLength -= float(sparks);
		for(i=0; i<sparks; ++i)
		{
		  newp = m_ParticleSystem->AddParticle();
		  newp->initStar();
		  newp->drag = 0.342f;  // terminal velocity is 40 ft/s
		  newp->xyz = xyz;
		  newp->xyz[1] += rsRandf(frameTime * 100.0f);
		  if(newp->xyz[1] > 50.0f) newp->xyz[1] = 50.0f;
		  newp->vel.set(rsRandf(20.0f) - 10.0f,
				rsRandf(30.0f) + 100.0f,
				rsRandf(20.0f) - 10.0f);
			newp->size = 10.0f;
			newp->rgb = rgb;
			newp->makeSmoke = 0;
		}
	}

	// Stars shooting out from spinner
	if(type == SPINNER)
	{
		dir.set(1.0f, 0.0f, 0.0f);
		crossvec.cross(dir, tiltvec);
		crossvec.normalize();
		crossvec *= 400.0f;
		temp = spin * frameTime;  // radius of spin this frame
		// spew 90-100 particles per second at maximum brightness
		sparkTrailLength += frameTime * bright * (rsRandf(10.0f) + 90.0f);
		int sparks = int(sparkTrailLength);
		sparkTrailLength -= float(sparks);
		for(i=0; i<sparks; ++i){
			spinquat.make(tilt + rsRandf(temp), tiltvec[0], tiltvec[1], tiltvec[2]);
			spinquat.toMat(spinmat.m);
			newp = m_ParticleSystem->AddParticle();
			newp->initStar();
			newp->xyz = xyz;
			newp->vel.set(vel[0] - (spinmat[0]*crossvec[0] + spinmat[4]*crossvec[1] + spinmat[8]*crossvec[2]) + rsRandf(20.0f) - 10.0f,
				vel[1] - (spinmat[1]*crossvec[0] + spinmat[5]*crossvec[1] + spinmat[9]*crossvec[2]) + rsRandf(20.0f) - 10.0f,
				vel[2] - (spinmat[2]*crossvec[0] + spinmat[6]*crossvec[1] + spinmat[10]*crossvec[2]) + rsRandf(20.0f) - 10.0f);
			newp->size = 15.0f;
			newp->rgb = rgb;
			newp->makeSmoke = 0;
			newp->t = newp->tr = rsRandf(0.5f) + 1.5f;
		}
		tilt += temp;
	}

	// trail from streamers
	if(type == STREAMER)//极光尾
	{
		rsVec diff = xyz - lastxyz;
		// distance streamer traveled since last frame
		sparkTrailLength += diff.length();
		// number of sparks to release each frame
		int sparks = int(sparkTrailLength * 0.04f);
		sparkTrailLength -= float(sparks) * 25.0f;
		for(i=0; i<sparks; ++i){
			newp = m_ParticleSystem->AddParticle();
			newp->initStar();
			newp->xyz = xyz - (diff * rsRandf(1.0f));
			newp->vel.set(vel[0] + rsRandf(80.0f) - 40.0f,
				vel[1] + rsRandf(80.0f) - 40.0f,
				vel[2] + rsRandf(80.0f) - 40.0f);
			newp->drag = 2.5f;
			newp->size = rsRandf(8.0f) + 4.0f;
			newp->rgb.set(1.0f, 0.8f, 0.6f);
			newp->t = rsRandf(2.0f) + 1.0f;
			newp->tr = newp->t;
			newp->makeSmoke = 0;
		}
	}

	// trail from meteors
	if(type == METEOR)//流星尾
	{
		rsVec diff = xyz - lastxyz;
		// distance rocket traveled since last frame
		sparkTrailLength += diff.length();
		// number of sparks to release
		int sparks = int(sparkTrailLength * 0.1f);
		rsVec smkpos = lastxyz;
		// release star every 10 feet
		float multiplier = 10.0f / sparkTrailLength;
		for(i=0; i<sparks; ++i){
			smkpos += diff * multiplier;
			newp = m_ParticleSystem->AddParticle();
			newp->initStar();
			newp->xyz = smkpos;
			newp->vel.set(vel[0] + rsRandf(40.0f) - 20.0f, 
				vel[1] + rsRandf(40.0f) - 20.0f, 
				vel[2] + rsRandf(40.0f) - 20.0f);
			newp->rgb = rgb;
			newp->drag = 2.0f;
			newp->t = newp->tr = rsRandf(0.5f) + 1.5f;
			newp->size = 10.0f;
			newp->makeSmoke = 0;
		}
		sparkTrailLength -= float(sparks) * 10.0f;
	}

	// trail from bees
	if(type == BEE)
	{
		rsVec diff = xyz - lastxyz;
		// distance rocket traveled since last frame
		sparkTrailLength += diff.length();
		// number of sparks to release
		int sparks = int(sparkTrailLength * 0.1f);
		rsVec smkpos = lastxyz;
		// release sparks every 10 feet
		float multiplier = 10.0f / sparkTrailLength;
		for(i=0; i<sparks; i++){
			smkpos += diff * multiplier;
			newp = m_ParticleSystem->AddParticle();
			newp->initStar();
			newp->xyz = smkpos;
			newp->vel.set(rsRandf(100.0f) - 50.0f - vel[0] * 0.5f,
				rsRandf(100.0f) - 50.0f - vel[1] * 0.5f,
				rsRandf(100.0f) - 50.0f - vel[2] * 0.5f);
			newp->rgb = rgb;
			newp->t = newp->tr = rsRandf(0.1f) + 0.15f;
			newp->size = 7.0f;
			newp->displayList = m_Scene->m_Flare.flarelist[3];
			newp->makeSmoke = 0;
		}
		sparkTrailLength -= float(sparks) * 10.0f;
	}

	// smoke and cloud illumination from rockets and explosions//烟雾，云彩照明
	if(dIllumination && ((type == ROCKET) || (type == FOUNTAIN) || (type == EXPLOSION)))
		Illuminate(this,dClouds,theWorld);

	// PullParticle of particles by suckers
	if(type == SUCKER)
		PullParticle(this,frameTime);

	// PushParticle of particles by shockwaves
	if(type == SHOCKWAVE)
		PushParticle(this,frameTime);

	// StretchParticle of particles by stretchers
	if(type == STRETCHER)
		StretchParticle(this,frameTime);
	
}
void Particle::draw(float * billboardMat,World * theWorld)
{

	if(life <= 0.0f) return;//生命结束

	if(depth < 0.0f && type != SHOCKWAVE) return;// cull small particles that are behind camera
	
	if(type == POPPER) return;// don't draw invisible particles


	glPushMatrix();

	glTranslatef(xyz[0], xyz[1], xyz[2]);

	switch(type)
	{
	  case SHOCKWAVE:		
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	  glPushMatrix();
	  glScalef(size, size, size);
	  drawShockwave(life, float(sqrt(size)) * 0.05f,theWorld);
	  glPopMatrix();
	  glMultMatrixf(billboardMat);
	  glScalef(size * 0.1f, size * 0.1f, size * 0.1f);
	  glColor4f(0.5f, 1.0f, 0.5f, bright);
	  glCallList(m_Scene->m_Flare.flarelist[0]);
	  glScalef(0.35f, 0.35f, 0.35f);
	  glColor4f(1.0f, 1.0f, 1.0f, bright);
	  glCallList(m_Scene->m_Flare.flarelist[0]);
	  if(life > 0.7f)
	  {  // Big torus just for fun	
	   glScalef(100.0f, 100.0f, 100.0f);
	   glColor4f(1.0f, life, 1.0f, (life - 0.7f) * 3.333f);
	   glCallList(m_Scene->m_Flare.flarelist[2]);
	  }
	  break;

	case SMOKE:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glMultMatrixf(billboardMat);
		glScalef(size, size, size);
		glColor4f(rgb[0], rgb[1], rgb[2], bright);
		glCallList(displayList);
		break;

	case EXPLOSION:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glMultMatrixf(billboardMat);
		glScalef(size, size, size);
		glColor4f(1.0f, 1.0f, 1.0f, bright);
		glScalef(bright, bright, bright);
		glCallList(displayList);
		break;

	default:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glMultMatrixf(billboardMat);
		glScalef(size, size, size);
		glColor4f(rgb[0], rgb[1], rgb[2], bright);
		glCallList(displayList);
		glScalef(0.35f, 0.35f, 0.35f);
		glColor4f(1.0f, 1.0f, 1.0f, bright);
		glCallList(displayList);
	}

   glPopMatrix();
}


void Particle::PullParticle(Particle* suck,float frameTime)
{
	rsVec diff;
	float pulldistsquared;
	float pullconst = (1.0f - suck->life) * 0.01f * frameTime;

	for(unsigned int i=0; i<m_ParticleSystem->last_particle; ++i)
	{
		Particle* puller = &(m_ParticleSystem->particles[i]);
		diff = suck->xyz - puller->xyz;
		pulldistsquared = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];
		if(pulldistsquared < 250000.0f && pulldistsquared != 0.0f){
			if(puller->type != SUCKER && puller->type != STRETCHER
				&& puller->type != SHOCKWAVE && puller->type != BIGMAMA){
				diff.normalize();
				puller->vel += diff * ((250000.0f - pulldistsquared) * pullconst);
			}
		}
	}
}


void Particle::PushParticle(Particle* shock,float frameTime)
{
	rsVec diff;
	float pushdistsquared;
	float pushconst = (1.0f - shock->life) * 0.002f * frameTime;

	for(unsigned int i=0; i<m_ParticleSystem->last_particle; ++i)
	{
		Particle* pusher = &(m_ParticleSystem->particles[i]);
		diff = pusher->xyz - shock->xyz;
		pushdistsquared = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];
		if(pushdistsquared < 640000.0f && pushdistsquared != 0.0f){
			if(pusher->type != SUCKER && pusher->type != STRETCHER
				&& pusher->type != SHOCKWAVE && pusher->type != BIGMAMA){
				diff.normalize();
				pusher->vel += diff * ((640000.0f - pushdistsquared) * pushconst);
			}
		}
	}
}


// vertical StretchParticle of other particles (x, z sucking; y PushParticle)
void Particle::StretchParticle(Particle* stretch,float frameTime)
{
	rsVec diff;
	float stretchdistsquared, temp;
	float stretchconst = (1.0f - stretch->life) * 0.002f * frameTime;

	for(unsigned int i=0; i<m_ParticleSystem->last_particle; ++i){
		Particle* stretcher =  &(m_ParticleSystem->particles[i]);
		diff = stretch->xyz - stretcher->xyz;
		stretchdistsquared = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];
		if(stretchdistsquared < 640000.0f && stretchdistsquared != 0.0f && stretcher->type != STRETCHER){
			diff.normalize();
			temp = (640000.0f - stretchdistsquared) * stretchconst;
			stretcher->vel[0] += diff[0] * temp * 5.0f;
			stretcher->vel[1] -= diff[1] * temp;
			stretcher->vel[2] += diff[2] * temp * 5.0f;
		}
	}
}
// Rockets and explosions Illuminate smoke
// Only explosions Illuminate clouds
void Particle::Illuminate(Particle* ill,int dClouds,World * theWorld)
{
	float temp;
	// desaturate illumination colors
	rsVec newrgb(ill->rgb[0] * 0.6f + 0.4f, ill->rgb[1] * 0.6f + 0.4f, ill->rgb[2] * 0.6f + 0.4f);

	// Smoke illumination
	if((ill->type == ROCKET) || (ill->type == FOUNTAIN))
	{
		float distsquared;
		for(unsigned int i=0; i<m_ParticleSystem->last_particle; ++i)
		{
		 Particle* smk = &(m_ParticleSystem->particles[i]);
		 if(smk->type == SMOKE)
		 {
		  distsquared = (ill->xyz[0] - smk->xyz[0]) * (ill->xyz[0] - smk->xyz[0])
			  + (ill->xyz[1] - smk->xyz[1]) * (ill->xyz[1] - smk->xyz[1])
			  + (ill->xyz[2] - smk->xyz[2]) * (ill->xyz[2] - smk->xyz[2]);
		  if(distsquared < 40000.0f)
		  {
			 temp = (40000.0f - distsquared) * 0.000025f;
			 temp = temp * temp * ill->bright;
			 smk->rgb[0] += temp * newrgb[0];
			 if(smk->rgb[0] > 1.0f) smk->rgb[0] = 1.0f;
			 smk->rgb[1] += temp * newrgb[1];
			 if(smk->rgb[1] > 1.0f) smk->rgb[1] = 1.0f;
			 smk->rgb[2] += temp * newrgb[2];
			 if(smk->rgb[2] > 1.0f) smk->rgb[2] = 1.0f;
			}
		  }
		}
	}
	if(ill->type == EXPLOSION)
	{
		float distsquared;
		for(unsigned int i=0; i<m_ParticleSystem->last_particle; ++i)
		{
			Particle* smk = &(m_ParticleSystem->particles[i]);
			if(smk->type == SMOKE){
				distsquared = (ill->xyz[0] - smk->xyz[0]) * (ill->xyz[0] - smk->xyz[0])
					+ (ill->xyz[1] - smk->xyz[1]) * (ill->xyz[1] - smk->xyz[1])
					+ (ill->xyz[2] - smk->xyz[2]) * (ill->xyz[2] - smk->xyz[2]);
				if(distsquared < 640000.0f){
					temp = (640000.0f - distsquared) * 0.0000015625f;
					temp = temp * temp * ill->bright;
					smk->rgb[0] += temp * newrgb[0];
					if(smk->rgb[0] > 1.0f)
						smk->rgb[0] = 1.0f;
					smk->rgb[1] += temp * newrgb[1];
					if(smk->rgb[1] > 1.0f)
						smk->rgb[1] = 1.0f;
					smk->rgb[2] += temp * newrgb[2];
					if(smk->rgb[2] > 1.0f)
						smk->rgb[2] = 1.0f;
				}
			}
		}
	}

	// cloud illumination
	if(ill->type == EXPLOSION && dClouds)
	{
		int north, south, west, east;  // limits of cloud indices to inspect
		int halfmesh = CLOUDMESH / 2;
		float distsquared;
		// remember clouds have 20000-foot radius from the World class, hence 0.00005
		// Hardcoded values like this are evil, but oh well
		south = int((ill->xyz[2] - 1600.0f) * 0.00005f * float(halfmesh)) + halfmesh;
		north = int((ill->xyz[2] + 1600.0f) * 0.00005f * float(halfmesh) + 0.5f) + halfmesh;
		west = int((ill->xyz[0] - 1600.0f) * 0.00005f * float(halfmesh)) + halfmesh;
		east = int((ill->xyz[0] + 1600.0f) * 0.00005f * float(halfmesh) + 0.5f) + halfmesh;
		// bound these values just in case
		if(south < 0) south = 0; if(south > CLOUDMESH-1) south = CLOUDMESH-1;
		if(north < 0) north = 0; if(north > CLOUDMESH-1) north = CLOUDMESH-1;
		if(west < 0) west = 0; if(west > CLOUDMESH-1) west = CLOUDMESH-1;
		if(east < 0) east = 0; if(east > CLOUDMESH-1) east = CLOUDMESH-1;
		//do any necessary cloud illumination
		for(int i=west; i<=east; i++){
			for(int j=south; j<=north; j++)
			{
				distsquared = (theWorld->clouds[i][j][0] - ill->xyz[0]) * (theWorld->clouds[i][j][0] - ill->xyz[0])
					+ (theWorld->clouds[i][j][1] - ill->xyz[1]) * (theWorld->clouds[i][j][1] - ill->xyz[1])
					+ (theWorld->clouds[i][j][2] - ill->xyz[2]) * (theWorld->clouds[i][j][2] - ill->xyz[2]);
				if(distsquared < 2560000.0f){
					temp = (2560000.0f - distsquared) * 0.000000390625f;
					temp = temp * temp * ill->bright;
					theWorld->clouds[i][j][6] += temp * newrgb[0];
					if(theWorld->clouds[i][j][6] > 1.0f)
						theWorld->clouds[i][j][6] = 1.0f;
					theWorld->clouds[i][j][7] += temp * newrgb[1];
					if(theWorld->clouds[i][j][7] > 1.0f)
						theWorld->clouds[i][j][7] = 1.0f;
					theWorld->clouds[i][j][8] += temp * newrgb[2];
					if(theWorld->clouds[i][j][8] > 1.0f)
						theWorld->clouds[i][j][8] = 1.0f;
				}
			}
		}
	}
}


