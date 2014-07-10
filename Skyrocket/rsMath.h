#ifndef RSMATH_H
#define RSMATH_H

#include <stdlib.h>
#include "rsVec.h"
#include "rsMatrix.h"
#include "rsQuat.h"
#include "rsTrigonometry.h"


#define RSEPSILON 0.000001f
#define RS_PIo2 1.5707963268f
#define RS_PI 3.14159265359f
#define RS_PIx2 6.28318530718f
#define RS_RAD2DEG 57.2957795131f


// Useful random number functions
// Don't forget to initialize with srand()
inline int rsRandi(int x)
{
  return rand() % x;
}
inline float rsRandf(float x)
{
  return x * (float(rand()) / float(RAND_MAX));
}

#endif
