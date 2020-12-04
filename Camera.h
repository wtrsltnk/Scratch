/* 
 * File:   Camera.h
 * Author: wouter
 *
 * Created on May 27, 2012, 3:17 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "vector3.h"

class Camera
{
public:
	Camera();
	Camera(const Camera& orig);
	virtual ~Camera();
	
	void update();
	
	Vector3 position;
	
	void moveForward(float amount);
	void moveLeft(float amount);
	void rotateDegree(float x, float y);
private:
	float matrix[16];
	float rotation[2];

};

#endif	/* CAMERA_H */

