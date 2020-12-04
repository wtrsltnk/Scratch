/* 
 * File:   Texture.h
 * Author: wouter
 *
 * Created on August 1, 2010, 11:42 PM
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

class Texture
{
public:
	Texture();
	Texture(int w, int h, int b);
	Texture(int w, int h, int b, unsigned char* d);
	virtual ~Texture();

	void setName(const char* name);
	void setData(int w, int h, int b, unsigned char* d = 0);
	unsigned int upload();
	void download();
	void use();

	const char* name() const;
	unsigned char* data() const;
	int width() const;
	int height() const;
	int bpp() const;
	unsigned int glindex() const;

private:
	char mName[32];
	unsigned char* mData;
	int mWidth;
	int mHeight;
	int mBpp;
	unsigned int mGlindex;

private:
	static int sIndices;

};

#endif	/* TEXTURE_H */

