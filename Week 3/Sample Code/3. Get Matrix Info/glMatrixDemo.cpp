//
#include <stdlib.h>
#include <iostream>
#include <cmath>
//
#include "glPlatform.h"

using namespace std;

//--------------------------------------
//  Function prototypes
//--------------------------------------
void cleanupAndExit(void);
void myDisplay(void);
void noDisplay(void);
void myResize(int w, int h);
void myKeyboard(unsigned char c, int x, int y);
void on_idle(void);
void drawSquare(float scale, float r, float g, float b, bool contour);
void doMatrixComputations(void);
void printMatrix(const GLfloat* m);
void drawReferenceFame(GLfloat scale);

//--------------------------------------
//  Configuration variables
//--------------------------------------

//  Set to 1 to get a printout of the model view matrix
#define PRINT_TRACE         	0
//  Set to 1 to use glMultMatrix instead of geometric transformation
#define USE_MULT_MATRIX     	1

//	set to 1 to draw the current reference frame
#define DRAW_REFERENCE_FRAME	1


//--------------------------------------
//  File-level global variables
//--------------------------------------
const int   INIT_WIN_X = 100,
            INIT_WIN_Y = 40;

int winWidth = 1000,
    winHeight = 1000;


//==========================================================
//  GLUT callback functions
//==========================================================

//  This is the function that does the actual scene drawing
void myDisplay(void)
{
    //  I will use this array to store the matrix querried from OpenGL
    //  I know that's it's the flattened version of a 4x4 transformation matrix,
    //  read column-by-column
    #if PRINT_TRACE
        GLfloat A[16];
    #endif
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    #if PRINT_TRACE
        cout << endl << "----------------" << endl << endl;
        glGetFloatv(GL_MODELVIEW_MATRIX, A);
        cout << "Start configuration" << endl;
        printMatrix(A);
    #endif

    //   preserve Reference frame R0
    glPushMatrix();
    #if PRINT_TRACE
        glGetFloatv(GL_MODELVIEW_MATRIX, A);
        cout << "After push matrix (just to prove/verify: matrix unchanged)" << endl;
        printMatrix(A);
    #endif

	//------------------------------
	//	The full drawing sequence
	//------------------------------
	//	---> Start in Reference frame R0
	//	1.  translate by +2 along the X axis			\  these two can be
	//	2.  translate by -1 along the (new) Y axis      /  swapped
	//	---> We are in Reference frame R1
	//	3.  rotate +π/6 (around the new Z axis)
	//	---> We are in Reference frame R2
	//	---> draw a small blue square
	//	4.  rotate -π/4 (around the new Z axis)
	//	---> We are in Reference frame R3
	//	5.  translate by -5 along the new X axis
	//	---> We are in Reference frame R4
	//	---> draw a larger gray square


	//----------------------------------------------------
    //  Regular version with geometric transformations
	//----------------------------------------------------
    #if USE_MULT_MATRIX == 0

		//	1.  translate by +2 along the X axis			\  these two can be
		//	2.  translate by -1 along the (new) Y axis      /  swapped
		//	the same, broken into 2 steps (and slower)
		//		glTranslatef(2.f, 0.f, 0.f);
		//		glTranslatef(0.f, -1.f, 0.f);
		//	---> We are in Reference frame R1
		glTranslatef(2.f, -1.f, 0.f);

		//   preserve Reference frame R1
		glPushMatrix();

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After translations 1 & 2 (Reference frame R1)" << endl;
			printMatrix(A);
		#endif

		//	3.  rotate +π/6 (around the new Z axis)
        glRotatef(30.f, 0.f, 0.f, 1.f);
		//	---> We are in Reference frame R2

		//   preserve Reference frame R2
		glPushMatrix();

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After rotation 1, when drawing blue square (Reference frame R2)" << endl;
			printMatrix(A);
		#endif

		//	---> draw a small blue square
		drawSquare(1.f, 0.f, 0.f, 1.f, true);
		#if DRAW_REFERENCE_FRAME
			drawReferenceFame(1.f);
		#endif

		//	4.  rotate -π/4 (around the new Z axis)
		glRotatef(-45.f, 0.f, 0.f, 1.f);
		//	---> We are in Reference frame R3

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After second rotation (Reference frame R3)" << endl;
			printMatrix(A);
		#endif
		
		//	5.  translate by -5 along the new X axis
		glTranslatef(-5.f, 0.f, 0.f);
		//	---> We are in Reference frame R4

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "When drawing gray square (Reference frame R4)" << endl;
			printMatrix(A);
		#endif

		//	---> draw a larger gray square
		drawSquare(2.5, 0.7f, 0.7f, 0.7f, true);

		#if DRAW_REFERENCE_FRAME
			drawReferenceFame(1.f);
		#endif

	//----------------------------------------------------
	//  Version with glMultMatrix
	//----------------------------------------------------
	#else

        GLfloat mat1[16] = { 1., 0.f, 0.f, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             0.f, 0.f, 1.f, 0.f,
                             2.f, 0.f, 0.f, 1.f};
        //  corresponds to this matrix, read column-by-column
        //      ( 1   0   0   2 )
        //      ( 0   1   0   0 )
        //      ( 0   0   1   0 )
        //      ( 0   0   0   1 )
        GLfloat mat2[16] = { 1., 0.f, 0.f, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             0.f, 0.f, 1.f, 0.f,
                             0.f, -1.f, 0.f, 1.f};
        //  corresponds to this matrix, read column-by-column
        //      ( 1   0   0    0 )
        //      ( 0   1   0   -1 )
        //      ( 0   0   1    0 )
        //      ( 0   0   0    1 )

		GLfloat mat3[16] = { sqrtf(3)/2, 0.5f, 0.f, 0.f,
							 -0.5f, sqrtf(3)/2, 0.f, 0.f,
							 0.f, 0.f, 1.f, 0.f,
							 0.f, 0.f, 0.f, 1.f};
		//  corresponds to this matrix, read column-by-column
		//      ( √3/2   -1/2   0   0 )
		//      (  1/2   √3/2   0   0 )     note:  30 degree --> π/6, and
		//      (  0       0    1   0 )            cos(π/6) = √3/2
		//      (  0       0    0   1 )            sin(π/6) = 1/2

		//  rotation by -π/4 = -90 degree (cos(-π/4) = √2/2, sin(-π/4) = -√2/2)
		const float CS2 = sqrtf(2.f)/2.f;
		GLfloat mat4[16] = { CS2, -CS2, 0.f, 0.f,
							 CS2, CS2, 0.f, 0.f,
							 0.f, 0.f, 1.f, 0.f,
							 0.f, 0.f, 0.f, 1.f};
		//  corresponds to this matrix, read column-by-column
		//      (  √2/2  √2/2  0  0 )
		//      ( -√2/2  √2/2  0  0 )
		//      (   0     0    1  0 )
		//      (   0     0    0  1 )


		//      translation by -4 along current x axis
		GLfloat mat5[16] = { 1.f, 0.f, 0.f, 0.f,
							 0.f, 1.f, 0.f, 0.f,
							 0.f, 0.f, 1.f, 0.f,
							 -5.f, 0.f, 0.f, 1.f};
		//  corresponds to this matrix, read column-by-column
		//      ( 1  0  0  -5 )
		//      ( 0  1  0   0 )
		//      ( 0  0  1   0 )
		//      ( 0  0  0   1 )

		//	1.  translate by +2 along the X axis
		glMultMatrixf(mat1);

		//	2.  translate by -1 along the (new) Y axis
		glMultMatrixf(mat2);
		//	---> We are in Reference frame R1

		//   preserve Reference frame R1
		glPushMatrix();

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After translations, (Reference frame R1)" << endl;
			printMatrix(A);
		#endif
		
		//	3.  rotate +π/6 (around the new Z axis)
		glMultMatrixf(mat3);
		//	---> We are in Reference frame R2
		
		//   preserve Reference frame R2
		glPushMatrix();

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After rotation 1, when drawing blue square (Reference frame R2)" << endl;
			printMatrix(A);
		#endif

		//	---> draw a small blue square
		drawSquare(1.f, 0.f, 0.f, 1.f, true);
		#if DRAW_REFERENCE_FRAME
			drawReferenceFame(1.f);
		#endif

		//	4.  rotate -π/4 (around the new Z axis)
		glMultMatrixf(mat4);
		//	---> We are in Reference frame R3

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "After second rotation (Reference frame R3)" << endl;
			printMatrix(A);
		#endif
		
		//	5.  translate by -5 along the new X axis
		glMultMatrixf(mat5);
		//	---> We are in Reference frame R4

		#if PRINT_TRACE
			glGetFloatv(GL_MODELVIEW_MATRIX, A);
			cout << "When drawing gray square (Reference frame R4)" << endl;
			printMatrix(A);
		#endif

		//	---> draw a larger gray square
		drawSquare(2.5, 0.7f, 0.7f, 0.7f, true);

		#if DRAW_REFERENCE_FRAME
			drawReferenceFame(1.f);
		#endif

	#endif

	//  Restoring Reference frame R1
	glPopMatrix();
    #if PRINT_TRACE
        glGetFloatv(GL_MODELVIEW_MATRIX, A);
        cout << "Pop back to Reference frame R1" << endl;
        printMatrix(A);
    #endif

    //  Restoring Reference frame R0
    glPopMatrix();
    #if PRINT_TRACE
        glGetFloatv(GL_MODELVIEW_MATRIX, A);
        cout << "Pop back to Reference frame R0" << endl;
        printMatrix(A);
    #endif

	#if DRAW_REFERENCE_FRAME
		drawReferenceFame(1.f);
	#endif

    //  We were drawing into the back buffer, now it should be brought
    //  to the forefront.
    glutSwapBuffers();
}


void noDisplay(void)
{
    // do nothing
}

//  This callback function is called when the window is resized
//  (generally by the user of the application).
//
void myResize(int w, int h)
{
    //  update the viewport to fill the entire window
    //  Since I haven't changed my projection matrix (defined in my
    //  call to gluOrtho2D), the display will scale.  I could try here
    //  to preserve the aspect ratio be recomputing w and/or h to agree
    //  with that ratio.
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    
    //  When it's done, request a refresh of the display
    glutPostRedisplay();
}


//  This callback function is called when a keyboard event occurs
//
void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        // 'ESC' key for exit
        case 27:
            //  If your glut implementation provides glutLeaveMainLoop()
            //  Then you could call it here to leave the glut main loop and
            //  return from the call to glutMainLoop(), in the main function.
            //  glutLeaveMainLoop();
            
            //  If this function is not available, then I do all my cleanup
            //  prior to terminating the program
            cleanupAndExit();
            break;
        
        case 'm':
        case 'M':
            doMatrixComputations();
            break;
            
        default:
            break;
    }
}

void on_idle(void)
{
    //  possibly I do something to update the scene
    
    //  And finally I perform the rendering
    glutPostRedisplay();
}


//=======================================================
//  Various drawing and calculation functions
//=======================================================

void drawSquare(float scale, float r, float g, float b, bool contour)
{
    glPushMatrix();
    glScalef(scale, scale, 1.f);
    
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2f(1, 0);
        glVertex2f(0, 1);
        glVertex2f(-1, 0.);
        glVertex2f(0, -1);
    glEnd();
    
    if (contour) {
    glColor3f(1-r, 1-g, 1-b);
    glBegin(GL_LINE_LOOP);
        glVertex2f(1, 0);
        glVertex2f(0, 1);
        glVertex2f(-1, 0.);
        glVertex2f(0, -1);
    glEnd();
    }
    glPopMatrix();
}


// draws the current reference frame
void drawReferenceFame(GLfloat scale)
{
	glBegin(GL_LINES);
		//	red x axis of length "scale"
		glColor3f(1.f, 0.f, 0.f);
		glVertex2f(0.f, 0.f);
		glVertex2f(scale, 0.f);
		//	green y axis of length "scale"
		glColor3f(0.f, 1.f, 0.f);
		glVertex2f(0.f, 0.f);
		glVertex2f(0.f, scale);
	glEnd();
}


void doMatrixComputations(void)
{
	GLfloat A[16];

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "Start at camera" << endl;
    printMatrix(A);
    
    glRotatef(30.f, 0.f, 0.f, 1.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After rotation" << endl;
    printMatrix(A);
    
    glTranslatef(4.f, 0.f, 5.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After translation" << endl;
    printMatrix(A);
    
    glScalef(3.f, -0.5f, 2.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After scaling" << endl;
    printMatrix(A);
    
    GLfloat B[16];
    glLoadIdentity();
    cout << "Restart at camera" << endl;
    glScalef(1.f/3.f, -1.f/0.5f, 1.f/2.f);
    glTranslatef(-4.f, 0.f, -5.f);
    glRotatef(-30.f, 0.f, 0.f, 1.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, B);
    cout << "Inverse transformation" << endl;
    printMatrix(B);
    
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "Retart at camera" << endl;
    printMatrix(A);
    
    glRotatef(30.f, 0.f, 0.f, 1.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After rotation" << endl;
    printMatrix(A);
    
    glTranslatef(4.f, 0.f, 5.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After translation" << endl;
    printMatrix(A);
    
    glScalef(3.f, -0.5f, 2.f);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After scaling" << endl;
    printMatrix(A);

    GLfloat C[16] = {-10.f, 0.f, 0.f, 0.f, 0.f, 100.f, 0.f, 0.f,
                     0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
    glMultMatrixf(C);
    glGetFloatv(GL_MODELVIEW_MATRIX, A);
    cout << "After applying scaling matrix C" << endl;
    printMatrix(A);

    glPopMatrix() ;
}

void printMatrix(const GLfloat* m) {
    cout << "((" << m[0] << "\t" << m[4] << "\t" << m[8] << "\t" << m[12] << ")" << endl;
    cout << " (" << m[1] << "\t" << m[5] << "\t" << m[9] << "\t" << m[13] << ")" << endl;
    cout << " (" << m[2] << "\t" << m[6] << "\t" << m[10] << "\t" << m[14] << ")" << endl;
    cout << " (" << m[3] << "\t" << m[7] << "\t" << m[11] << "\t" << m[15] << "))" << endl;
}

//=======================================================
//  Main function (and exit)
//=======================================================

void cleanupAndExit(void)
{
    //  If I need to delete objects, I don't want the display function
    //  to start redrawing stuff while they are being deleted.
    //  There may be a call to the glutTimerFunc somewhere in the pipeline,
    //  so I will simply redirect this call a display function that does nothing
    glutDisplayFunc(noDisplay);
    
    //  now I can delete stuff or close files to my heart's content
    
    //  and finally exit
    exit(0);
}

int main(int argc, char** argv)
{
    //  Initialize glut and create a new window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
    glutCreateWindow("demo CSC406");
    
    //  set up the callbacks
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myResize);
    glutKeyboardFunc(myKeyboard);
    glutIdleFunc(on_idle);

    //  I define a square "world" 10x10 with the origin at the center
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.f, 5.f, -5.f, 5.f);
    glMatrixMode(GL_MODELVIEW);

    //  Now we enter the main loop of the program and to a large extend
    //  "lose control" over its execution.  The callback functions that 
    //  we set up earlier will be called when the corresponding event
    //  occurs
    glutMainLoop();
    
    //  If we leave the glut main loop, then we end up here
    cout << "Out of the glut main loop" << endl;
    
    //  Now, possibly, we could clean things up, free storage, close files, etc.
    
    return 0;
}

