#include "Model.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

using namespace std;

const float langle = 20;
const float cangle = 45;

static void drawLeg(float angle) {
    glPushMatrix();
    GLUquadric *q = gluNewQuadric();
    glRotatef(angle + 15.0, 1, 0, 0);
    gluCylinder(q, 0.18, 0.14, 0.6, 36, 1);
    gluQuadricDrawStyle(q, GLU_FILL);
    glTranslatef(0, 0, 0.55);
    glutSolidSphere(0.14, 36, 12);
    glRotatef(angle + 80.0, 1, 0, 0);
    glutSolidCone(0.16, 0.6, 36, 1);
    glPopMatrix();
}

void drawClaws(void) {
    glPushMatrix();
    GLUquadric *q = gluNewQuadric();
    gluCylinder(q, 0.18, 0.14, 0.6, 36, 1);
    gluQuadricDrawStyle(q, GLU_FILL);
    glTranslatef(0, 0, 0.55);
    glutSolidSphere(0.14, 36, 12);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(0.14, 0.7, 36, 1);
    glRotatef(-45, 0, 1, 0);
    glScalef(1.6, 1, 1);
    glRotatef(10, 0, 1, 0);
    glScalef(1.2, 1, 1);
    glRotatef(-10, 0, 1, 0);
    gluCylinder(q, 0.1, 0.15, 0.6, 36, 1);
    glTranslatef(0, 0, 0.6);
    glutSolidSphere(0.15, 36, 12);
    glRotatef(30, 0, 1, 0);
    glTranslatef(0, 0, 0.04);
    glutSolidCone(0.15, 0.27, 36, 1);
    glPopMatrix();
}

void drawAlien(float leg_angle, float claw_pinch, float claw_raise) {
    glPushMatrix();
    glColor3d(0.898, 0.239, 0.114);

    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glScalef(1.0, 0.6, 1.0);
    glutSolidSphere(1.0, 36, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin((90-langle)*M_PI/180), 0.18, 0.8*cos((90-langle)*M_PI/180));
    glRotatef((90-langle), 0, 1, 0);
    drawLeg(leg_angle);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin((90+langle)*M_PI/180), 0.18, 0.8*cos((90+langle)*M_PI/180));
    glRotatef((90+langle), 0, 1, 0);
    drawLeg(-leg_angle);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin(-(90-langle)*M_PI/180), 0.18, 0.8*cos(-(90-langle)*M_PI/180));
    glRotatef(-(90-langle), 0, 1, 0);
    drawLeg(-leg_angle);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin(-(90+langle)*M_PI/180), 0.18, 0.8*cos(-(90+langle)*M_PI/180));
    glRotatef(-(90+langle), 0, 1, 0);
    drawLeg(leg_angle);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin(cangle*M_PI/180), 0.8*0.6*sin(claw_raise) + 0.18, 0.8*cos(cangle*M_PI/180));
    glRotatef(-claw_raise, 1, 0, 0);
    glRotatef(cangle-claw_pinch, 0, 1, 0);
    glScalef(-1, 1, 1);
    drawClaws();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8*sin(-cangle*M_PI/180), 0.8*0.6*sin(claw_raise) + 0.18, 0.8*cos(-cangle*M_PI/180));
    glRotatef(-claw_raise, 1, 0, 0);
    glRotatef(-cangle+claw_pinch, 0, 1, 0);
    drawClaws();
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.02, 0.02, 0.02);
    glTranslatef(0.4, 0.35, 0.8);
    glutSolidSphere(0.15, 12, 4);
    glRotatef(20, 0, 1, 0);
    glTranslatef(0.07, 0.07, 0.07);
    glColor3d(1, 1, 1);
    glutSolidSphere(0.05, 12, 4);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.02, 0.02, 0.02);
    glTranslatef(-0.4, 0.35, 0.8);
    glRotatef(-20, 0, 1, 0);
    glutSolidSphere(0.15, 12, 4);
    glTranslatef(0.07, 0.07, 0.07);
    glColor3d(1, 1, 1);
    glutSolidSphere(0.05, 12, 4);
    glPopMatrix();

    glPopMatrix();
}

