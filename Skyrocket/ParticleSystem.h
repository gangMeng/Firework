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
   
   void Update();//��������ϵͳ

   void AllocNewParticle();//���������ӿռ�

   void DeallocParticle();

   Particle *AddParticle();
   
   void RemoveParticle(unsigned int rempart);

   std::vector<Particle> particles;//����ϵͳ

   unsigned int last_particle;//����ʹ����Ŀ

};

#endif