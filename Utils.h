#ifndef UTILS_H
#define UTILS_H

typedef struct {
    float pinch;
    float walk;
    float raise;
} alienState;

typedef struct {
    float dir;
    float x;
    float y;
    float z;
} position;

typedef struct {
    float angle;
    float subangle;
    float velocity;
    float gravity;
} pendulumState;

void getShadowMat(float matrix[16], float lpos[4]);
void updatePendulum(pendulumState *pendulum);

#endif // !UTILS_H
