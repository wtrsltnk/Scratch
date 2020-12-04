/* 
 * File:   Shader.h
 * Author: wouter
 *
 * Created on July 8, 2012, 5:01 PM
 */

#ifndef SHADER_H
#define	SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "vector3.h"

using namespace std;

class Shader
{
	int vs;
	int fs;
	int  program;
	Shader();

public:
	static Shader* createShader(const string& vsFile, const string& fsFile);
	
	virtual ~Shader();
	static int compileShader(const string& file, unsigned int type);
	void use();

	unsigned int getUniform(const char* name);
	void setUniformVector(unsigned int uniform, const Vector3& vec);

	static void useNoShader();
};

#endif	/* SHADER_H */

