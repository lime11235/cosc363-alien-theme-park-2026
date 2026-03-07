#ifndef UTILS_H
#define UTILS_H

typedef struct {
    float pinch;
    float walk;
    float raise;
} alienState;

typedef struct {
    float dir;
    float dirv;
    float x;
    float y;
    float z;
    bool firstMouse;
} position;

typedef struct {
    float angle;
    float subangle;
    float velocity;
    float gravity;
} pendulumState;

typedef struct {
    float angle;
    bool occupied;
} catapultState;

void getShadowMat(float matrix[16], float lpos[4]);
void updatePendulum(pendulumState *pendulum);
void initCatapult(catapultState *catapult);

#endif // !UTILS_H
