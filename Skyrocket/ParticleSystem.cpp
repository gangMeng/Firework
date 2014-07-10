#include "stdafx.h"
#include"ParticleSystem.h"
ParticleSystem * Particle::m_ParticleSystem = NULL;//粒子系统初始化

ParticleSystem::ParticleSystem()
{
  last_particle = 0;

  Particle::m_ParticleSystem = this;
 
  ConstrainedFire::m_ParticleSystem = this;

 
 
}
void ParticleSystem::Update()
{
 

}
void ParticleSystem::AllocNewParticle()
{
  // Make more particles if necessary (always keep 1000 extra).
  // Ordinarily, you would resize as needed during the update loop, probably in the
  // AddParticle() function.  But that logic doesn't work with this particle system
  // because particles can spawn other particles.  resizing the vector, and, thus, 
  // moving all particle addresses, doesn't work if you are in the middle of
  // updating a particle.

  const unsigned int size = (unsigned int)particles.size();
  if(particles.size() - int(last_particle) < 50000) 
	  particles.resize(particles.size() + 100000);
 
}
void ParticleSystem::DeallocParticle()
{
 for(unsigned int i=0; i<last_particle; i++)
 {
   Particle* curpart(&(particles[i]));
   if(curpart->life <= 0.0f || curpart->xyz[1] < 0.0f)
   RemoveParticle(i);
  }
}
Particle * ParticleSystem::AddParticle()
{	
  if(last_particle < particles.size()) ++last_particle;

  return &(particles[last_particle-1]);
}


void ParticleSystem::RemoveParticle(unsigned int rempart)
{
	
	--last_particle;
	if(rempart != last_particle) particles[rempart] = particles[last_particle];

}
