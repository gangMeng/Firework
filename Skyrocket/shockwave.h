#ifndef _SHOCKWAVE_
#define _SHOCKWAVE_


class World;
void initShockwave();

// temp influences color intensity (0.0 - 1.0)
// texmove is amount to advance the texture coordinates
void drawShockwave(float temperature, float texmove,World * theWorld);



#endif