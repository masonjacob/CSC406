//
//  Triangle.cpp
//  Week 01 - New
//
//  Created by Jean-Yves Hervé on 9/13/22.
//
#include "Triangle.hpp"
#include "glPlatform.h"

Triangle::Triangle(float theXy[][2])
{
	for (int k=0; k<3; k++)
	{
		xy[k][0] = theXy[k][0];
		xy[k][1] = theXy[k][1];
	}
}
	
void Triangle::draw(void) const
{
	glPushMatrix();
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glBegin(GL_POLYGON);
		for (size_t k=0; k<3; k++)
			glVertex2fv(xy[k]);
	glEnd();
	glPopMatrix();
}
