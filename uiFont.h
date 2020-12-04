/*
 * uiFont.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef UIFONT_H_
#define UIFONT_H_

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace ui
{

// This is from imgui, recast navigation code
inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

class Font
{
public:
	Font();
	virtual ~Font();
	
	float size() const { return this->mFontSize; }

	bool initializeFont(const char* fontpath, float size = 15.0f);
	int getTextLength(const char* text, int count = 0);
	int getTextHeight(const char* text);
	void getTextBoungingBox(const char* text, float bb[4]);
	void drawText(float x, float y, const char *text, unsigned int col = ui::RGBA(255, 255, 255, 255));
	void drawTextColorLess(float x, float y, const char *text);
	void getBakedQuad(int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q);

protected:
	stbtt_bakedchar mCharData[96]; // ASCII 32..126 is 95 glyphs
	unsigned int mTextureId;
	float mFontSize;

};

}

#endif /* UIFONT_H_ */
