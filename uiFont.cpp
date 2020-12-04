/*
 * uiFont.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#include "uiFont.h"
#include <glad/glad.h>
#include <stdio.h>

namespace ui
{
	
	typedef struct sVertex
	{
		float position[3];
		float texCoords[2];
		
	} tVertex;

Font::Font()
	: mTextureId(0)
{
}

Font::~Font()
{
}

bool Font::initializeFont(const char* fontpath, float fontSize)
{
	this->mFontSize = fontSize;
	
    // Load font.
    FILE* fp = fopen(fontpath, "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char* ttfBuffer = (unsigned char*)malloc(size);
    if (!ttfBuffer)
    {
		fclose(fp);
		return false;
    }

    fread(ttfBuffer, 1, size, fp);
    fclose(fp);
    fp = 0;

    unsigned char* bmap = (unsigned char*)malloc(512*512);
    if (!bmap)
    {
		free(ttfBuffer);
		return false;
    }

    stbtt_BakeFontBitmap(ttfBuffer,0, fontSize, bmap, 512, 512, 32, 96, this->mCharData);

    // can free ttf_buffer at this point
    glGenTextures(1, &this->mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(ttfBuffer);
    free(bmap);

    return true;
}

static const float g_tabStops[4] = {150, 210, 270, 330};

int Font::getTextLength(const char* text, int count)
{
	float xpos = 0;
	float len = 0;
	if (count <= 0) count = strlen(text);
	while (*text && count--)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (xpos < g_tabStops[i])
				{
					xpos = g_tabStops[i];
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = this->mCharData + c-32;
			int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		++text;
	}
	return len;
}

int Font::getTextHeight(const char* text)
{
	float height0 = 0;
	while (*text)
	{
		int c = (unsigned char)*text;
		if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = this->mCharData + c-32;
			if ((b->y1-b->y0) > height0) height0 = (b->y1-b->y0);
		}
		++text;
	}
	return height0;
}

void Font::getTextBoungingBox(const char* text, float bb[4])
{
	if (!text) return;

	float x = 0, y = 0;
	const float ox = x;

	while (*text)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (x < g_tabStops[i]+ox)
				{
					x = g_tabStops[i]+ox;
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			stbtt_aligned_quad q;
			this->getBakedQuad(512,512, c-32, &x,&y,&q);

			// min x
			if (bb[0] > q.x0) bb[0] = q.x0;
			if (bb[0] > q.x1) bb[0] = q.x1;

			// min y
			if (bb[1] > q.y0) bb[1] = q.y0;
			if (bb[1] > q.y1) bb[1] = q.y1;

			// max x
			if (bb[2] < q.x0) bb[2] = q.x0;
			if (bb[2] < q.x1) bb[2] = q.x1;

			// max x
			if (bb[3] < q.y0) bb[3] = q.y0;
			if (bb[3] < q.y1) bb[3] = q.y1;
		}
		++text;
	}	
}

void Font::drawText(float x, float y, const char *text, unsigned int col)
{
        if (this->mTextureId == 0) return;
        if (!text) return;

        glFrontFace(GL_CW);
        glBindTexture(GL_TEXTURE_2D, this->mTextureId);

        glBegin(GL_TRIANGLES);

        glColor4ub(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff);

        const float ox = x;

        while (*text)
        {
			int c = (unsigned char)*text;
			if (c == '\t')
			{
				for (int i = 0; i < 4; ++i)
				{
					if (x < g_tabStops[i]+ox)
					{
						x = g_tabStops[i]+ox;
						break;
					}
				}
			}
			else if (c >= 32 && c < 128)
			{
				stbtt_aligned_quad q;
				this->getBakedQuad(512,512, c-32, &x,&y,&q);

				glTexCoord2f(q.s0, q.t0);
				glVertex2f(q.x0, q.y0);
				glTexCoord2f(q.s1, q.t1);
				glVertex2f(q.x1, q.y1);
				glTexCoord2f(q.s1, q.t0);
				glVertex2f(q.x1, q.y0);

				glTexCoord2f(q.s0, q.t0);
				glVertex2f(q.x0, q.y0);
				glTexCoord2f(q.s0, q.t1);
				glVertex2f(q.x0, q.y1);
				glTexCoord2f(q.s1, q.t1);
				glVertex2f(q.x1, q.y1);
			}
			++text;
        }

        glEnd();
        glFrontFace(GL_CCW);
}

void Font::drawTextColorLess(float x, float y, const char *text)
{
        if (this->mTextureId == 0) return;
        if (!text) return;

        // assume orthographic projection with units = screen pixels, origin at top left
        glBindTexture(GL_TEXTURE_2D, this->mTextureId);

        glBegin(GL_TRIANGLES);

        const float ox = x;

        while (*text)
        {
			int c = (unsigned char)*text;
			if (c == '\t')
			{
				for (int i = 0; i < 4; ++i)
				{
					if (x < g_tabStops[i]+ox)
					{
						x = g_tabStops[i]+ox;
						break;
					}
				}
			}
			else if (c >= 32 && c < 128)
			{
				stbtt_aligned_quad q;
				this->getBakedQuad(512,512, c-32, &x,&y,&q);

				glTexCoord2f(q.s0, q.t0);
				glVertex2f(q.x0, q.y0);
				glTexCoord2f(q.s1, q.t1);
				glVertex2f(q.x1, q.y1);
				glTexCoord2f(q.s1, q.t0);
				glVertex2f(q.x1, q.y0);

				glTexCoord2f(q.s0, q.t0);
				glVertex2f(q.x0, q.y0);
				glTexCoord2f(q.s0, q.t1);
				glVertex2f(q.x0, q.y1);
				glTexCoord2f(q.s1, q.t1);
				glVertex2f(q.x1, q.y1);
			}
			++text;
        }

        glEnd();
}

void Font::getBakedQuad(int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q)
{
	stbtt_bakedchar *b = this->mCharData + char_index;
	int round_x = STBTT_ifloor(*xpos + b->xoff);
	int round_y = STBTT_ifloor(*ypos - b->yoff);

	q->x0 = (float)round_x;
	q->y0 = (float)round_y;
	q->x1 = (float)round_x + b->x1 - b->x0;
	q->y1 = (float)round_y - b->y1 + b->y0;

	q->s0 = b->x0 / (float)pw;
	q->t0 = b->y0 / (float)pw;
	q->s1 = b->x1 / (float)ph;
	q->t1 = b->y1 / (float)ph;

	*xpos += b->xadvance;
}

}
