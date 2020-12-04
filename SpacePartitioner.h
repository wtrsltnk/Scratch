/* 
 * File:   SpacePartitioner.h
 * Author: Administrator
 *
 * Created on 25 mei 2012, 23:07
 */

#ifndef SPACEPARTITIONER_H
#define	SPACEPARTITIONER_H

#include <set>
#include "vector3.h"

class Tracer;

class SpacePartitioner
{
public:
	typedef struct sPlane
	{
		float normal[3];
		float d;
		
	} tPlane;
	
	typedef struct sNode
	{
		int splittingPlaneIndex;
		float mins[3];
		float maxs[3];
		int children[2];
		
	} tNode;
	
	typedef struct sLeaf
	{
		float mins[3];
		float maxs[3];
		int firstMarkedFace;
		int markedFaceCount;
		
	} tLeaf;
	
public:
	SpacePartitioner();
	virtual ~SpacePartitioner();
	
	int findLeafAtPoint(const Vector3& point);
	
	void clear();
	void renderAllNodes();
	void renderAllLeafs();
	void renderAllNodesAndLeafs();
	void renderList(const std::set<int>& nodes, const std::set<int>& leafs);
	void renderNode(int index);
	void renderLeaf(int index);
	void renderBoundingBox(const float mins[], const float maxs[]);
	
	void setPlanes(int count, tPlane* planes);
	void setNodes(int count, tNode* nodes);
	void setLeafs(int count, tLeaf* leafs);
	void setMarkedFaces(int count, unsigned short* markedFaces);
	
	int planeCount;
	tPlane* planes;
	
	int nodeCount;
	tNode* nodes;
	
	int leafCount;
	tLeaf* leafs;
	
	int markedFacesCount;
	unsigned short* markedFaces;

};

#endif	/* SPACEPARTITIONER_H */

