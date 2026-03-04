
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <climits>
#include <GL/freeglut.h>
#include <math.h>
#include "Model.h"
#include "Animation.h"
#include "Utils.h"

using namespace std;

const int FPS = 30;

int frame = 0;
bool skeystates[256] = {false};
float dir = 180;
struct {
    float x = 0;
    float z = 10;
} pos;
float walk = -10;
float pinch = 0;
float waddle = -5;

void sidewalk() {
    static int inc = -1;
    inc *= -1;
    registerDynamicAnimation((animation) {
        .start_ts = frame,
        .end_ts = frame + 60,
        .animation_curve = easeInOutQuart,
        .from = (float)inc * 5,
        .to = (float)inc * 5 * -1,
        .value = &waddle,
        .callback = sidewalk
    });
}

void drawFloor() {
    bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = -50; x <= 50; x++) {
		for (int z = -50; z <= 50; z++) {
			if (flag) glColor3f(0.6, 1.0, 0.8);
			else 	  glColor3f(0.8, 1.0, 0.6);
			glVertex3f(x, -0.001, z);
			glVertex3f(x, -0.001, z+1);
			glVertex3f(x+1, -0.001, z+1);
			glVertex3f(x+1, -0.001, z);
			flag = !flag;
		}
	}
	glEnd();
}

void display() {
	float lpos[4] = {0., 10., 10., 1.0};  //light's position
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pos.x, 1.5, pos.z, pos.x + sin(M_PI*dir/180), 1.2, pos.z + cos(M_PI*dir/180), 0., 1., 0.);

    glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);
    // float red[4] = {0, 1, 0, 1};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, red);

	drawFloor();

    glPushMatrix();
        glTranslatef(waddle, 0.5, 0);
        drawAlien(walk, pinch, 45, true);
    glPopMatrix();

    float shadowMat[16]; 
    getShadowMat(shadowMat, lpos);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glMultMatrixf(shadowMat);
        glTranslatef(waddle, 0.5, 0);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        drawAlien(walk, pinch, 45, false);
    glPopMatrix();

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
	gluPerspective(50., 1., 1., 1000.);

    registerStaticAnimation((animation_infinite) {
        .value = &walk,
        .increment = 2.0,
        .to = 15.0,
        .from = -15.0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &pinch,
        .increment = 0.6,
        .to = 20.0,
        .from = -5.0,
        .type = ALTERNATE
    });

    sidewalk();
}

void timer(int value) {
    frame = value;
    value++;
    if (skeystates[GLUT_KEY_LEFT]) {
        dir += 5;
    } else if (skeystates[GLUT_KEY_RIGHT]) {
        dir -= 5;
    } else if (skeystates[GLUT_KEY_UP]) {
        pos.x += 0.5*sin(M_PI*dir/180);
        pos.z += 0.5*cos(M_PI*dir/180);
    } else if (skeystates[GLUT_KEY_DOWN]) {
        pos.x -= 0.5*sin(M_PI*dir/180);
        pos.z -= 0.5*cos(M_PI*dir/180);
    }
    animate(value);
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, value);
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
