#include "Global.h"

int FPS = 30;
int frame = 0;
bool wireframe = false;
GLuint texIds[4];

position pos = {
    .dir = 180,
    .dirv = 0,
    .x = 0,
    .y = 15,
    .z = 20,
    .firstMouse = true
};

alienState alienGlobal = {
    .pinch = 0,
    .walk = -10,
    .raise = 0,
};

pendulumState pendulum = {
    .angle = 40,
    .subangle = 0,
    .velocity = 0.0625,
    .gravity = 0
};

catapultState catapult = {
    .angle = 0,
    .occupied = true,
    .load = 0,
    .moving = false,
    .grab = false,
    .qoffset = 0
};

thrownAlien thrown = {
    .x = 30,
    .y = 0,
    .rotation = -90,
    .start = {0, 12},
    .end = {30, 0},
};
