#include <sstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "rubikscube.h"

using namespace std;

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

//GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambiente Lichtwere
//GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Diffuse Lichtwerte
//GLfloat LightSpecular[]= { 0.0f, 0.0f, 0.0f, 0.0f }; // Diffuse Lichtwerte
//GLfloat LightEmission[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat LightPosition[]= { 0.0f, 0.0f, -1.0f, 1.0f }; // Lichtposition
//bool scan_colors = false;
//bool solve_rubik = false;

RubiksCube rubi;

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

	if( SDL_GetTicks() - rubi.LastFPS >= 1000 ) { // When A Second Has Passed...
		rubi.LastFPS = SDL_GetTicks();			// Update Our Time Variable
		rubi.FPS = rubi.Frames;						// Save The FPS
		rubi.Frames = 0;							// Reset The FPS Counter
	}

	if((rubi.movements_done == rubi.movements.length() || rubi.movements.length() == 0) && !rubi.inRotation()) {
		rubi.RenderRubikHorizontal(0,0,0);
	}
    
	if(rubi.movements.length() > 0 && !rubi.inRotation()) {
		switch(rubi.movements[rubi.movements_done]) {
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
	rubi.font.printAt(20,450,"Keys: l=Left, f=Front, r=Right, b=Back, u=Up, d=Down");
	rubi.font.printAt(80,430,"Shift+l=Left',...");
	
	stringstream str;
	str << "FPS: " << rubi.FPS;
	rubi.font.printAt(20,20,str.str().c_str());
	rubi.Frames++;

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

int main(int argc, char **argv) {
    memset(&key_map, 0, sizeof(key_map));
    
    // Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
        rubi.load_textures();
        
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
            rubi.frametime = frameend - framestart;
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