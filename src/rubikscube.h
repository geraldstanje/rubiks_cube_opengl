//
//  rubik.h
//  Rubiks_Cube_OpenGL
//
//  Created by Gerald Stanje on 4/17/14.
//  Copyright (c) 2014 Gerald. All rights reserved.
//

#ifndef Rubiks_Cube_OpenGL_rubikscube_h
#define Rubiks_Cube_OpenGL_rubikscube_h

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "sbmpfont.h"
#include "sbmptexture.h"
using namespace std;

#define USE_TEXTURES

class RubiksCube {
private:
	struct color_container { int colors[6]; };
	GLfloat farbeNach3f[7][3];
    
public:
	int rubik_colors[3][3][3][6]; // Dim1=Spalte 0-2, Dim2=Reihe 0-2, Dim3=Tiefe 0-2, Dim4=Wuerfenseite 0-5
	float rotation[6];
	float rot;
	enum Color { WHITE=0, RED=1, ORANGE=2, YELLOW=3, GREEN=4, BLUE=5, BLACK=6 };
	enum Side { LEFT=0, FRONT=1, RIGHT=2, BACK=3, TOP=4, BOTTOM=5 };
	GLuint cube_faces[6];
	GLuint CoordinateAxis;
    SBMPFont font;
    SBMPTexture texture_white;
    SBMPTexture texture_red;
    SBMPTexture texture_orange;
    SBMPTexture texture_yellow;
    SBMPTexture texture_blue;
    SBMPTexture texture_green;
    SBMPTexture texture_black;
    int	FPS;
    int Frames;
    int LastFPS; // Last FPS Check Time
    float frametime;
    string movements;
    unsigned int movements_done;
    
public:
	RubiksCube();
    
	void make_face(GLuint &displaylist, int color);
	void intialize_surface(const int *surface_color);
	void intialize_face(const int *surface_color, unsigned int side);
    
	void RenderCube(float X, float Y, float Z, float SCALE, color_container color);
    
	void RenderRubikHorizontal(float rotA, float rotB, float rotC);
	void RenderRubikDepth(float rotA, float rotB, float rotC);
	void RenderRubikVertical(float rotA, float rotB, float rotC);
    
	void UpdateDataHorizontal(int row, bool direction);
	void UpdateDataDepth(int column, bool direction);
	void UpdateDataVertical(int depth, bool direction);
    
	void DrawCoordinateAxis();
	void DrawCoordinateSystem();
    
	bool inRotation();
	void HandleKeyboard();
    
    void load_textures();
};

#endif
