#include "lightmap.h"
#include <math.h>
#include <string.h>

//
// the following computations are based on:
// PolyEngine (c) Alexey Goloshubin and Quake I source by id Software
//

void CalcSurfaceExtents (HL1::tBSPFace* in, float* mins, float* maxs, Hl1BspData* bsp)
{
	HL1::tBSPTexInfo* t = bsp->texinfos + in->texinfo;

	mins[0] = mins[1] =  999999;
	maxs[0] = maxs[1] = -999999;

	for (int i = 0; i < in->edgeCount; i++)
	{
		HL1::tBSPVertex* v;
		int e = bsp->surfedges[in->firstEdge + i];
		if (e >= 0)
			v = &bsp->vertices[bsp->edges[e].vertex[0]];
		else
			v = &bsp->vertices[bsp->edges[-e].vertex[1]];

		for (int j = 0; j < 2; j++)
		{
			int val = v->point[0] * t->vecs[j][0] + v->point[1] * t->vecs[j][1] + v->point[2] * t->vecs[j][2] +t->vecs[j][3];
			if (val < mins[j]) mins[j] = val;
			if (val > maxs[j]) maxs[j] = val;
		}
	}
}

void ComputeLightmap(HL1::tBSPFace* in, float* mins, float* maxs, Hl1BspData* bsp, Texture& lightmap)
{
	int size[2];
	int width, height;

	// compute lightmap size
	for (int c = 0; c < 2; c++)
	{
		float tmin = (float) floor(mins[c]/16.0f);
		float tmax = (float) ceil(maxs[c]/16.0f);

		size[c] = (int) (tmax-tmin);
	}

	width = size[0] + 1;
	height = size[1] + 1;

	int c = 0;
	while (in->styles[c] == -1)
		c++;

	int lsz = width * height * 3;
	unsigned char* data = (unsigned char *)malloc(lsz);
	memcpy(data, bsp->lightingData + in->lightOffset + (lsz * c), lsz);

	float light_adjust = 0.5f;

	for (int i = 0; i < lsz; i++)
	{
		float f = powf((data[i] + 1) / 256.0f, light_adjust);
		int inf = f * 255.0f + 0.5f;
		if (inf < 0) inf = 0;
		if (inf > 255) inf = 255;
		data[i] = int(inf);
	}

	lightmap.setData(width, height, 3, data);

	free(data);
}
