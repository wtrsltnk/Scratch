/* 
 * File:   Tracer.h
 * Author: wouter
 *
 * Created on May 27, 2012, 4:49 PM
 */

#ifndef TRACER_H
#define	TRACER_H

#include "Hl1DataManager.h"
#include <vector>

typedef struct 
{
	Vector3 intersection;
	std::vector<Vector3> face;
} Hit;

class Tracer
{
public:
	class Result
	{
	public:
		float fraction;
		std::vector<Vector3> points;
		std::vector<Vector3> intersections;
		std::vector<Hit> faceIntersections;
		unsigned short face;
		
		void render();
		
	} result;
public:
	Tracer(Hl1DataManager* d);
	virtual ~Tracer();
	
	Result trace(const Vector3& start, const Vector3& end);
	
private:
	void checkNode(int nodeIndex, const Vector3& start, const Vector3& end);
	Hl1DataManager* data;

};

#endif	/* TRACER_H */

