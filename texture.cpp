/* 
 * File:   Texture.cpp
 * Author: wouter
 * 
 * Created on August 1, 2010, 11:42 PM
 */

#include "texture.h"
#include <glad/glad.h>
#include <string.h>

int Texture::sIndices = 1;

Texture::Texture()
	: mData(0), mWidth(-1), mHeight(-1), mBpp(-1), mGlindex(0)
{
}

Texture::Texture(int w, int h, int b)
	: mData(0), mWidth(w), mHeight(h), mBpp(b), mGlindex(0)
{
	int datasize = w * h * b;
	this->mData = new unsigned char[datasize];
	memset(this->mData, 255, sizeof(unsigned char) * datasize);
}

Texture::Texture(int w, int h, int b, unsigned char* d)
	: mData(0), mWidth(w), mHeight(h), mBpp(b), mGlindex(0)
{
	int datasize = w * h * b;
	this->mData = new unsigned char[datasize];
	memcpy(this->mData, d, sizeof(unsigned char) * datasize);
}

Texture::~Texture()
{
	if (this->mData != 0)
		delete []this->mData;

	if (this->mGlindex != 0)
		glDeleteTextures(1, &this->mGlindex);
}

void Texture::setName(const char* name)
{
	for (int i = 0; i < 32; i++)
		this->mName[i] = name[i];
}

void Texture::setData(int w, int h, int b, unsigned char* d)
{
	this->mWidth = w;
	this->mHeight = h;
	this->mBpp = b;

	int datasize = w * h * b;
	this->mData = new unsigned char[datasize];
	if (d != 0)
		memcpy(this->mData, d, sizeof(unsigned char) * datasize);
	else
		memset(this->mData, 255, sizeof(unsigned char) * datasize);
}

unsigned int Texture::upload()
{
	int format = (this->mBpp == 4 ? GL_RGBA : GL_RGB);

	glGenTextures(1, &this->mGlindex);
	glBindTexture(GL_TEXTURE_2D, this->mGlindex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, this->mWidth, this->mHeight, 0, format, GL_UNSIGNED_BYTE, this->mData);

	delete []this->mData;
	this->mData = 0;
	
	return this->mGlindex;
}

void Texture::download()
{
	int format = GL_RGB;

	// Determine format
	switch (this->mBpp)
	{
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
	}

	glBindTexture(GL_TEXTURE_2D, this->mGlindex);

	if (this->mData == 0)
		this->mData = new unsigned char[this->mWidth * this->mHeight * this->mBpp];

	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)this->mData);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, this->mGlindex);
}

const char* Texture::name() const
{
	return this->mName;
}

unsigned char* Texture::data() const
{
	return this->mData;
}

int Texture::width() const
{
	return this->mWidth;
}

int Texture::height() const
{
	return this->mHeight;
}

int Texture::bpp() const
{
	return this->mBpp;
}

unsigned int Texture::glindex() const
{
	return this->mGlindex;
}
