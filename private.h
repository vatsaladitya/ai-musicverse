#ifndef __PRIVATE_H__
#define __PRIVATE_H__

#ifndef __cplusplus
#error A C++ compiler is needed to process
#endif

#include <GL/gl.h>
#include <IMG/SOIL.h>
#include <string>
#include <iostream>

#include "func.hpp"

#define CBPLAY             0x0000
#define CBPAUSE            0x0001
#define CBSTOP             0x0002
#define CBPLAYLIST         0x0003
#define CBVOLUME           0x0004
#define CBFULLSCREEN       0X0005

typedef struct
{
	int x;
	int y;
	int width;
	int height;
} quad;

HWND globalWindow;
HINSTANCE globalInstance;
MSG globalMsg;

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 25

quad dpb = {BUTTON_WIDTH * 0, 850, BUTTON_WIDTH, BUTTON_HEIGHT};
quad npb = {BUTTON_WIDTH * 1, 850, BUTTON_WIDTH, BUTTON_HEIGHT};

float p2tx(float x);
float p2ty(float y);
float p2cx(float pixel);
float p2cy(float pixel);

/*float p2cx(float pixel)
{
float width = (float) glutGet(GLUT_WINDOW_WIDTH);
float halfw = width / 2.0f;
if(pixel < halfw) return -(1.0f - ((pixel * 1.0f) / halfw));
if(pixel > halfw) return (((pixel - halfw) * 1.0f) / halfw);
}*/

float p2cx(float pixel)
{
float width = (float) glutGet(GLUT_WINDOW_WIDTH);
float halfw = width / 2.0f;
if(pixel < halfw) return -(halfw - pixel);
if(pixel > halfw) return (pixel - halfw);
}

/*float p2cy(float pixel)
{
float height = (float) glutGet(GLUT_WINDOW_HEIGHT);
float halfy = height / 2.0f;
if(pixel > halfy) return -(((pixel - halfy) * 1.0f) / halfy);
if(pixel < halfy) return (1.0f - (pixel* 1.0f) / halfy);
}*/

float p2cy(float pixel)
{
float height = (float) glutGet(GLUT_WINDOW_HEIGHT);
float halfy = height / 2.0f;
if(pixel > halfy) return -(pixel - halfy);
if(pixel < halfy) return (halfy - pixel);
}

// 1000:1::500:?
float p2tx(float x)
{
	return (x / 1000);
}

//875:1::500:?
float p2ty(float y)
{
	return (1 - (y / 875));
}

void aiVertex2f(float x, float y)
{
	glVertex2f(p2cx(x), p2cy(y));
}

void aiTexCoord2f(float x, float y)
{
	glTexCoord2f(p2tx(x), p2ty(y));
}

void button(HWND hwnd, int mode, std::string text, int x, int y, int width, int height)
{
	if(mode == 0)
	{
	glBegin(GL_QUADS);
		aiTexCoord2f(npb.x             , npb.y);	          aiVertex2f(x         , y);
		aiTexCoord2f(npb.x + npb.width , npb.y);	          aiVertex2f(x + width , y);
		aiTexCoord2f(npb.x + npb.width , npb.y + npb.height); aiVertex2f(x + width , y + height);
		aiTexCoord2f(npb.x             , npb.y + npb.height); aiVertex2f(x         , y + height);
	glEnd();
	}

	if(mode == 1)
	{
	glBegin(GL_QUADS);
		aiTexCoord2f(dpb.x             , dpb.y);	          aiVertex2f(x         , y);
		aiTexCoord2f(dpb.x + dpb.width , dpb.y);	          aiVertex2f(x + width , y);
		aiTexCoord2f(dpb.x + dpb.width , dpb.y + dpb.height); aiVertex2f(x + width , y + height);
		aiTexCoord2f(dpb.x             , dpb.y + dpb.height); aiVertex2f(x         , y + height);
	glEnd();
	}

}

void ctrlButton(int button, int x, int y)
{
	switch(button)
	{
		case CBPLAY:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(0 , 800); aiVertex2f(x     , y);
				aiTexCoord2f(50, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(50, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(0 , 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
		case CBPAUSE:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(50 , 800); aiVertex2f(x     , y);
				aiTexCoord2f(100, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(100, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(50 , 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
		case CBSTOP:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(100, 800); aiVertex2f(x     , y);
				aiTexCoord2f(150, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(150, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(100, 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
		case CBPLAYLIST:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(150, 800); aiVertex2f(x     , y);
				aiTexCoord2f(200, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(200, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(150, 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
		case CBVOLUME:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(200, 800); aiVertex2f(x     , y);
				aiTexCoord2f(250, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(250, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(200, 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
		case CBFULLSCREEN:
			{
			glBegin(GL_QUADS);
				aiTexCoord2f(250, 800); aiVertex2f(x     , y);
				aiTexCoord2f(300, 800); aiVertex2f(x + 50, y);
				aiTexCoord2f(300, 850); aiVertex2f(x + 50, y + 50);
				aiTexCoord2f(250, 850); aiVertex2f(x     , y + 50);
			glEnd();
			break;
			}
	}
}

void highlightCtrlButton(int x, int y)
{
    float r, g, b;
    glBegin(GL_QUADS);
        if(light_mode)
            {r = 0.0f; g = 0.0f; b = 0.8f;}
		else
            {r = 0.3f; g = 0.0f; b = 0.8f;}

        glColor4f(r, g, b, 0.0f); aiVertex2f(x     , y);
        glColor4f(r, g, b, 0.0f); aiVertex2f(x + 50, y);
        glColor4f(r, g, b, 0.5f); aiVertex2f(x + 50, y + 50);
        glColor4f(r, g, b, 0.5f); aiVertex2f(x     , y + 50);
    glEnd();
}

#endif
