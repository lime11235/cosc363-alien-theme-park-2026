#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <algorithm>
#include <climits>
#include <GL/freeglut.h>
#include <math.h>
#include "Model.h"
#include "Animation.h"
#include "Utils.h"
#include "Global.h"

using namespace std;

bool skeystates[256] = {false};
bool keystates[256] = {false};

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
	float lpos[4] = {5., 50., 0., 1.0};  //light's position
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pos.x, pos.y, pos.z, 
        pos.x + sin(M_PI*pos.dir/180)*cos(M_PI*pos.dirv/180), 
        pos.y + sin(M_PI*pos.dirv/180), 
        pos.z + cos(M_PI*pos.dir/180)*cos(M_PI*pos.dirv/180), 
        0., 1., 0.);

    if (!wireframe)
        glEnable(GL_LIGHTING);
    else 
        glDisable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);
    // float red[4] = {0, 1, 0, 1};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, red);

	drawFloor();

    glPushMatrix();
        glTranslatef(-20, 0, 0);
        drawPendulumRide(pendulum, alienGlobal, true);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -20);
        drawCatapult(catapult, alienGlobal, true);
        glTranslatef(0, thrown.y, thrown.x);
        glRotatef(thrown.rotation, 1, 0, 0);
        drawAlien(alienGlobal, true);
    glPopMatrix();

    float shadowMat[16]; 
    getShadowMat(shadowMat, lpos);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glMultMatrixf(shadowMat);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glPushMatrix();
            glTranslatef(-20, 0, 0);
            drawPendulumRide(pendulum, alienGlobal, false);
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
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
        .value = &(alienGlobal.walk),
        .increment = 2.0,
        .to = 15.0,
        .from = -15.0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(alienGlobal.pinch),
        .increment = 0.6,
        .to = 20.0,
        .from = -5.0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(pendulum.gravity),
        .increment = 0.05,
        .to = 10,
        .from = 0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(pendulum.subangle),
        .increment = 7,
        .to = 360,
        .from = 0,
        .type = RESTART
    });

    initCatapult(&catapult);
}

void timer(int value) {
    frame = value;
    value++;
    if (skeystates[GLUT_KEY_LEFT] || keystates['a']) {
        pos.x += 0.5*sin(M_PI*(pos.dir + 90)/180);
        pos.z += 0.5*cos(M_PI*(pos.dir + 90)/180);
    } 
    if (skeystates[GLUT_KEY_RIGHT] || keystates['d']) {
        pos.x -= 0.5*sin(M_PI*(pos.dir + 90)/180);
        pos.z -= 0.5*cos(M_PI*(pos.dir + 90)/180);
    } 
    if (skeystates[GLUT_KEY_UP] || keystates['w']) {
        pos.x += 0.5*sin(M_PI*pos.dir/180);
        pos.z += 0.5*cos(M_PI*pos.dir/180);
    } 
    if (skeystates[GLUT_KEY_DOWN] || keystates['s']) {
        pos.x -= 0.5*sin(M_PI*pos.dir/180);
        pos.z -= 0.5*cos(M_PI*pos.dir/180);
    } 
    if (keystates[' ']) {
        pos.y += 0.5;
    }
    if (skeystates[GLUT_KEY_SHIFT_L]) {
        pos.y -= 0.5;
    }
    animate(value);
    updatePendulum(&pendulum);
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, value);
}

void mouseMove(int xpos, int ypos) {
    static float lastX = 300, lastY = 300;
    if (pos.firstMouse) {
        lastX = xpos;
        lastY = ypos;
        pos.firstMouse = false;
    }

    float xoffset = (lastX - xpos) * 0.1;
    float yoffset = (lastY - ypos) * 0.15;
    lastX = xpos;
    lastY = ypos;

    pos.dir += xoffset;
    pos.dirv += yoffset;

    pos.dirv = min(pos.dirv, 90.0f);
    pos.dirv = max(pos.dirv, -90.0f);
}

void mouseClick(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        pos.firstMouse = true;
    }
}

void specialUp(int key, int x, int y) {
    skeystates[key] = false;
}

void specialDown(int key, int x, int y) {
    skeystates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keystates[key] = false;
}

void keyboardDown(unsigned char key, int x, int y) {
    if (key == 'q') 
        wireframe = !wireframe;
    keystates[key] = true;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Alien Theme Park");
    glutIgnoreKeyRepeat(1);
    initialize();
    
    glutDisplayFunc(display);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(1000/FPS, timer, 0);
    glutMotionFunc(mouseMove);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
}
