/* 
 * File:   StaticGeometry.h
 * Author: Administrator
 *
 * Created on 25 mei 2012, 22:58
 */

#ifndef STATICGEOMETRY_H
#define	STATICGEOMETRY_H

#include "hl1types.h"
#include "texture.h"
#include <set>

class StaticGeometry
{
public:
	typedef struct sVertex
	{
		float position[3];
		float texcoord[2][2];
		float normal[3];
		unsigned char color[4];

	} tVertex;
	
	typedef struct sFace
	{
		int index;
		unsigned int mode;
		int flags;
		int firstVertex;
		int vertexCount;

		HL1::tBSPPlane plane;
		Texture lightmap;

	} tFace;

public:
	StaticGeometry();
	virtual ~StaticGeometry();
	
	void clear();
	void renderAllFaces();
	void renderFacesFromList(const std::set<int>& indices);
	void renderFace(int index);
	
	virtual void setVertices(int count, tVertex* vertices);
	virtual void setFaces(int count, tFace* faces);
	
	int vertexCount;
	tVertex* vertices;
	
	int faceCount;
	tFace* faces;

};

#endif	/* STATICGEOMETRY_H */

