//=============================================================================
//  Physically-based Simulation in Computer Graphics
//  ETH Zurich
//=============================================================================

#include "Primitives.h"
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>

const double dv = 3;
const double thickness = 0.1;
const int CDIVS = 32;

// render a thick line
void MSpring::render()
{
	glColor3f(0.5, 0.5, 0.5);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2d(a->pos.x() / dv, a->pos.y() / dv);
	glVertex2d(b->pos.x() / dv, b->pos.y() / dv);
	glEnd();
}

// render a circle using triangle fans
void MPoint::render()
{
	if (fixed)
		glColor3f(0, 0, 1);
	else
		glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	double mx = pos.x();
	double my = pos.y();
	double r = 0.1;
	double ox = mx;
	double oy = my;
	glVertex2d(mx / dv, my / dv);
	for (int i = 0; i <= CDIVS; i++)
	{
		double ang = (double)i * M_PI * 2 / CDIVS;
		double x = r*sin(ang) + mx;
		double y = r*cos(ang) + my;
		glVertex2d(ox / dv, oy / dv);
		glVertex2d(x / dv, y / dv);
		ox = x;
		oy = y;
	}
	glEnd();
}
