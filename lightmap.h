#ifndef _LIGHTMAP_H
#define	_LIGHTMAP_H

#include "hl1types.h"
#include "Hl1BspData.h"
#include "texture.h"

void CalcSurfaceExtents(HL1::tBSPFace* in, float *mins, float *maxs, Hl1BspData* bsp);
void ComputeLightmap(HL1::tBSPFace* in, float *mins, float *maxs, Hl1BspData* bsp, Texture& lightmap);

#endif	/* _LIGHTMAP_H */

