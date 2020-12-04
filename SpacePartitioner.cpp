/* 
 * File:   SpacePartitioner.cpp
 * Author: Administrator
 * 
 * Created on 25 mei 2012, 23:07
 */

#include "SpacePartitioner.h"
#include "Tracer.h"
#include <glad/glad.h>
#include "vector3.h"
#include <memory.h>

SpacePartitioner::SpacePartitioner()
	: planeCount(0), planes(0), nodeCount(0), nodes(0), leafCount(0), leafs(0), markedFacesCount(0), markedFaces(0)
{
}

SpacePartitioner::~SpacePartitioner()
{
	this->clear();
}

void SpacePartitioner::clear()
{
	if (this->planes != 0)
		delete []this->planes;
	this->planeCount = 0;
	this->planes = 0;
	
	if (this->nodes != 0)
		delete []this->nodes;
	this->nodeCount = 0;
	this->nodes = 0;
	
	if (this->leafs != 0)
		delete []this->leafs;
	this->leafCount = 0;
	this->leafs = 0;
}

int SpacePartitioner::findLeafAtPoint(const Vector3& point)
{
    int i = 0;

    while(i >= 0)
    {
        tNode* node = &this->nodes[i];
        tPlane* plane = &this->planes[node->splittingPlaneIndex];

        float distance;

		// f(x,y,z) = Ax+Ay+Az-D
		distance =  plane->normal[0] * -point[0]
					+plane->normal[1] * -point[1]
					+plane->normal[2] * -point[2]
					-plane->d;

        if(distance >= 0)				// in front or on the plane
            i = node->children[0];
        else							// behind the plane
            i = node->children[1];
    }

    return -(i+1);
}

void SpacePartitioner::renderAllNodes()
{
	for (int n = 0; n < this->nodeCount; n++)
		this->renderNode(n);
}

void SpacePartitioner::renderAllLeafs()
{
	for (int l = 0; l < this->leafCount; l++)
		this->renderLeaf(l);
}

void SpacePartitioner::renderAllNodesAndLeafs()
{
	this->renderAllNodes();
	this->renderAllLeafs();
}

void SpacePartitioner::renderList(const std::set<int>& nodes, const std::set<int>& leafs)
{
	for (std::set<int>::const_iterator n = nodes.begin(); n != nodes.end(); ++n)
		this->renderNode(*n);
	for (std::set<int>::const_iterator l = leafs.begin(); l != leafs.end(); ++l)
		this->renderLeaf(*l);
}

void SpacePartitioner::renderNode(int index)
{
	if (index >= 0 && index < this->nodeCount)
		this->renderBoundingBox(this->nodes[index].mins, this->nodes[index].maxs);
}

void SpacePartitioner::renderLeaf(int index)
{
	if (index >= 0 && index < this->leafCount)
		this->renderBoundingBox(this->leafs[index].mins, this->leafs[index].maxs);
}

void SpacePartitioner::renderBoundingBox(const float mins[], const float maxs[])
{
	glBegin(GL_LINES);
	
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

void SpacePartitioner::setPlanes(int count, tPlane* p)
{
	// Copy the given faces to a new array of faces
	if (this->planes != 0)
		delete []this->planes;
	this->planes = new SpacePartitioner::tPlane[count];
	memcpy(this->planes, p, sizeof(SpacePartitioner::tPlane) * count);
	this->planeCount = count;
}

void SpacePartitioner::setNodes(int count, SpacePartitioner::tNode* n)
{
	// Copy the given faces to a new array of faces
	if (this->nodes != 0)
		delete []this->nodes;
	this->nodes = new SpacePartitioner::tNode[count];
	memcpy(this->nodes, n, sizeof(SpacePartitioner::tNode) * count);
	this->nodeCount = count;
}

void SpacePartitioner::setLeafs(int count, SpacePartitioner::tLeaf* l)
{
	// Copy the given faces to a new array of faces
	if (this->leafs != 0)
		delete []this->leafs;
	this->leafs = new SpacePartitioner::tLeaf[count];
	memcpy(this->leafs, l, sizeof(SpacePartitioner::tLeaf) * count);
	this->leafCount = count;
}

void SpacePartitioner::setMarkedFaces(int count, unsigned short* m)
{
	// Copy the given faces to a new array of faces
	if (this->markedFaces != 0)
		delete []this->markedFaces;
	this->markedFaces = new unsigned short[count];
	memcpy(this->markedFaces, m, sizeof(unsigned short) * count);
	this->markedFacesCount = count;
}

