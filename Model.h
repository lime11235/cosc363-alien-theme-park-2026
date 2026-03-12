#ifndef MODEL_H
#define MODEL_H

#include "Utils.h"

void drawAlien(alienState state, bool color);
void drawPendulumRide(pendulumState state, alienState astate, bool color);
void drawCatapult(catapultState state, alienState astate, bool color);
void drawSky(void);
void drawFloor(void);
void drawLavaPit(bool color);
void drawAlienQueue(bool color);

#endif // !MODEL_H
