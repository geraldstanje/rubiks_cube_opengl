#ifndef S_BMP_TEXTURE_H
#define S_BMP_TEXTURE_H

//#include <gl/glew.h>
#include <GL/glew.h>

class SBMPTexture
{
public:
	void destroy();
	
	/////////////
	// Konstruktoren
	///////////////
	SBMPTexture();
	SBMPTexture(const char* filename,int minFilter = GL_LINEAR,
		int magFilter = GL_LINEAR, bool clampToEdge = false);


        virtual ~SBMPTexture();
	
	/////////////////
	// Funktionen
	////////////////

	virtual bool createMipmap(const char* filename,int minFilter = GL_LINEAR_MIPMAP_NEAREST,
		int magFilter = GL_LINEAR, bool clampToEdge = false);
	virtual bool createTexture(const char* filename, int minFilter = GL_LINEAR,
		int magFilter = GL_LINEAR, bool clampToEdge = false);
	
	void select();

	GLuint getGLTexture();
protected:
	GLuint texture;
};

#endif 
