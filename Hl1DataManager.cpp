/* 
 * File:   Hl1DataManager.cpp
 * Author: wouter
 * 
 * Created on May 27, 2012, 10:19 AM
 */

#include "Hl1DataManager.h"
#include "Tracer.h"
#include <glad/glad.h>
#include "vector3.h"
#include "indexarray.h"
#include "lightmap.h"
#include <iostream>

using namespace std;

Hl1DataManager::Hl1DataManager()
	: bsp(0), currentLeaf(0)
{
}

Hl1DataManager::~Hl1DataManager()
{
}

bool Hl1DataManager::load(const char* filename)
{
	this->bsp = new Hl1BspData(filename);
	
	if (this->bsp->isLoaded())
	{
		bool result = true;
		
		if (this->loadStaticGeometry(*bsp) == false)
			result = false;
		
		if (this->vis.Decompress(this->bsp->leafCount, this->bsp->leafs, this->bsp->models, this->bsp->visibilityData) == false)
			result = false;
		
		return result;
	}
	return false;
}

bool Hl1DataManager::loadStaticGeometry(Hl1BspData& data)
{
	StaticGeometry::tFace* faces = new StaticGeometry::tFace[data.faceCount];
	IndexArray<StaticGeometry::tVertex> vertices;

	for (int f = 0; f < data.faceCount; f++)
	{
		HL1::tBSPFace& inFace = data.faces[f];
		HL1::tBSPPlane& bspPlane = data.planes[inFace.planeIndex];
		HL1::tBSPTexInfo& texInfo = data.texinfos[inFace.texinfo];
		HL1::tBSPTexInfo& bspTexInfo = data.texinfos[inFace.texinfo];
		HL1::tBSPMipTexHeader* bspMiptex = data.getMiptex(bspTexInfo.miptexIndex);
		StaticGeometry::tFace* outFace = &faces[f];

		if (inFace.edgeCount == 3) outFace->mode = GL_TRIANGLES;
		if (inFace.edgeCount == 4) outFace->mode = GL_QUADS;
		if (inFace.edgeCount >= 5) outFace->mode = GL_POLYGON;

		outFace->index = f;
		outFace->firstVertex = vertices.current();
		outFace->vertexCount = inFace.edgeCount;
		outFace->flags = bspTexInfo.flags;
		outFace->plane = bspPlane;
		// Uncomment onderstaande, voor collision detection bij de faces is heir iets mee aan de hand....
		if (inFace.side == 1)
		{
			outFace->plane.normal[0] = -outFace->plane.normal[0];
			outFace->plane.normal[1] = -outFace->plane.normal[1];
			outFace->plane.normal[2] = -outFace->plane.normal[2];
			outFace->plane.distance = -outFace->plane.distance;
		}

        float min[2], max[2];

		if (data.lightingData != 0)
		{
			// Calculate and grab the lightmap buffer
			CalcSurfaceExtents(&inFace, min, max, &data);
			ComputeLightmap(&inFace, min, max, &data, outFace->lightmap);
			outFace->lightmap.upload();
		}

		// Create a vertex list for this face
		for (int e = 0; e < inFace.edgeCount; e++)
		{
			StaticGeometry::tVertex v;

			int edgeIndex = data.surfedges[inFace.firstEdge + e];
			if (edgeIndex < 0)
			{
				edgeIndex = -edgeIndex;
				HL1::tBSPEdge* edge = &data.edges[edgeIndex];
				v.position[0] = data.vertices[edge->vertex[1]].point[0];
				v.position[1] = data.vertices[edge->vertex[1]].point[1];
				v.position[2] = data.vertices[edge->vertex[1]].point[2];
			}
			else
			{
				HL1::tBSPEdge* edge = &data.edges[edgeIndex];
				v.position[0] = data.vertices[edge->vertex[0]].point[0];
				v.position[1] = data.vertices[edge->vertex[0]].point[1];
				v.position[2] = data.vertices[edge->vertex[0]].point[2];
			}

			// Calculate the texture texcoords
			Vector3 vec(v.position);
            float s = vec.dotProduct(Vector3(texInfo.vecs[0])) + texInfo.vecs[0][3];
            float t = vec.dotProduct(Vector3(texInfo.vecs[1])) + texInfo.vecs[1][3];
            v.texcoord[0][0] = s / float(bspMiptex->width);
            v.texcoord[0][1] = t / float(bspMiptex->height);

			if (data.lightingData != 0)
			{
				// Calculate the lightmap texcoords
				v.texcoord[1][0] = ((float(outFace->lightmap.width()) / 2.0f) + (s - ((min[0] + max[0]) / 2.0f)) / 16.0f) / float(outFace->lightmap.width());
				v.texcoord[1][1] = ((float(outFace->lightmap.height()) / 2.0f) + (t - ((min[1] + max[1]) / 2.0f)) / 16.0f) / float(outFace->lightmap.height());
			}
			vertices.add(&v);
		}
	}
	this->setVertices(vertices.current(), vertices);
	this->setFaces(data.faceCount, faces);
	return true;
}

void Hl1DataManager::uploadLightmaps()
{
	for (int f = 0; f < this->faceCount; f++)
		this->faces[f].lightmap.upload();
}

void Hl1DataManager::setCurrentLeaf(const Vector3& point)
{
    int i = 0;

    while(i >= 0)
    {
        HL1::tBSPNode* node = &this->bsp->nodes[i];
        HL1::tBSPPlane* plane = &this->bsp->planes[node->planeIndex];

        float distance;

		// f(x,y,z) = Ax+Ay+Az-D
		distance =  plane->normal[0] * point[0]
					+plane->normal[1] * point[1]
					+plane->normal[2] * point[2]
					-plane->distance;

        if(distance >= 0)				// in front or on the plane
            i = node->children[0];
        else							// behind the plane
            i = node->children[1];
    }

    this->currentLeaf = -(i+1);
}

int Hl1DataManager::getCurrentLeaf() const
{
	return this->currentLeaf;
}

Tracer* Hl1DataManager::createTracer()
{
	return new Tracer(this);
}

void Hl1DataManager::castShadows(const float sundirection[])
{
 	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

	// first pass, stencil operation decreases stencil value
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	
	
	// second pass, stencil operation increases stencil value
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	
	
	glFrontFace(GL_CCW);
	glColorMask(1, 1, 1, 1);

	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.10f);
		glVertex3f(-0.1f,-0.1f,-0.10f);
		glVertex3f( 0.1f, 0.1f,-0.10f);
		glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);
}
	
void Hl1DataManager::renderVisibleFaces()
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

	for (int i = 0; i < this->bsp->leafs[currentLeaf].markSurfacesCount; i++)
		this->renderFace(this->bsp->marksurfaces[this->bsp->leafs[currentLeaf].firstMarkSurface + i]);
	
	tVISLeaf& visLeaf = this->vis.m_vis[currentLeaf];
	for (int i = 0; i < visLeaf.leafCount; i++)
	{
		HL1::tBSPLeaf& leaf = this->bsp->leafs[visLeaf.leafs[i]];
		for (int f = 0; f < leaf.markSurfacesCount; f++)
			this->renderFace(this->bsp->marksurfaces[leaf.firstMarkSurface + f]);
	}
}

void Hl1DataManager::renderAllNodes()
{
	for (int n = 0; n < this->bsp->nodeCount; n++)
		this->renderNode(n);
}

void Hl1DataManager::renderAllLeafs()
{
	for (int l = 0; l < this->bsp->leafCount; l++)
		this->renderLeaf(l);
}

void Hl1DataManager::renderAllNodesAndLeafs()
{
	this->renderAllNodes();
	this->renderAllLeafs();
}

void Hl1DataManager::renderList(const std::set<int>& nodes, const std::set<int>& leafs)
{
	for (std::set<int>::const_iterator n = nodes.begin(); n != nodes.end(); ++n)
		this->renderNode(*n);
	for (std::set<int>::const_iterator l = leafs.begin(); l != leafs.end(); ++l)
		this->renderLeaf(*l);
}

void Hl1DataManager::renderNode(int index)
{
	if (index >= 0 && index < this->bsp->nodeCount)
		this->renderBoundingBox(this->bsp->nodes[index].mins, this->bsp->nodes[index].maxs);
}

void Hl1DataManager::renderLeaf(int index)
{
	if (index >= 0 && index < this->bsp->leafCount)
		this->renderBoundingBox(this->bsp->leafs[index].mins, this->bsp->leafs[index].maxs);
}

void Hl1DataManager::renderCurrentLeafWithFaces()
{
	glDisable(GL_DEPTH_TEST);
	glLineWidth(2.0f);
	this->renderLeaf(currentLeaf);
	for (int i = 0; i < this->bsp->leafs[currentLeaf].markSurfacesCount; i++)
		this->renderFace(this->bsp->marksurfaces[this->bsp->leafs[currentLeaf].firstMarkSurface + i]);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1.0f);
}

void Hl1DataManager::renderBoundingBox(const short smins[], const short smaxs[])
{
    glBegin(GL_LINES);

    float mins[3];
    float maxs[3];
    for (int i = 0; i < 3; i++)
    {
        maxs[i] = smaxs[i];
        mins[i] = smins[i];
    }

	// bottom
	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], mins[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], mins[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], mins[2]);

	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], mins[1], mins[2]);
	
	// top
	glVertex3f(mins[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], maxs[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], maxs[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], maxs[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	//sides
	glVertex3f(mins[0], maxs[1], mins[2]);
	glVertex3f(mins[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], maxs[1], mins[2]);
	glVertex3f(maxs[0], maxs[1], maxs[2]);

	glVertex3f(maxs[0], mins[1], mins[2]);
	glVertex3f(maxs[0], mins[1], maxs[2]);

	glVertex3f(mins[0], mins[1], mins[2]);
	glVertex3f(mins[0], mins[1], maxs[2]);
	glEnd();
}

