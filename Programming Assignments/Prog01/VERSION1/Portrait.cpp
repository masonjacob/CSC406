#include "Portrait.hpp"
#include "glPlatform.h"


Portrait::Portrait(float portrait_center[2], float scale, float orientation) {
    this->scale = scale;
    this->orientation = orientation;
    for (int i = 0; i < 2; i++) {
        this->portrait_center[i] = portrait_center[i];
    }

}

Portrait::Portrait(float portrait_center[2], float sclae, float orientation, float colors[3][4]) {
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

//Draw the Face
this->draw_ellipse(this->portrait_center[0], this->portrait_center[1], 0.f, 60.f, 300*this->scale, 400*this->scale, true);

//Draw the Hair
glBegin(GL_POLYGON);
glColor3f(0.9, 0.776, 0.729);
for (int i = 0; i < 40; i++) {
    glVertex2fv()
}
}

void Portrait::rotate_portrait(void) const {

}

void Portrait::draw_circle() const {

}

void Portrait::draw_ellipse(int x,int y,float start_angle,float end_angle,int radius_x, int radius_y, bool fill) const {
double angle, increment;
if (fill) {
    glBegin(GL_POLYGON);
} else {
    glBegin(GL_LINE_STRIP);
}
	glColor3f(0.9, 0.776, 0.729);
	increment=M_PI/std::max(radius_x,radius_y)/2;
	angle=start_angle;
	while(angle<=end_angle)
	{
		glVertex2f(int((radius_x*cos(angle)+y)+.5),int((radius_y*sin(angle)+x)+.5));
		angle=angle+increment;
	}
	glEnd();
}

void Portrait::draw_square() const {

}

void Portrait::draw_triagle() const {

}