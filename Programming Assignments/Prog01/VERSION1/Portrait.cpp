#include "Portrait.hpp"
#include "glPlatform.h"


Portrait::Portrait(float portrait_center[2], float scale, float orientation) 
    : colors{{0.9f,0.776f,0.7f, 1.f},{74.0f/255.0f, 45.f/255.f, 25.f/255.f, 1.f},{0.1f, 0.7f, 0.1f, 1.f}}
{
    this->scale = scale;
    this->orientation = orientation;
    for (int i = 0; i < 2; i++) {
        this->portrait_center[i] = portrait_center[i];
    }
    
}

Portrait::Portrait(float portrait_center[2], float scale, float orientation, float colors[3][4]) {
    this->scale = scale;
    this->orientation = orientation;
    for (int i = 0; i < 2; i++) {
        this->portrait_center[i] = portrait_center[i];
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            this->colors[i][j] = colors[i][j];
        }
    }
}

void Portrait::draw(void) const {

//Setup orgin orientation, and scaling
// Save previous location data
glPushMatrix();
glTranslatef(portrait_center[0], portrait_center[1], 0.f);
glRotatef(orientation, 0.f, 0.f, 1.f);
glScalef(scale, scale, 0);

//Draw the Face
glColor3fv(colors[0]);
this->draw_ellipse(0, 0, 0, 0.f, 2*M_PI, 300, 400, true);

//Draw the ears
this->draw_ellipse(-280, -50, 0, 0.f, 2*M_PI, 50, 100, true); 
this->draw_ellipse(280, -50, 0, 0.f, 2*M_PI, 50, 100, true); 

//Draw the Hair
glColor3fv(colors[1]);
this->draw_ellipse(-300, 100, -10, 0, 2*M_PI, 50, 100, true);
this->draw_ellipse(-100, 250, -60, 0, 2*M_PI, 150, 200, true);

glPopMatrix();

glEnd();
// Return to previous location/orientation
glPopMatrix();
}

void Portrait::draw_ellipse(int x,int y,float rotation_angle, float start_angle,float end_angle,int radius_x, int radius_y, bool fill) const {
glPushMatrix();
glTranslatef(x, y, 0);
glRotatef(rotation_angle, 0.f, 0.f, 1.f);
double angle, increment;
if (fill) {
    glBegin(GL_POLYGON);
} else {
    glBegin(GL_LINE_STRIP);
}
	increment=M_PI/std::max(radius_x,radius_y)/2;
	angle=start_angle;
	while(angle<=end_angle)
	{
		glVertex2f(int((radius_x*cos(angle))+.5),int((radius_y*sin(angle))+.5));
		angle=angle+increment;
	}
glEnd();
glPopMatrix();
}

void Portrait::draw_square() const {

}

void Portrait::draw_triagle() const {

}