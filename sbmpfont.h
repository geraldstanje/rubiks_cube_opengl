#ifndef SBMPFONT_H
#define SBMPFONT_H

#include "sbmptexture.h"

class SBMPFont
{
public: 
	SBMPFont();
	~SBMPFont();
        
        void init(const char* fontFile);

        void printAt(float x, float y, const char* text, bool second=false);
protected:
        SBMPTexture texture;
        GLuint lists;
};

#endif
