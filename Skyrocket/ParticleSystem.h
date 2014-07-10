#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#ifndef _PARTICLE_
#include"Particle.h"
#endif

#ifndef _CONSTRAINEDFIRE_
#include"../Fire/ConstrainedFire.h"
#endif


#include<vector>

class ParticleSystem
{
 public:

   ParticleSystem();
   
   void Update();//更新粒子系统

   void AllocNewParticle();//分配新粒子空间

   void DeallocParticle();

   Particle *AddParticle();
   
   void RemoveParticle(unsigned int rempart);

   std::vector<Particle> particles;//粒子系统

   unsigned int last_particle;//粒子使用数目

};

#endif