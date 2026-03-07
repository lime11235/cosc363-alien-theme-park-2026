#ifndef ANIMATION_H
#define ANIMATION_H

#include <functional>

using namespace std;

struct animation {
    int start_ts;
    int end_ts;
    float (*animation_curve)(float t);
    float from;
    float to;
    float *value;
    function<void(int)> callback;
    bool operator<(const animation& other) const {
        return start_ts < other.start_ts;
    }
};

typedef struct animation animation;

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

void registerStaticAnimation(animation_infinite animation);
void registerDynamicAnimation(animation animation);

void animate(int value);

float easeOutElastic(float t);
float easeInOutQuart(float t);
float easeOutBounce(float t);
float easeInQuad(float t);

#endif // !ANIMATION_H
