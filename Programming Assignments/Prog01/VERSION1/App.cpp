#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "glPlatform.h"
#include "Portrait.hpp"

using namespace std;

//--------------------------------------
//  Function prototypes
//--------------------------------------
void display(void);
void resize_window(int w, int h);
void on_idle(void);

//--------------------------------------
//  File-level global variables
//--------------------------------------

int winWidth = 800;
int winHeight = 800;
float color1[]  = {1.f, 0.f, 0.f};
float color2[]  = {0.f, 1.f, 1.f};
float* currentColor = color1;
float window_center[2] = {(float)winHeight/2, (float)winWidth/2};

Portrait* beautiful_portrait;

//--------------------------------------
//  Interface constants
//--------------------------------------

const int   INIT_WIN_X = (1920-winWidth)/2,
            INIT_WIN_Y = (1080-winHeight)/2;


void display(void) {
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set mode to model view, which we need to be in to draw.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//--------------------------
	//	basic drawing code
	//--------------------------
	beautiful_portrait->draw();

	// swap buffer we drew in into the front
	glutSwapBuffers();
}


//	This callback function is called when the window is resized
//	(generally by the user of the application).
//	It is also called when the window is created, why I placed there the
//	code to set up the virtual camera for this application.
//
void resize_window(int w, int h)
{
	//	Use entire window for drawing
	glViewport(0, 0, w, h);
	winWidth = w; 
    winHeight = h;
	
	// Create virtual camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//	Dimensions of the "virtual world" that window maps to
	// gluOrtho2D(0.f, winWidth, 0.f, winHeight);

	// Fix for Linux
	glOrtho(0.f, winWidth, 0.f, winHeight, -1, 1);

	//	Refresh display
	glutPostRedisplay();
}

void on_idle(void)
{
    //  possibly I do something to update the scene
    
	//	And finally I perform the rendering
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//	Initialize glut and create a new window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutCreateWindow("Mason's Gorgeous Portrait");
	beautiful_portrait = new Portrait(window_center, 1, 0);

	
	//	set up the callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize_window);
    glutIdleFunc(on_idle);

	//	Enter main loop 
	glutMainLoop();

	return 0;
}
