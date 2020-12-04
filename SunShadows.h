/* 
 * File:   SunShadows.h
 * Author: wouter
 *
 * Created on July 2, 2012, 9:01 PM
 */

#ifndef SUNSHADOWS_H
#define	SUNSHADOWS_H

#include <vector>
#include "Hl1DataManager.h"
#include "StaticGeometry.h"
#include "vector3.h"

class SunShadows
{
public:
	class Caster
	{
	public:
		Caster() { }
		virtual ~Caster() { }
		
		int count;
		StaticGeometry::tVertex* vertices;
	};
public:
	SunShadows(Hl1DataManager* data, const Vector3& vec);
	virtual ~SunShadows();
	
	void render(int i);
	
private:
	Hl1DataManager* data;
	std::vector<Caster*> casters;

};

#endif	/* SUNSHADOWS_H */

