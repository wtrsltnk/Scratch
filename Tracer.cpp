/* 
 * File:   Tracer.cpp
 * Author: wouter
 * 
 * Created on May 27, 2012, 4:49 PM
 */

#include "Tracer.h"
#include "indexarray.h"
#include <iostream>
#include <math.h>
#include <glad/glad.h>

#define EPSILON (1.0f/32.0f)

using namespace std;

void Tracer::Result::render()
{
	glColor3f(0,1,0);
	glDisable(GL_DEPTH_TEST);
	std::vector<Vector3>::iterator itr = this->intersections.begin();
	glPointSize(10);
	glBegin(GL_POINTS);
	for (;itr != this->intersections.end(); ++itr)
		glVertex3f((*itr).x(), (*itr).y(), (*itr).z());
	glColor3f(1, 0, 0);
	std::vector<Hit>::iterator i = this->faceIntersections.begin();
	for (;i != this->faceIntersections.end(); ++i)
		glVertex3f((*i).intersection.x(), (*i).intersection.y(), (*i).intersection.z());
	glEnd();
	glBegin(GL_LINE_STRIP);
	std::vector<Hit>::iterator r = this->faceIntersections.begin();
	for (;r != this->faceIntersections.end(); ++r)
	{
		glVertex3f((*r).intersection.x(), (*r).intersection.y(), (*r).intersection.z());
		std::vector<Vector3>::iterator a = (*r).face.begin();
		for (;a != (*r).face.end(); ++a)
			glVertex3f((*a).x(), (*a).y(), (*a).z());
	}
	glEnd();
	glPointSize(1);
	glLineWidth(2);
	glColor3f(1,1,0);
	glBegin(GL_LINE_STRIP);
	itr = this->points.begin();
	for (;itr != this->points.end(); ++itr)
		glVertex3f((*itr).x(), (*itr).y(), (*itr).z());
	glEnd();
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
}

Tracer::Tracer(Hl1DataManager* d)
	: data(d)
{
}

Tracer::~Tracer()
{
}

Tracer::Result Tracer::trace(const Vector3& start, const Vector3& end)
{
	this->result.fraction = -1;
	this->result.face = -1;
	this->result.points.push_back(start);
	
	this->checkNode(0, start, end);
	return this->result;
}

void Tracer::checkNode(int nodeIndex, const Vector3& start, const Vector3& end)
{
//	cout << "\t" << nodeIndex << endl;
	if (nodeIndex < 0)
	{
		HL1::tBSPLeaf* leaf = &this->data->bsp->leafs[-(nodeIndex+1)];
		for (int f = 0; f < leaf->markSurfacesCount; f++)
		{
			int faceIndex = this->data->bsp->marksurfaces[leaf->firstMarkSurface + f];
			StaticGeometry::tFace* face = &this->data->faces[faceIndex];
			HL1::tBSPPlane* plane = &face->plane;
			
			float startDistance =  plane->normal[0] * start[0]
								+plane->normal[1] * start[1]
								+plane->normal[2] * start[2]
								-plane->distance;

			float endDistance =  plane->normal[0] * end[0]
								+plane->normal[1] * end[1]
								+plane->normal[2] * end[2]
								-plane->distance;

			if (startDistance >= 0 && endDistance >= 0)
			{
			}
			else if (startDistance < 0 && endDistance < 0)
			{
			}
			else
			{
				this->result.face = faceIndex;
				float r = (1.0f / (fabs(startDistance)+fabs(endDistance)) * startDistance);
				Vector3 intersection = start + ((end-start) * r);
				cout << startDistance << " " << endDistance << " " << intersection << "-" << faceIndex << endl;
				Hit h;
				h.intersection = intersection;
				for (int i = 0; i < face->vertexCount; i++)
					h.face.push_back(Vector3(this->data->vertices[face->firstVertex+i].position));
				this->result.faceIntersections.push_back(h);
			}
		}
	}
	else
	{
		HL1::tBSPNode* node = &this->data->bsp->nodes[nodeIndex];
		HL1::tBSPPlane* plane = &this->data->bsp->planes[node->planeIndex];

		float startDistance =  plane->normal[0] * start[0]
							+plane->normal[1] * start[1]
							+plane->normal[2] * start[2]
							-plane->distance;

		float endDistance =  plane->normal[0] * end[0]
							+plane->normal[1] * end[1]
							+plane->normal[2] * end[2]
							-plane->distance;

		if (startDistance >= 0 && endDistance >= 0)
		{
			this->checkNode(node->children[0], start, end);
		}
		else if (startDistance < 0 && endDistance < 0)
		{
			this->checkNode(node->children[1], start, end);
		}
		else
		{
			float r = (1.0f / (fabs(startDistance)+fabs(endDistance)) * startDistance);
			Vector3 intersection = start + ((end-start) * r);
			this->result.intersections.push_back(intersection);
			this->checkNode(node->children[1], intersection, end);
			this->checkNode(node->children[0], start, intersection);
		}
	}
}


