#include "sbmpfont.h"
#include <string.h>

SBMPFont::SBMPFont() {
  lists = 0;
}

SBMPFont::~SBMPFont() {
  if (lists)
    glDeleteLists(lists,128);
}

void SBMPFont::init(const char *fontFile) {
	texture.createTexture(fontFile);

  // Build font
  float x,y = 0.0f;

  lists = glGenLists(128);
  texture.select();

  for (int i = 0; i < 128; ++i) {
    x = float(i % 16)/16.0f;
		y = float(i / 16)/16.0f;

    glNewList(lists + i,GL_COMPILE);
			glBegin(GL_QUADS);
        glTexCoord2f(x,y+0.0625f);			
				glVertex2i(0,0);
				glTexCoord2f(x+0.0625f,y+0.0625f);	
				glVertex2i(16,0);			
				glTexCoord2f(x+0.0625f,y);		
				glVertex2i(16,16);			
				glTexCoord2f(x,y);			
				glVertex2i(0,16);			
			glEnd();					
			glTranslatef(10.0f,0.0f,0.0f);				
		glEndList();
	}
}

void SBMPFont::printAt(float x, float y, const char* text, bool second) {
  int set = 0;
 
  if (second)
    set = 1;

  // states
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_DEPTH_TEST);						
  texture.select();
  glEnable(GL_TEXTURE_2D);
 
 
  // set screen size
  glMatrixMode(GL_PROJECTION);						
  glPushMatrix();										
  glLoadIdentity();									
  glOrtho(0,640,0,480,-1,1);
 
  // go to modelview
	glMatrixMode(GL_MODELVIEW);							
	glPushMatrix();										
	glLoadIdentity();									
 
  glTranslatef(x, y, 0.0f);								
 
  glColor3f(1,1,1);
 
  // set lisbase
  glListBase(lists - 32 + (128*set) );
  // draw text
	glCallLists(strlen(text),GL_BYTE,text);
 
  // un-do changes
	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();
 
	glMatrixMode(GL_MODELVIEW);							
	glPopMatrix();										
 
  // states
  glPopAttrib();
}
