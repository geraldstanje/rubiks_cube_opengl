#include "sbmptexture.h"
#include <SDL2/SDL.h>

// Defines
#ifndef GL_CLAMP_TO_EDGE
  #define GL_CLAMP_TO_EDGE  0x812F
#endif

#ifndef GL_BGR_EXT
  #define GL_BGR_EXT GL_BGR
#endif

SBMPTexture::SBMPTexture() {
  texture = 0;
}

SBMPTexture::~SBMPTexture() {
  destroy();
}

SBMPTexture::SBMPTexture(const char *filename, int minFilter, int magFilter, bool clampToEdge) {
  createTexture(filename, minFilter, magFilter, clampToEdge); 
}

bool SBMPTexture::createTexture(const char *filename, int minFilter, int magFilter, bool clampToEdge) {
  SDL_Surface *bmpFile;

  destroy();

  if ( (bmpFile = SDL_LoadBMP(filename) ) ) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    if (clampToEdge) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, bmpFile->w, bmpFile->h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmpFile->pixels);

    SDL_FreeSurface(bmpFile);
  
    return true;
  }
  
  return false;
}

bool SBMPTexture::createMipmap(const char *filename, int minFilter, int magFilter, bool clampToEdge) {
  SDL_Surface *bmpFile;

  destroy();
  
  if ( (bmpFile = SDL_LoadBMP(filename) ) ) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    
    if (clampToEdge) {
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    }
  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magFilter);
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmpFile->w, bmpFile->h,GL_BGR_EXT, GL_UNSIGNED_BYTE,bmpFile->pixels);

    SDL_FreeSurface(bmpFile);

    return true;
  }
  
  return false;
}

GLuint SBMPTexture::getGLTexture() {
  return texture;
}

void SBMPTexture::select() {
  glBindTexture(GL_TEXTURE_2D,texture);
}

void SBMPTexture::destroy() {
  if (texture) {
    glDeleteTextures(1, &texture);
    texture = 0;
  }
}