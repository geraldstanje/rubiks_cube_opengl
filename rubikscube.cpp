//
//  rubikscube.cpp
//  Rubiks_Cube_OpenGL
//
//  Created by Gerald Stanje on 4/17/14.
//  Copyright (c) 2014 Gerald. All rights reserved.
//

#include "rubikscube.h"

RubiksCube::RubiksCube() {
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
    
    FPS = 0;
    Frames = 0;
    LastFPS = 0; // Last FPS Check Time
    frametime = 0.0f;
    
    movements_done = 0;
}
    
void RubiksCube::make_face(GLuint &displaylist, int color) {
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
    
void RubiksCube::intialize_surface(const int *surface_color) {
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
    
void RubiksCube::intialize_face(const int *surface_color, unsigned int side) {
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
    
void RubiksCube::RenderCube(float X, float Y, float Z, float SCALE, color_container color) {
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
    
void RubiksCube::RenderRubikHorizontal(float rotA, float rotB, float rotC) {
        
    for (int y=0;y<3;y++) {
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
    
void RubiksCube::RenderRubikDepth(float rotA,float rotB,float rotC) {
        
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
    
void RubiksCube::RenderRubikVertical(float rotA,float rotB,float rotC) {
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

void RubiksCube::UpdateDataHorizontal(int row, bool direction) {
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

void RubiksCube::UpdateDataDepth(int column, bool direction) {
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

void RubiksCube::UpdateDataVertical(int depth, bool direction) {
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
    
void RubiksCube::DrawCoordinateAxis() {
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
    
void RubiksCube::DrawCoordinateSystem() {
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
    
bool RubiksCube::inRotation() {
    bool inRotation = false;
		
    for (int i = 0; i < 6; i++) {
        if (rotation[i] != 0.0f) {
            inRotation = true;
            break;
        }
    }
        
    return inRotation;
}
    
void RubiksCube::HandleKeyboard() {
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

void RubiksCube::load_textures() {
    // Build the font from a BMP image font.bmp
    font.init("data/font.bmp");
    texture_white.createTexture("data/template_white.bmp");
    texture_red.createTexture("data/template_red.bmp");
    texture_orange.createTexture("data/template_orange.bmp");
    texture_yellow.createTexture("data/template_yellow.bmp");
    texture_blue.createTexture("data/template_blue.bmp");
    texture_green.createTexture("data/template_green.bmp");
    texture_black.createTexture("data/template_black.bmp");
}