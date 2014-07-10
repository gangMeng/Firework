#ifndef _PERLINNOISE_
#define _PERLINNOISE_


//  ÔëÉùº¯Êý



float noise1(const float x);
float noise2(const float x, const float y);
float noise3(const float x, const float y, const float z);

float turbulence2(const float x, const float y, float freq);
float turbulence3(const float x, const float y, const float z, float freq);

float tileableNoise1(const float x, const float w);
float tileableNoise2(const float x, const float y, const float w, const float h);
float tileableNoise3(const float x, const float y, const float z, const float w, const float h, const float d);

float tileableTurbulence2(const float x, const float y, const float w, const float h, float freq);
float tileableTurbulence3(const float x, const float y, const float z, const float w, const float h, const float d, float freq);

void initPerlin();


#endif 
