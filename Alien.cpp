
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <GL/freeglut.h>

using namespace std;

void display() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Alien Theme Park");
    
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
