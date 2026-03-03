#ifndef ANIMATION_H
#define ANIMATION_H

typedef struct {
    int start_ts;
    int end_ts;
    float (*animation_curve)(float t);
    float from;
    float to;
    float *value;
} animation;

typedef enum {
    RESTART,
    ALTERNATE
} animation_type;

typedef struct {
    float *value;
    float increment;
    float to;
    float from;
    animation_type type;
} animation_infinite;

void register_animation(animation_infinite animation);

void animate(void);

#endif // !ANIMATION_H
