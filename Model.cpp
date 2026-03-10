#include "Model.h"
#include "Utils.h"
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
        gluCylinder(q, 0.18, 0.14, 0.6, 8, 1);
        gluQuadricDrawStyle(q, GLU_FILL);
        glTranslatef(0, 0, 0.55);
        glutSolidSphere(0.14, 6, 2);
        glRotatef(angle + 80.0, 1, 0, 0);
        glutSolidCone(0.16, 0.6, 8, 1);
    glPopMatrix();
}

void drawClaws(void) {
    glPushMatrix();
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q, 0.18, 0.14, 0.6, 6, 1);
        gluQuadricDrawStyle(q, GLU_FILL);
        glTranslatef(0, 0, 0.55);
        glutSolidSphere(0.14, 6, 2);
        glRotatef(90, 0, 1, 0);
        glutSolidCone(0.14, 0.7, 6, 1);
        glRotatef(-45, 0, 1, 0);
        glScalef(1.6, 1, 1);
        glRotatef(10, 0, 1, 0);
        glScalef(1.2, 1, 1);
        glRotatef(-10, 0, 1, 0);
        gluCylinder(q, 0.1, 0.15, 0.6, 6, 1);
        glTranslatef(0, 0, 0.6);
        glutSolidSphere(0.15, 6, 2);
        glRotatef(30, 0, 1, 0);
        glTranslatef(0, 0, 0.04);
        glutSolidCone(0.15, 0.27, 6, 1);
    glPopMatrix();
}

void drawAlien(alienState state, bool color) {
    const float leg_angle = state.walk;
    const float claw_pinch = state.pinch;
    const float claw_raise = state.raise;
    glPushMatrix();
        if (color) {
            const float orange[4] = {0.898, 0.239, 0.114, 1};
            const float white[4] = {1, 1, 1, 1};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, orange);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
            glColor3fv(orange);
        }

        glPushMatrix();
            glTranslatef(0, 0.3, 0);
            glScalef(1.0, 0.6, 1.0);
            glutSolidSphere(1.0, 12, 4);
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
            glTranslatef(0.8*sin(cangle*M_PI/180), 0.8*0.6*sin(claw_raise*M_PI/180) + 0.18, 0.8*cos(cangle*M_PI/180));
            glRotatef(-claw_raise, 1, 0, 0);
            glRotatef(cangle-claw_pinch, 0, 1, 0);
            glScalef(-1, 1, 1);
            drawClaws();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.8*sin(-cangle*M_PI/180), 0.8*0.6*sin(claw_raise*M_PI/180) + 0.18, 0.8*cos(-cangle*M_PI/180));
            glRotatef(-claw_raise, 1, 0, 0);
            glRotatef(-cangle+claw_pinch, 0, 1, 0);
            drawClaws();
        glPopMatrix();

        glPushMatrix();
            if (color)
                glColor3d(0.02, 0.02, 0.02);
            glTranslatef(0.4, 0.35, 0.8);
            glutSolidSphere(0.15, 6, 2);
            glRotatef(20, 0, 1, 0);
            glTranslatef(0.07, 0.07, 0.07);
            if (color)
                glColor3d(1, 1, 1);
            glutSolidSphere(0.05, 6, 2);
        glPopMatrix();

        glPushMatrix();
            if (color)
                glColor3d(0.02, 0.02, 0.02);
            glTranslatef(-0.4, 0.35, 0.8);
            glRotatef(-20, 0, 1, 0);
            glutSolidSphere(0.15, 6, 2);
            glTranslatef(0.07, 0.07, 0.07);
            if (color)
                glColor3d(1, 1, 1);
            glutSolidSphere(0.05, 6, 2);
        glPopMatrix();
    glPopMatrix();
}

static void drawCylinder(float radius, float height) {
    GLUquadric *q; 
    q = gluNewQuadric();
    gluCylinder(q, radius, radius, height, 6, 1);
    gluQuadricDrawStyle(q, GLU_FILL);
}

void drawPendulumRide(pendulumState state, alienState astate, bool color) {
    const float base[4] = {0.2, 0.2, 0.2, 0};
    const float black[4] = {0, 0, 0, 1};
    glPushMatrix();
        if (color) {
            glColor3fv(base);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
        }
        glTranslatef(0, 15, 0);
        glPushMatrix();
            glRotatef(state.angle, 1, 0, 0);
            glPushMatrix();
                glScalef(2, 13, 2);
                glRotatef(45, 0, 1, 0);
                glutSolidOctahedron();
            glPopMatrix();
            glTranslatef(0, -13, 0);
            glRotatef(state.subangle, 0, 1, 0);
            glPushMatrix();
                glRotatef(-90, 1, 0, 0);
                glutSolidCone(0.6, 4, 6, 1);
            glPopMatrix();
            glutSolidCube(1.2);
            for (int a = 0; a < 361; a += 90) {
                glPushMatrix();
                    glRotatef(a, 0, 1, 0);
                    drawCylinder(0.5, 4);
                    glTranslatef(0, 0, 4);
                    GLUquadric *q = gluNewQuadric();
                    gluCylinder(q, 0.5, 0.7, 0.3, 24, 2);
                    glTranslatef(0, 0, 1.02);
                    glRotatef(-90, 1, 0, 0);
                    drawAlien(astate, color);
                    // reset color
                    if (color) {
                        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
                        glColor3fv(base);
                    }
                glPopMatrix();
            }
        glPopMatrix();

        glTranslatef(1, 0, 0);
        glPushMatrix();
            glRotatef(70, 0, 1, 0);
            glRotatef(60, 1, 0, 0);
            drawCylinder(1, 18);
        glPopMatrix();
        glPushMatrix();
            glRotatef(110, 0, 1, 0);
            glRotatef(60, 1, 0, 0);
            drawCylinder(1, 18);
        glPopMatrix();
        glTranslatef(-2, 0, 0);
        glPushMatrix();
            glRotatef(-70, 0, 1, 0);
            glRotatef(60, 1, 0, 0);
            drawCylinder(1, 18);
        glPopMatrix();
        glPushMatrix();
            glRotatef(-110, 0, 1, 0);
            glRotatef(60, 1, 0, 0);
            drawCylinder(1, 18);
        glPopMatrix();
    glPopMatrix();
}

static void drawAlienLoader(catapultState state, alienState astate, bool color) {
    glTranslatef(0, 0.4, state.load*5.5);
    glRotatef(-90, 0, 1, 0);
    drawAlien((alienState) {
        .pinch = astate.pinch,
        .walk = state.moving ? astate.walk : 0,
        .raise = 70.0f * state.load
    }, color);
    if (state.grab) {
        glRotatef(-70*state.load, 1, 0, 0);
        glTranslatef(0, 0, 2);
        glRotatef(-70*state.load, 1, 0, 0);
        glRotatef(-90, 0, 1, 0);
        drawAlien(astate, color);
    }
}

void drawCatapult(catapultState state, alienState astate, bool color) {
    const float brown[4] = {0.5882, 0.294, 0, 1};
    const float black[4] = {0, 0, 0, 1};
    if (color) {
        glColor3fv(brown);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
    }
    glPushMatrix();
        glPushMatrix();
            glTranslatef(-4, 1, -4);
            glScalef(1, 2, 20);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(4, 1, -4);
            glScalef(1, 2, 20);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-4, 4.5, 0);
            glScalef(1, 5, 2);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(4, 4.5, 0);
            glScalef(1, 5, 2);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, 6, 0.5);
            glScalef(8, 2, 0.5);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, 1, 0);
            glRotatef(90, 0, 1, 0);
            drawCylinder(0.5, 4);
            glRotatef(180, 0, 1, 0);
            drawCylinder(0.5, 4);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, 1, 0);
            glRotatef(state.angle, 1, 0, 0);
            glTranslatef(0, 0, -11);

            glRotatef(-90, 1, 0, 0);
            GLUquadric *q = gluNewQuadric();
            gluCylinder(q, 0.5, 1.0, 0.7, 12, 1);
            glRotatef(90, 1, 0, 0);

            if (state.occupied) {
                glPushMatrix();
                    glTranslatef(0, 1.2, 0);
                    glRotatef(180, 1, 0, 0);
                    drawAlien(astate, color);
                    glTranslatef(0, -1.2, 0);
                glPopMatrix();
            }

            if (color) {
                glColor3fv(brown);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
            }

            glTranslatef(0, 0, 5);

            glScalef(1.5, 0.5, 12);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(2, 0, -18);
            drawAlienLoader(state, astate, color);
        glPopMatrix();
    glPopMatrix();
}
