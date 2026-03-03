
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <GL/freeglut.h>
#include <math.h>
#include "Model.h"

using namespace std;

const int FPS = 30;

bool skeystates[256] = {false};
float angle = 0;
float height = 5.0;

void drawFloor() {
	glColor3f(0., 0.5, 0.);

	for(int i = -50; i <= 50; i++)  {
		glBegin(GL_LINES);
			glVertex3f(-50., 0, i);
			glVertex3f( 50., 0, i);
			glVertex3f(i, 0, -50.);
			glVertex3f(i, 0,  50.);
		glEnd();
	}
}

void display() {
	float lpos[4] = {0., 10., 10., 1.0};  //light's position
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(15.*sin(angle*M_PI/180), height, 15.*cos(angle*M_PI/180), 0., 0., 0., 0., 1., 0.);

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);

	drawFloor();
    drawAlien(0, 0, 0);

    glFlush();
}

void initialize(void) {
	glClearColor(1., 1., 1., 1.);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20., 1., 10., 1000.);
}

void timer(int value) {
    if (skeystates[GLUT_KEY_LEFT]) {
        angle -= 1.5;
    } else if (skeystates[GLUT_KEY_RIGHT]) {
        angle += 1.5;
    } else if (skeystates[GLUT_KEY_UP]) {
        height += 0.4;
    } else if (skeystates[GLUT_KEY_DOWN]) {
        height -= 0.4;
    }
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, 0);
}

void specialUp(int key, int x, int y) {
    skeystates[key] = false;
}

void specialDown(int key, int x, int y) {
    skeystates[key] = true;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Alien Theme Park");
    glutIgnoreKeyRepeat(1);
    initialize();
    
    glutDisplayFunc(display);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutTimerFunc(1000/FPS, timer, 0);
    glutMainLoop();
    return 0;
}
