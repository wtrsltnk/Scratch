/* 
 * File:   StaticGeometry.cpp
 * Author: Administrator
 * 
 * Created on 25 mei 2012, 22:58
 */

#include "StaticGeometry.h"
#include <glad/glad.h>
#include <memory.h>

StaticGeometry::StaticGeometry()
	: vertexCount(0), vertices(0), faceCount(0), faces(0)
{
}

StaticGeometry::~StaticGeometry()
{
	this->clear();
}

void StaticGeometry::clear()
{
	if (this->vertices != 0)
		delete []this->vertices;
	this->vertexCount = 0;
	this->vertices = 0;
	
	if (this->faces != 0)
		delete []this->faces;
	this->faceCount = 0;
	this->faces = 0;
}

void StaticGeometry::renderAllFaces()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(tVertex), &this->vertices[0].position[0]);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(tVertex), &this->vertices[0].normal[0]);

	glClientActiveTextureARB(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(tVertex), &this->vertices[0].texcoord[0][0]);

	glClientActiveTextureARB(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(tVertex), &this->vertices[0].texcoord[1][0]);

	for (int f = 0; f < this->faceCount; f++)
		this->renderFace(f);
}

void StaticGeometry::renderFacesFromList(const std::set<int>& indices)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(tVertex), &this->vertices[0].position[0]);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(tVertex), &this->vertices[0].normal[0]);

	glClientActiveTextureARB(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(tVertex), &this->vertices[0].texcoord[0][0]);

	glClientActiveTextureARB(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(tVertex), &this->vertices[0].texcoord[1][0]);

	for (std::set<int>::const_iterator f = indices.begin(); f != indices.end(); ++f)
		this->renderFace(*f);
}

void StaticGeometry::renderFace(int index)
{
	if (index >= 0 && index < this->faceCount)
	{
		tFace& face = this->faces[index];
		
		if (face.flags != 0)
			return;

		glActiveTextureARB(GL_TEXTURE1);
		face.lightmap.use();

		glDrawArrays(face.mode, face.firstVertex, face.vertexCount);
	}
}

void StaticGeometry::setVertices(int count, tVertex* vertices)
{
	if (this->vertices != 0)
		delete []this->vertices;
	this->vertices = new tVertex[count];
	memcpy(this->vertices, vertices, sizeof(tVertex) * count);
	this->vertexCount = count;
}

void StaticGeometry::setFaces(int count, tFace* faces)
{
	if (this->faces != 0)
		delete []this->faces;
	this->faces = new tFace[count];
	memcpy(this->faces, faces, sizeof(tFace) * count);
	this->faceCount = count;
}
