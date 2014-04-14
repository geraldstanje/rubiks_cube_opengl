#include <sstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "sbmpfont.h"
#include "sbmptexture.h"

using namespace std;

#define USE_TEXTURES

// Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

enum Key { NO_GAME_KEY = 0, KEY_ROT_UP_CUBE = 1, KEY_ROT_DOWN_CUBE = 2 };
bool key_map[2];
GLfloat	rquad1;
GLfloat	rquad2;

float rot[3] = {40.0f, 45.0f, 0.0f };
float oldMousePos_x = 0.0f;
float oldMousePos_y = 0.0f;
bool bLButtonDown = false;

int	FPS = 0, Frames = 0, LastFPS = 0; // Last FPS Check Time
string movements;
unsigned int movements_done = 0;
float frametime = 0.0f;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambiente Lichtwere
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Diffuse Lichtwerte
GLfloat LightSpecular[]= { 0.0f, 0.0f, 0.0f, 0.0f }; // Diffuse Lichtwerte
GLfloat LightEmission[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, -1.0f, 1.0f }; // Lichtposition
SBMPFont font;
SBMPTexture texture_white;
SBMPTexture texture_red;
SBMPTexture texture_orange;
SBMPTexture texture_yellow;
SBMPTexture texture_blue;
SBMPTexture texture_green;
SBMPTexture texture_black;
bool scan_colors = false;
bool solve_rubik = false;


class RubikCube {
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

public:
	RubikCube() {
		 rot = 0.0f; // Make the RGB (3 floats) a class. Give it a member "Set" or "SetRGB" taking 3 floats. "farbeNach[0].Set(1,1,1); // white"
		 CoordinateAxis = 0;
		 farbeNach3f[0][0] = 1.0f;  // White
		 farbeNach3f[0][1] = 1.0f;
		 farbeNach3f[0][2] = 1.0f;

		 farbeNach3f[1][0] = 1.0f;  // Red
		 farbeNach3f[1][1] = 0.0f;
		 farbeNach3f[1][2] = 0.0f;

		 farbeNach3f[2][0] = 1.0f;  // Orange   
		 farbeNach3f[2][1] = 0.7f;//0.5f;
		 farbeNach3f[2][2] = 0.2f;//0.0f;

		 farbeNach3f[3][0] = 1.0f;  // Yellow
		 farbeNach3f[3][1] = 1.0f;
		 farbeNach3f[3][2] = 0.0f;

		 farbeNach3f[4][0] = 0.0938f;  // Green
		 farbeNach3f[4][1] = 0.7344f;
		 farbeNach3f[4][2] = 0.36328f;

		 farbeNach3f[5][0] = 0.0f;  // Blue
		 farbeNach3f[5][1] = 0.0f;
		 farbeNach3f[5][2] = 1.0f;

		 farbeNach3f[6][0] = 0.0f;  // Black
		 farbeNach3f[6][1] = 0.0f;
		 farbeNach3f[6][2] = 0.0f;

		 // no rotation at begin
		 for (int y=0;y<6;y++) rotation[y] = 0.0f;
	}
    
	void make_face(GLuint &displaylist, int color) {
		displaylist = glGenLists(1);
  
		glNewList(displaylist, GL_COMPILE);

        #ifdef USE_TEXTURES
			// Flaeche liegt in der x/z ebene
			glBegin(GL_QUADS);
			glColor3fv(farbeNach3f[color]);
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.9f, 1.0f,-0.9f);					// Top Right Of The Quad (Top)
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, 1.0f,-0.9f);					// Top Left Of The Quad (Top)
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.9f, 1.0f, 0.9f);					// Bottom Left Of The Quad (Top)
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.9f, 1.0f, 0.9f);					// Bottom Right Of The Quad (Top)
			glEnd();	

			texture_black.select();

			// draw chamfered edges
			glTranslatef(-0.95f,0.0f,0.0f);

			glColor3fv(farbeNach3f[BLACK]);
			glBegin(GL_QUADS);
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.05f, 1.0f,-0.9f);					// Top Right Of The Quad (Top)
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, 1.0f,-0.9f);					// Top Left Of The Quad (Top)
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.05f, 1.0f, 0.9f);					// Bottom Left Of The Quad (Top)
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f, 1.0f, 0.9f);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(1.9f,0.0f,0.0f); 

			glColor3fv(farbeNach3f[BLACK]);
			glBegin(GL_QUADS);
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.05f, 1.0f,-0.9f);					// Top Right Of The Quad (Top)
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.05f, 1.0f,-0.9f);					// Top Left Of The Quad (Top)
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.05f, 1.0f, 0.9f);					// Bottom Left Of The Quad (Top)
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f, 1.0f, 0.9f);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(-0.95f,0.0f,-0.95f); 

			glColor3fv(farbeNach3f[BLACK]);
			glBegin(GL_QUADS);
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-0.05f);					// Top Right Of The Quad (Top)
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-0.05f);					// Top Left Of The Quad (Top)
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.05f);					// Bottom Left Of The Quad (Top)
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.05f);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(0.0f,0.0f,1.90f);

			glColor3fv(farbeNach3f[BLACK]);
			glBegin(GL_QUADS);
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, 1.0f,-0.05f);					// Top Right Of The Quad (Top)
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-0.05f);					// Top Left Of The Quad (Top)
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.05f);					// Bottom Left Of The Quad (Top)
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.05f);					// Bottom Right Of The Quad (Top)
			glEnd();

        #else

			glBegin(GL_QUADS);
			glColor3fv(farbeNach3f[color]);
				glVertex3f( 0.8f, 1.0f,-0.8f);					// Top Right Of The Quad (Top)
				glVertex3f(-0.8f, 1.0f,-0.8f);					// Top Left Of The Quad (Top)
				glVertex3f(-0.8f, 1.0f, 0.8f);					// Bottom Left Of The Quad (Top)
				glVertex3f( 0.8f, 1.0f, 0.8f);					// Bottom Right Of The Quad (Top)
			glEnd();	


			// draw chamfered edges
			glTranslatef(-0.9f,0.0f,0.0f); 

			glColor3fv(farbeNach3f[Color::BLACK]);
			glBegin(GL_QUADS);
				glVertex3f( 0.1f, 1.0f,-0.8);					// Top Right Of The Quad (Top)
				glVertex3f(-0.1f, 1.0f,-0.8);					// Top Left Of The Quad (Top)
				glVertex3f(-0.1f, 1.0f, 0.8);					// Bottom Left Of The Quad (Top)
				glVertex3f( 0.1f, 1.0f, 0.8);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(1.8f,0.0f,0.0f); 

			glColor3fv(farbeNach3f[Color::BLACK]);
			glBegin(GL_QUADS);
				glVertex3f( 0.1f, 1.0f,-0.8);					// Top Right Of The Quad (Top)
				glVertex3f(-0.1f, 1.0f,-0.8);					// Top Left Of The Quad (Top)
				glVertex3f(-0.1f, 1.0f, 0.8);					// Bottom Left Of The Quad (Top)
				glVertex3f( 0.1f, 1.0f, 0.8);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(-0.9f,0.0f,-0.9f); 

			glColor3fv(farbeNach3f[Color::BLACK]);
			glBegin(GL_QUADS);
				glVertex3f( 1.0f, 1.0f,-0.1);					// Top Right Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f,-0.1);					// Top Left Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f, 0.1);					// Bottom Left Of The Quad (Top)
				glVertex3f( 1.0f, 1.0f, 0.1);					// Bottom Right Of The Quad (Top)
			glEnd();

			glTranslatef(0.0f,0.0f,1.8f);

			glColor3fv(farbeNach3f[Color::BLACK]);
			glBegin(GL_QUADS);
				glVertex3f( 1.0f, 1.0f,-0.1);					// Top Right Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f,-0.1);					// Top Left Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f, 0.1);					// Bottom Left Of The Quad (Top)
				glVertex3f( 1.0f, 1.0f, 0.1);					// Bottom Right Of The Quad (Top)
			glEnd();
        #endif
 
		glEndList();
	}
    
	void intialize_surface(const int *surface_color) {
		int a = 0;

		// Left Surface
		for (int y=0;y<3;y++) for (int z=0;z<3;z++){
			rubik_colors[0][y][z][LEFT] = surface_color[a++];
		}

		// Front Surface
		for (int x=0;x<3;x++) for (int y=0;y<3;y++){
			rubik_colors[x][y][2][FRONT] = surface_color[a++];
		}

		// Right Surface
		for (int y=0;y<3;y++) for (int z=0;z<3;z++){
			rubik_colors[2][y][z][RIGHT] = surface_color[a++];
		}

		// Back Surface
		for (int x=0;x<3;x++) for (int y=0;y<3;y++){
			rubik_colors[x][y][0][BACK] = surface_color[a++];
		}

		// Top Surface
		for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			rubik_colors[x][2][z][TOP] = surface_color[a++];
		}

		// Bottom Surface
		for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			rubik_colors[x][0][z][BOTTOM] = surface_color[a++];
		}
	}
    
	void intialize_face(const int *surface_color, unsigned int side) {
		int a = 0;
        
		if(side == LEFT) {
			rubik_colors[0][0][0][LEFT] = surface_color[a++];
			rubik_colors[0][1][0][LEFT] = surface_color[a++];
			rubik_colors[0][2][0][LEFT] = surface_color[a++];
			rubik_colors[0][0][1][LEFT] = surface_color[a++];
			rubik_colors[0][1][1][LEFT] = surface_color[a++];
			rubik_colors[0][2][1][LEFT] = surface_color[a++];
			rubik_colors[0][0][2][LEFT] = surface_color[a++];
			rubik_colors[0][1][2][LEFT] = surface_color[a++];
			rubik_colors[0][2][2][LEFT] = surface_color[a++];

			// Left Surface
			//for (int y=0;y<3;y++) for (int z=0;z<3;z++){
			//	rubik_colors[0][y][z][LEFT] = surface_color[a++];
			//}
		}
		else if(side == FRONT) {		
			// Front Surface
			//for (int x=0;x<3;x++) for (int y=0;y<3;y++){
			//	rubik_colors[x][y][2][FRONT] = surface_color[a++];
			//}
			rubik_colors[0][2][2][FRONT] = surface_color[a++];
			rubik_colors[1][2][2][FRONT] = surface_color[a++];
			rubik_colors[2][2][2][FRONT] = surface_color[a++];
			rubik_colors[0][1][2][FRONT] = surface_color[a++];
			rubik_colors[1][1][2][FRONT] = surface_color[a++];
			rubik_colors[2][1][2][FRONT] = surface_color[a++];
			rubik_colors[0][0][2][FRONT] = surface_color[a++];
			rubik_colors[1][0][2][FRONT] = surface_color[a++];
			rubik_colors[2][0][2][FRONT] = surface_color[a++];
		}
		else if(side == RIGHT) {
			// Right Surface
			//for (int y=0;y<3;y++) for (int z=0;z<3;z++){
			//	rubik_colors[2][y][z][RIGHT] = surface_color[a++];
			//}
			rubik_colors[2][0][2][RIGHT] = surface_color[a++];
			rubik_colors[2][1][2][RIGHT] = surface_color[a++];
			rubik_colors[2][2][2][RIGHT] = surface_color[a++];
			rubik_colors[2][0][1][RIGHT] = surface_color[a++];
			rubik_colors[2][1][1][RIGHT] = surface_color[a++];
			rubik_colors[2][2][1][RIGHT] = surface_color[a++];
			rubik_colors[2][0][0][RIGHT] = surface_color[a++];
			rubik_colors[2][1][0][RIGHT] = surface_color[a++];
			rubik_colors[2][2][0][RIGHT] = surface_color[a++];
		}
		else if(side == BACK) {
			// Back Surface
			//for (int x=0;x<3;x++) for (int y=0;y<3;y++){
			//	rubik_colors[x][y][0][BACK] = surface_color[a++];
			//}
			rubik_colors[0][0][0][BACK] = surface_color[a++];
			rubik_colors[1][0][0][BACK] = surface_color[a++];
			rubik_colors[2][0][0][BACK] = surface_color[a++];
			rubik_colors[0][1][0][BACK] = surface_color[a++];
			rubik_colors[1][1][0][BACK] = surface_color[a++];
			rubik_colors[2][1][0][BACK] = surface_color[a++];
			rubik_colors[0][2][0][BACK] = surface_color[a++];
			rubik_colors[1][2][0][BACK] = surface_color[a++];
			rubik_colors[2][2][0][BACK] = surface_color[a++];
		}
		else if(side == TOP) {
			// Top Surface
			//for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			//	rubik_colors[x][2][z][TOP] = surface_color[a++];
			//}
			rubik_colors[0][2][0][TOP] = surface_color[a++];
			rubik_colors[1][2][0][TOP] = surface_color[a++];
			rubik_colors[2][2][0][TOP] = surface_color[a++];
			rubik_colors[0][2][1][TOP] = surface_color[a++];
			rubik_colors[1][2][1][TOP] = surface_color[a++];
			rubik_colors[2][2][1][TOP] = surface_color[a++];
			rubik_colors[0][2][2][TOP] = surface_color[a++];
			rubik_colors[1][2][2][TOP] = surface_color[a++];
			rubik_colors[2][2][2][TOP] = surface_color[a++];
		}
		else if(side == BOTTOM) {
			// Bottom Surface
			//for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			//	rubik_colors[x][0][z][BOTTOM] = surface_color[a++];
			//}
			rubik_colors[0][0][2][BOTTOM] = surface_color[a++];
			rubik_colors[1][0][2][BOTTOM] = surface_color[a++];
			rubik_colors[2][0][2][BOTTOM] = surface_color[a++];
			rubik_colors[0][0][1][BOTTOM] = surface_color[a++];
			rubik_colors[1][0][1][BOTTOM] = surface_color[a++];
			rubik_colors[2][0][1][BOTTOM] = surface_color[a++];
			rubik_colors[0][0][0][BOTTOM] = surface_color[a++];
			rubik_colors[1][0][0][BOTTOM] = surface_color[a++];
			rubik_colors[2][0][0][BOTTOM] = surface_color[a++];
		}
	}
    
	void RenderCube(float X, float Y, float Z, float SCALE, color_container color) {
		glPushMatrix(); 
		glTranslatef(X,Y,Z); 
		glScalef(SCALE,SCALE,SCALE);
		for (int i=0;i<6;i++) {
			glPushMatrix();
			switch(i)  
			{ 
				case LEFT: glRotatef(90,0,0,1);
				break; 
				case FRONT: glRotatef(90,1,0,0);
				break;
				case RIGHT: glRotatef(-90,0,0,1); 
				break;
				case BACK: glRotatef(-90,1,0,0); 
				break;
				case TOP: //glRotatef(90,0,0,1); // do nothing;
				break;
				case BOTTOM: glRotatef(180,1,0,0);
				break;
			}

			if(color.colors[i] == WHITE) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);					
				texture_white.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == RED) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);				
				texture_red.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == ORANGE) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);				
				texture_orange.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == YELLOW) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);						
				texture_yellow.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == BLUE) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);					
				texture_blue.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == GREEN) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);					
				texture_green.select();
				glEnable(GL_TEXTURE_2D);
			}
			else if(color.colors[i] == BLACK) {
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);					
				texture_black.select();
				glEnable(GL_TEXTURE_2D);
			}

			glCallList(cube_faces[color.colors[i]]);

			glPopAttrib();
			
			glPopMatrix();
		}

		glPopMatrix();
	}
    
	void RenderRubikHorizontal(float rotA, float rotB, float rotC) {

		for (int y=0;y<3;y++)  
		{
			glPushMatrix(); 
 
			glTranslatef(-1.0,-1.0,-1.0);
			if (y==0) glRotatef(rotA,0.0f,1.0f,0.0f); 
            else if (y==2) glRotatef(rotC,0.0f,1.0f,0.0f);
			glTranslatef(1.0,1.0,1.0);
 
            for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
				color_container col;
                memcpy(&col.colors,&rubik_colors[x][y][z],sizeof(int) * 6);
                RenderCube(static_cast<float>(-3+(x*2)),static_cast<float>(-3+(y*2)),static_cast<float>(-3+(z*2)),1.0f,col);
            }
 
            glPopMatrix();
        }
    }
    
	void RenderRubikDepth(float rotA,float rotB,float rotC) {

		for (int x = 0; x < 3; x++) {
			glPushMatrix(); 
 
			glTranslatef(-1.0,-1.0,-1.0);
			if (x==0) glRotatef(rotA,1.0f,0.0f,0.0f); 
            else if (x==2) glRotatef(rotC,1.0f,0.0f,0.0f);
			glTranslatef(1.0,1.0,1.0);
 
            for (int y=0;y<3;y++) for (int z=0;z<3;z++) {
				color_container col;
                memcpy(&col.colors,&rubik_colors[x][y][z],sizeof(int) * 6);
                RenderCube(static_cast<float>(-3+(x*2)),static_cast<float>(-3+(y*2)),static_cast<float>(-3+(z*2)),1.0f,col);
            }
 
            glPopMatrix();
        }
	}
    
	void RenderRubikVertical(float rotA,float rotB,float rotC) {
		for (int z = 0; z < 3; z++) {
			glPushMatrix(); 
 
			glTranslatef(-1.0,-1.0,-1.0);
			if (z==0) glRotatef(rotA,0.0f,0.0f,1.0f); 
            else if (z==2) glRotatef(rotC,0.0f,0.0f,1.0f);
			glTranslatef(1.0,1.0,1.0);
 
            for (int x=0;x<3;x++) for (int y=0;y<3;y++) {
				color_container col;
                memcpy(&col.colors,&rubik_colors[x][y][z],sizeof(int) * 6);
                RenderCube(static_cast<float>(-3+(x*2)),static_cast<float>(-3+(y*2)),static_cast<float>(-3+(z*2)),1.0f,col);
            }
 
            glPopMatrix();
        }
	}
	void UpdateDataHorizontal(int row, bool direction) {
		int rubik_colors_help[3][3][3][6];
		int face_array_rotation_l[] = { 3, 0, 1, 2, 4, 5 };
		int face_array_rotation_r[] = { 1, 2, 3, 0, 4, 5 };

		int u = 2;
		for(int depth=0;depth<3;depth++) { // Schleife von Tiefe0 - Tiefe2
			for (int column=0;column<3;column++) { // Schleife von Spalte0 - Spalte2
                for(int i=0;i<6;i++) {
                    if(direction) // Spalte0 -> Tiefe0
                        rubik_colors_help[column][row][u][i] = rubik_colors[depth][row][column][face_array_rotation_l[i]];
                    else // Tiefe0  -> Spalte0
                        rubik_colors_help[u][row][column][i] = rubik_colors[column][row][depth][face_array_rotation_r[i]];
                }
			}
			u--;
		}

		for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			memcpy(&rubik_colors[x][row][z],&rubik_colors_help[x][row][z],sizeof(int) *6);
		}
	}
	void UpdateDataDepth(int column, bool direction) {
		int rubik_colors_help[3][3][3][6];
		int face_array_rotation_l[] = { 0, 5, 2, 4, 1, 3 };
		int face_array_rotation_r[] = { 0, 4, 2, 5, 3, 1 };

		for(int x = 0; x < 3; x++) { // Schleife von Reihe0 - Reihe2
			int u = 0;
            
			for (int y = 2; y >= 0; y--) { // Schleife von Spalte0 - Spalte2
                for(int i=0; i < 6; i++) {
                    if(direction) // Spalte0 -> Reihe0
                        rubik_colors_help[column][y][x][i] = rubik_colors[column][x][u][face_array_rotation_r[i]];
                    else // Reihe0 -> Spalte0
                        rubik_colors_help[column][x][y][i] = rubik_colors[column][u][x][face_array_rotation_l[i]];
                }
                u++;
			}
		}

		for (int x=0;x<3;x++) for (int z=0;z<3;z++) {
			memcpy(&rubik_colors[column][x][z], &rubik_colors_help[column][x][z], sizeof(int) * 6);
		}
	}
	void UpdateDataVertical(int depth, bool direction) {
		int rubik_colors_help[3][3][3][6];
		int face_array_rotation_l[] = { 4, 1, 5, 3, 2, 0 };
		int face_array_rotation_r[] = { 5, 1, 4, 3, 0, 2 };

		for(int x = 0; x < 3; x++) { // Schleife von Reihe0 - Reihe2
			int u = 0;
            
			for (int y = 2; y >= 0; y--) { // Schleife von Spalte0 - Spalte2
                for(int i = 0; i < 6; i++) {
                    if(!direction)
                        rubik_colors_help[x][y][depth][i] = rubik_colors[u][x][depth][face_array_rotation_r[i]];
                    else
                        rubik_colors_help[y][x][depth][i] = rubik_colors[x][u][depth][face_array_rotation_l[i]];
                }
                u++;
			}
		}

		for (int x = 0; x < 3; x++) for (int z = 0; z < 3; z++) {
			memcpy(&rubik_colors[x][z][depth], &rubik_colors_help[x][z][depth], sizeof(int) * 6);
		}
	}
    
	void DrawCoordinateAxis() {
		float a = 1.0;
        float b = 12.0;

        glBegin(GL_TRIANGLES);
            glColor3f(0.8f, 0.8f, 0.8f);
            // x-Achse:
            glVertex3f(b, 0, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, a);

            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex3f(0, a, 0);
            glVertex3f(0, 0, a);
            glVertex3f(b, 0, 0);

            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3f(0, 0, 0);
            glVertex3f(0, a, 0);
            glVertex3f(b, 0, 0);

            glColor3f(0.8f, 0.8f, 0.8f);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, a);
            glVertex3f(0, a, 0);
        glEnd();
	}
    
	void DrawCoordinateSystem() {
		glPushMatrix();

		glTranslatef(-4, -5, -5);

		glRotatef(180.0f, 1, 0, 0);

		DrawCoordinateAxis();

		glPushMatrix();
		glTranslatef(12.0f, 0.0f, 0.0f);
		glPopMatrix();

		glTranslatef(-1, -1, 0);
		glRotatef(-90.0f, 0, 0, 1);
		glRotatef(90.0f, 1, 0, 0);

		DrawCoordinateAxis();

		glPushMatrix();
		glTranslatef(12.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0, 0, 1);
		glPopMatrix();

		glTranslatef(-1, -1, 0);
		glRotatef(-90.0f, 0, 0, 1);
		glRotatef(90.0f, 1, 0, 0);

		DrawCoordinateAxis();

		glPushMatrix();
		glTranslatef(12.0f, 0.0f, 0.0f);
		glPopMatrix();

		glPopMatrix();
	}
    
	bool inRotation() {
		bool inRotation = false; 
		
		for (int i = 0; i < 6; i++) {
			if (rotation[i] != 0.0f) 
			{ 
				inRotation = true; 
				break; 
			} 
		}

		return inRotation;
	}
    
	void HandleKeyboard() {
		// smooth rotation for row0
		if(rotation[TOP] != 0.0f) {
			if(rotation[TOP] >= 0.0f && rotation[TOP] <= 0.2f * frametime) {
				UpdateDataHorizontal(2, false);  
				rot = 0.0f;
				rotation[TOP] = 0.0f;
				movements_done++;
			} //glRotate(full_angle * elapsed_time / rotation_time
			else if (rotation[TOP] <= 0.0f && rotation[TOP] >= -0.2f * frametime) { // 0.0 -0.1
				UpdateDataHorizontal(2, true);
				rot = 0.0f;
				rotation[TOP] = 0.0f;
				movements_done++;
			} 

			if (rotation[TOP] > 0.0f) { rotation[TOP] -= 0.2f * frametime; rot += -0.2f * frametime; }  // 0.1
			if (rotation[TOP] < 0.0f) { rotation[TOP] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikHorizontal(0,0,rot);
		}


		// smooth rotation for row2
		if(rotation[BOTTOM] != 0.0f) {
			if(rotation[BOTTOM] >= 0.0f && rotation[BOTTOM] <= 0.2f * frametime) {
				UpdateDataHorizontal(0, false);  
				rot = 0.0f;
				rotation[BOTTOM] = 0.0f;
				movements_done++;
			}
			else if (rotation[BOTTOM] <= 0.0f && rotation[BOTTOM] >= -0.2f * frametime) {
				UpdateDataHorizontal(0, true);  
				rot = 0.0f;
				rotation[BOTTOM] = 0.0f;
				movements_done++;
			} 

			if (rotation[BOTTOM] > 0.0f){ rotation[BOTTOM] -= 0.2f * frametime; rot -= 0.2f * frametime; }
			if (rotation[BOTTOM] < 0.0f){ rotation[BOTTOM] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikHorizontal(rot,0,0);
		}


		// smooth rotation for column0
		if(rotation[LEFT] != 0.0f) {
			if(rotation[LEFT] >= 0.0f && rotation[LEFT] <= 0.2f * frametime) {
				UpdateDataDepth(0, false); 
				rot = 0.0f;
				rotation[LEFT] = 0.0f;
				movements_done++;
			}	 
 
			else if (rotation[LEFT] <= 0.0f && rotation[LEFT] >= -0.2f * frametime) {
				UpdateDataDepth(0, true);
				rot = 0.0f;
				rotation[LEFT] = 0.0f;
				movements_done++;
			} 

			if (rotation[LEFT] > 0.0f) { rotation[LEFT] -= 0.2f * frametime; rot -= 0.2f * frametime; }
			if (rotation[LEFT] < 0.0f) { rotation[LEFT] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikDepth(rot,0,0);
		}

		// smooth rotation for column2
		if(rotation[RIGHT] != 0.0f) {
			if(rotation[RIGHT] >= 0.0f && rotation[RIGHT] <= 0.2f * frametime) {
				UpdateDataDepth(2, false); 
				rot = 0.0f;
				rotation[RIGHT] = 0.0f;
				movements_done++;
			}
			else if (rotation[RIGHT] <= 0.0f && rotation[RIGHT] >= -0.2f * frametime) {
				UpdateDataDepth(2, true);
				rot = 0.0f;
				rotation[RIGHT] = 0.0f;
				movements_done++;
			} 

			if (rotation[RIGHT] > 0.0f) { rotation[RIGHT] -= 0.2f * frametime; rot -= 0.2f * frametime; }
			if (rotation[RIGHT] < 0.0f) { rotation[RIGHT] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikDepth(0,0,rot);
		}

		// smooth rotation for row0
		if(rotation[FRONT] != 0.0f) {
			if(rotation[FRONT] >= 0.0f && rotation[FRONT] <= 0.2f * frametime) {
				UpdateDataVertical(2, false); 
				rot = 0.0f;
				rotation[FRONT] = 0.0f;
				movements_done++;
			}
			else if (rotation[FRONT] <= 0.0f && rotation[FRONT] >= -0.2f * frametime) {
				UpdateDataVertical(2, true);
				rot = 0.0f;
				rotation[FRONT] = 0.0f;
				movements_done++;
			} 

			if (rotation[FRONT] > 0.0f) { rotation[FRONT] -= 0.2f * frametime; rot -= 0.2f * frametime; }
			if (rotation[FRONT] < 0.0f) { rotation[FRONT] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikVertical(0,0,rot);
		}

		// smooth rotation for row0
		if(rotation[BACK] != 0.0f) {
			if(rotation[BACK] >= 0.0f && rotation[BACK] <= 0.2f * frametime) {
				UpdateDataVertical(0, false); 
				rot = 0.0f;
				rotation[BACK] = 0.0f;
				movements_done++;
			}
			else if (rotation[BACK] <= 0.0f && rotation[BACK] >= -0.2f * frametime) {
				UpdateDataVertical(0, true);
				rot = 0.0f;
				rotation[BACK] = 0.0f;
				movements_done++;
			} 

			if (rotation[BACK] > 0.0f) { rotation[BACK] -= 0.2f * frametime; rot -= 0.2f * frametime; }
			if (rotation[BACK] < 0.0f) { rotation[BACK] += 0.2f * frametime; rot += 0.2f * frametime; }

			RenderRubikVertical(rot,0,0);
		}
	}
};


RubikCube rubi;

// this functions handles all SDL events every loop
bool handleEvents() {
	SDL_Event event;

 	//Next event
	while (SDL_PollEvent(&event))
	{
		//Check event
		switch(event.type) {
			case SDL_KEYDOWN:
            {
				switch (event.key.keysym.sym) {
				case SDLK_u:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.TOP] = -90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.TOP] = 90.0f;
					}
				    break;

				case SDLK_d:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.BOTTOM] = 90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.BOTTOM] = -90.0f;
					}
				    break;

				case SDLK_l:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.LEFT] = 90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.LEFT] = -90.0f;
					}
				    break;

				case SDLK_r:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.RIGHT] = -90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false; 
						rubi.rotation[rubi.RIGHT] = 90.0f;
					}
				    break;

				case SDLK_f:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false; 
						rubi.rotation[rubi.FRONT] = -90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false; 
						rubi.rotation[rubi.FRONT] = 90.0f;
					}
				    break;

				case SDLK_b:
					if(!rubi.inRotation() && event.key.keysym.mod & KMOD_SHIFT) {
						key_map[NO_GAME_KEY] = false; 
						rubi.rotation[rubi.BACK] = 90.0f;
					}
					else if(!rubi.inRotation()) {
						key_map[NO_GAME_KEY] = false;
						rubi.rotation[rubi.BACK] = -90.0f;
					}
				    break;

				case SDLK_ESCAPE:
					return false;
					break;
				}

				break;
			}

			case SDL_QUIT:
				return false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					oldMousePos_x = event.motion.x;
					oldMousePos_y = event.motion.y;

					bLButtonDown = true;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					bLButtonDown = false;
				}
				break;

			case SDL_MOUSEMOTION:
				if (bLButtonDown) {
					rot[0] -= ((oldMousePos_y - event.motion.y) * 180.0f) / 200.0f;
					rot[1] -= ((oldMousePos_x - event.motion.x) * 180.0f) / 200.0f;
					oldMousePos_x = event.motion.x;
					oldMousePos_y = event.motion.y;
				}
				break;

			default:
				break;
		}
	}

	return true;
}

int DrawGLScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	// Reset the transformation matrix to the identity matrix:
	// [ 1 0 0 0 ]
	// [ 0 1 0 0 ]
	// [ 0 0 1 0 ]
	// [ 0 0 0 1 ]
	glLoadIdentity();									// Reset The Current Modelview Matrix

	glTranslatef(-4.0,1.0,-25.0);						// Move Out the Screen -25.0

	glTranslatef(-1,-1,-1);

	// rotate the whole cube
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);

	glTranslatef(1,1,1);

	rubi.DrawCoordinateSystem();

	if( SDL_GetTicks() - LastFPS >= 1000 ) { // When A Second Has Passed...
		LastFPS = SDL_GetTicks();			// Update Our Time Variable
		FPS = Frames;						// Save The FPS
		Frames = 0;							// Reset The FPS Counter
	}

	if((movements_done == movements.length() || movements.length() == 0) && !rubi.inRotation()) {
		rubi.RenderRubikHorizontal(0,0,0);
	}
    
	if(movements.length() > 0 && !rubi.inRotation()) {
		switch(movements[movements_done]) {
		case 'Q':
			rubi.rotation[rubi.TOP] = 90.0f;
			break;

		case 'q':
			rubi.rotation[rubi.TOP] = -90.0f;
			break;

		case 'D':
			rubi.rotation[rubi.BACK] = -90.0f;
			break;

		case 'd':
			rubi.rotation[rubi.BACK] = 90.0f;
			break;

		case 'W':
			rubi.rotation[rubi.LEFT] = -90.0f;
			break;

		case 'w':
			rubi.rotation[rubi.LEFT] = 90.0f;
			break;

		case 'S':
			rubi.rotation[rubi.RIGHT] = 90.0f;
			break;

		case 's':
			rubi.rotation[rubi.RIGHT] = -90.0f;
			break;

		case 'E':
			rubi.rotation[rubi.FRONT] = 90.0f;
			break;

		case 'e':
			rubi.rotation[rubi.FRONT] = -90.0f;
			break;

		case 'A':
			rubi.rotation[rubi.BOTTOM] = -90.0f;
			break;

		case 'a':
			rubi.rotation[rubi.BOTTOM] = 90.0f;
			break;
		}
	}

	rubi.HandleKeyboard();


	// Set Bitmap Font
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
    glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);	// Set the color to white and display some messages
	font.printAt(20,450,"Keys: l=Left, f=Front, r=Right, b=Back, u=Up, d=Down");
	font.printAt(80,430,"Shift+l=Left',...");
	
	stringstream str;
	str << "FPS: " << FPS;
	font.printAt(20,20,str.str().c_str());
	Frames++;

	glPopAttrib();


	glLoadIdentity();

	return true; // Alles hat geklappt
}

bool InitGL(int width, int height) { // Initialisierung des OpenGL-Fensters
	if (height == 0) {									// Prevent A Divide By Zero By
		height = 1;										// Making Height Equal One
	}
    
	glViewport(0,0,width,height);						// Reset The Current Viewport
    
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//glOrtho(-12.5, 12.5, -10, 10, 0.1, 100);
    
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
    
	//glEnable(GL_NORMALIZE);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // initialisiere das ambiente Licht
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	//glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // Position des Lichts
	//glEnable(GL_LIGHT1); // aktiviere Licht eins
	//glEnable(GL_LIGHTING); // aktiviere Beleuchtung
    
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);				// light-black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    
	// set caption
    //SDL_WM_SetCaption("The Rubik Cube (c) by Stanje Gerald","");
    
	return true;										// Initialization Went OK
}

bool init() {
	// Initialization flag
	bool success = true;
    
	// Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		cout << "SDL could not initialize! SDL Error " << SDL_GetError() << endl;
		success = false;
	}
	else {
		// Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        
		// Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else {
			// Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL ) {
				cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else {
				// Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
					cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
				}
                
				// Initialize OpenGL
				if( !InitGL(SCREEN_WIDTH, SCREEN_HEIGHT) ) {
					cout << "Unable to initialize OpenGL!" << endl;
					success = false;
				}
			}
		}
	}
    
	return success;
}

void close() {
	// Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
    
	// Quit SDL subsystems
	SDL_Quit();
}

#include <errno.h>
#include <libproc.h>
#include <unistd.h>

int main(int argc, char **argv) {
    memset(&key_map, 0, sizeof(key_map));
    
    // Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
        // Build the font from a BMP image font.bmp
        font.init("data/font.bmp");
        texture_white.createTexture("data/template_white.bmp");
        texture_red.createTexture("data/template_red.bmp");
        texture_orange.createTexture("data/template_orange.bmp");
        texture_yellow.createTexture("data/template_yellow.bmp");
        texture_blue.createTexture("data/template_blue.bmp");
        texture_green.createTexture("data/template_green.bmp");
        texture_black.createTexture("data/template_black.bmp");
        
        // create the displaylist
        for (int i=0;i<7;i++) rubi.make_face(rubi.cube_faces[i],i);
        
        // initalizing colors
        int surface_color[54] = {rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE, rubi.ORANGE,
                                 rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW, rubi.YELLOW,
                                 rubi.RED, rubi.RED, rubi.RED, rubi.RED, rubi.RED, rubi.RED, rubi.RED, rubi.RED, rubi.RED,
                                 rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE, rubi.WHITE,
                                 rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN, rubi.GREEN,
                                 rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE, rubi.BLUE};
        
        for (int x=0;x<3;x++) for (int y=0;y<3;y++) for (int z=0;z<3;z++) for (int face=0;face<6;face++) rubi.rubik_colors[x][y][z][face] = rubi.BLACK;
        
        rubi.intialize_surface(surface_color);
        
		// Main loop flag
		bool quit = false;
        
		// Event handler
		SDL_Event e;
        
		// Enable text input
		SDL_StartTextInput();
        
		// While application is running
		while( !quit ) {
            if (! handleEvents() ) {
                break;
            }
            
            // Init values
            static float framestart = SDL_GetTicks();
            static float frameend = 0.0f;
            
            // Repeat this loop until the start of the frame
            // is not the same as the end of frame (prevents on
            // fast machines that a frame will be drawn twice)
            do {
                frameend = SDL_GetTicks();
            }
            while (frameend == framestart);
            
            // calculate frametime
            frametime = frameend - framestart;
            framestart = frameend;
            
            // Die Szene ausgeben
            DrawGLScene();
            
			// Update screen
			SDL_GL_SwapWindow( gWindow );
		}
        
		// Disable text input
		SDL_StopTextInput();
	}
    
	// Free resources and close SDL
	close();

    return 0;
}