/* 
 * File:   SunShadows.cpp
 * Author: wouter
 * 
 * Created on July 2, 2012, 9:01 PM
 */

#include <glad/glad.h>
#include <iostream>

#include "SunShadows.h"
#include "vector3.h"
#include "Hl1DataManager.h"

using namespace std;

SunShadows::SunShadows(Hl1DataManager* data, const Vector3& vec)
	: data(data)
{
	float moveOut = 10;
	for (int f = 0; f < data->faceCount; f++)
	{
		float dot = vec.dotProduct(data->faces[f].plane.normal);
		if (dot > 0 && data->faces[f].flags == 0)
		{
			Caster* caster = new Caster();
			this->casters.push_back(caster);

			caster->count = data->faces[f].vertexCount;
			caster->vertices = new Hl1DataManager::tVertex[data->faces[f].vertexCount * 2];
			for (int v = 0; v < data->faces[f].vertexCount; v++)
			{
				caster->vertices[v * 2] = data->vertices[data->faces[f].firstVertex + v];
				caster->vertices[v * 2 + 1] = data->vertices[data->faces[f].firstVertex + v];
				caster->vertices[v * 2 + 1].position[0] += (vec.x() * moveOut);
				caster->vertices[v * 2 + 1].position[1] += (vec.y() * moveOut);
				caster->vertices[v * 2 + 1].position[2] += (vec.z() * moveOut);
			}
		}
	}
}

SunShadows::~SunShadows()
{
}

float colors[][3] = {
	{ 1,0,0 },
	{ 1,1,0 },
	{ 0,1,1 },
	{ 0,0,1 },
	{ 0,1,0 },
	{ 1,0,1 },
	{ 0.5,1,0 },
	{ 0,1,0.5 }
};

void SunShadows::render(int i)
{
	for (int i = 0; i < int(this->casters.size()); i++)
	{
		Caster* caster = this->casters[i];
		
		glBegin(GL_QUADS);
		for (int v = 0; v < caster->count-1; v++)
		{
			glVertex3fv(caster->vertices[v * 2 + 1].position);
			glVertex3fv(caster->vertices[v * 2].position);
			glVertex3fv(caster->vertices[(v+1) * 2].position);
			glVertex3fv(caster->vertices[(v+1) * 2 + 1].position);
		}
		glVertex3fv(caster->vertices[(caster->count-1) * 2 + 1].position);
		glVertex3fv(caster->vertices[(caster->count-1) * 2].position);
		glVertex3fv(caster->vertices[0].position);
		glVertex3fv(caster->vertices[1].position);
		glEnd();
		
		
		glBegin(GL_POLYGON);
		for (int v = 0; v < caster->count; v++)
			glVertex3fv(caster->vertices[v * 2].position);
		glEnd();
	}
}
