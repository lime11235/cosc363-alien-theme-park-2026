#include "Global.h"

int FPS = 30;
int frame = 0;
bool wireframe = false;
GLuint texIds[5];

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

sunState sun = {
    .rotation = 0,
    .height = 0,
    .colors = {
        {0.9294, 0.6039, 0.6471},
        {0.4157, 0.6392, 0.6706},
        {0.7686, 0.5961, 0.8902},
    },
    .scale = 0
};

thrownAlien thrown = {
    .x = 30,
    .y = 0,
    .rotation = -90,
    .start = {0, 12},
    .end = {30, 0},
};
