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
    float load;
    bool moving;
    bool grab; 
    float qoffset;
} catapultState;

typedef struct {
    float rotation;
    float height;
    float colors[3][3];
    float scale;
} sunState;

typedef struct {
    float x;
    float y;
    float rotation;
    float start[2];
    float end[2];
} thrownAlien;

void getShadowMat(float matrix[16], float lpos[4]);
void updatePendulum(pendulumState *pendulum);
void initCatapult(catapultState *catapult);
void getSunColor(float ans[4]);

#endif // !UTILS_H
