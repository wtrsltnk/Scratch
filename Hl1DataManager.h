/* 
 * File:   Hl1DataManager.h
 * Author: wouter
 *
 * Created on May 27, 2012, 10:19 AM
 */

#ifndef HL1DATAMANAGER_H
#define	HL1DATAMANAGER_H

#include "Hl1BspData.h"
#include "StaticGeometry.h"
#include "SpacePartitioner.h"
#include "visibility.h"

class Tracer;

class Hl1DataManager : public StaticGeometry
{
public:
	Hl1DataManager();
	virtual ~Hl1DataManager();
	
	bool load(const char* filename);
	void uploadLightmaps();
	
	Hl1BspData* bsp;
	Visibility vis;
	
	void setCurrentLeaf(const Vector3& point);
	int getCurrentLeaf() const;
	Tracer* createTracer();
	
	void castShadows(const float sundirection[]);
	
	void renderVisibleFaces();
	void renderAllNodes();
	void renderAllLeafs();
	void renderAllNodesAndLeafs();
	void renderList(const std::set<int>& nodes, const std::set<int>& leafs);
	void renderNode(int index);
	void renderLeaf(int index);
	void renderCurrentLeafWithFaces();
	void renderBoundingBox(const short mins[], const short maxs[]);
	
private:
	bool loadStaticGeometry(Hl1BspData& data);
	
	int currentLeaf;
	
};

#endif	/* HL1DATAMANAGER_H */

