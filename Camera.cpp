/* 
 * File:   Camera.cpp
 * Author: wouter
 * 
 * Created on May 27, 2012, 3:17 PM
 */

#include "Camera.h"
#include <glad/glad.h>
#include "vector3.h"
#include <iostream>

using namespace std;

Camera::Camera()
{
	this->rotation[0] = -70;
	this->rotation[1] = -132;
}

Camera::Camera(const Camera& orig)
{
}

Camera::~Camera()
{
}

void Camera::update()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(this->rotation[0], 1, 0, 0);
	glRotatef(this->rotation[1], 0, 0, 1);
	glTranslatef(-this->position.x(), -this->position.y(), -this->position.z());
	
	glGetFloatv(GL_MODELVIEW_MATRIX, this->matrix);
}

void Camera::moveForward(float amount)
{
	Vector3 forward(this->matrix[2], this->matrix[6], this->matrix[10]);
	this->position += (forward * amount);
//	cout << this->position << endl;
}

void Camera::moveLeft(float amount)
{
	Vector3 left(this->matrix[0], this->matrix[4], this->matrix[8]);
	this->position += (left * amount);
//	cout << this->position << endl;
}

void Camera::rotateDegree(float x, float y)
{
	this->rotation[0] += x;
	this->rotation[1] += y;
//	cout << this->rotation[0] << " " << this->rotation[1] << endl;
}
