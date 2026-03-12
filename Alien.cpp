#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>
#include <climits>
#include <GL/freeglut.h>
#include <math.h>
#include "Model.h"
#include "Animation.h"
#include "Utils.h"
#include "Global.h"
#include "loadTGA.h"

using namespace std;

bool skeystates[256] = {false};
bool keystates[256] = {false};

void drawEverything(bool color) {
    glPushMatrix();
        glTranslatef(30, 0, 0);
        drawPendulumRide(pendulum, alienGlobal, color);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-15, 0, -20);
        glRotatef(90, 0, 1, 0);
        drawCatapult(catapult, alienGlobal, color);
        glPushMatrix();
            glTranslatef(0, thrown.y, thrown.x);
            glRotatef(thrown.rotation, 1, 0, 0);
            drawAlien(alienGlobal, color);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.3, 0, -18);
            glRotatef(-90, 0, 1, 0);
            drawAlienQueue(color);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(12, 0, -20);
        drawLavaPit(color);
    glPopMatrix();
}

void display() {
	float lpos[4] = {0., 50., 0., 1.0};  //light's position
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
        glTranslatef(pos.x, pos.y, pos.z);
        drawSky();
    glPopMatrix();

    glPushMatrix();
        drawEverything(true);
    glPopMatrix();

    float shadowMat[16]; 
    getShadowMat(shadowMat, lpos);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glMultMatrixf(shadowMat);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        drawEverything(false);
    glPopMatrix();

    glutSwapBuffers();
}

void loadTexture(size_t index, char *path, GLint mode) {
    glBindTexture(GL_TEXTURE_2D, texIds[index]);
    loadTGA(path);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE, GL_TEXTURE_ENV_MODE, mode);
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

    glGenTextures(4, texIds);
    loadTexture(0, (char*)"resources/redsoil.tga", GL_MODULATE);
    loadTexture(1, (char*)"resources/bricks.tga", GL_MODULATE);
    loadTexture(2, (char*)"resources/nightsky.tga", GL_REPLACE);
    loadTexture(3, (char*)"resources/woodplanks.tga", GL_MODULATE);

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
