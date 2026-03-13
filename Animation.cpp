#include <vector>
#include <set>
#include <math.h>
#include "Animation.h"
#include "Global.h"

using namespace std;

static vector<animation_infinite> static_animations;
static set<animation> dynamic_animations;

void registerStaticAnimation(animation_infinite animation) {
    static_animations.push_back(animation);
}

void animate(int value) {
    for (int i = 0; i < static_animations.size(); i++) {
        auto a = static_animations[i];
        *(a.value) += a.increment;
        if (*(a.value) >= a.to || (a.type == ALTERNATE && *(a.value) <= a.from)) {
            switch (a.type) {
                case ALTERNATE:
                    static_animations[i].increment *= -1;
                    break;
                case RESTART:
                    *(a.value) = a.from;
                    break;
            }
        }
    }

    vector<animation> removal;

    for (auto a : dynamic_animations) {
        if (value >= a.end_ts) {
            *(a.value) = a.to;
            removal.push_back(a);
            continue;
        }
        int duration = a.end_ts - a.start_ts;
        float t = (float)(value-a.start_ts) / (float)duration;
        float y = 0;

        if (a.animation_curve == NULL) {
            // Linear curve
            y = t;
        } else {
            y = a.animation_curve(t);
        }

        float range = a.to - a.from;
        *(a.value) = range*y + a.from;
    }

    for (auto a : removal) {
        if (a.callback)
            a.callback(value);
        dynamic_animations.erase(a);
    }
}

void registerDynamicAnimation(animation animation) {
    dynamic_animations.insert(animation);
}

void initializeAnimations(void) {
    registerStaticAnimation((animation_infinite) {
        .value = &(alienGlobal.walk),
        .increment = 2.0,
        .to = 15.0,
        .from = -15.0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(alienGlobal.pinch),
        .increment = 0.6,
        .to = 20.0,
        .from = -5.0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(pendulum.gravity),
        .increment = 0.05,
        .to = 10,
        .from = 0,
        .type = ALTERNATE
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(pendulum.subangle),
        .increment = 7,
        .to = 360,
        .from = 0,
        .type = RESTART
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(sun.rotation),
        .increment = 0.5,
        .to = 360,
        .from = 0,
        .type = RESTART
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(sun.height),
        .increment = 0.01,
        .to = 2*M_PI,
        .from = 0,
        .type = RESTART
    });

    registerStaticAnimation((animation_infinite) {
        .value = &(sun.scale),
        .increment = 0.01,
        .to = 3,
        .from = 0,
        .type = RESTART
    });
}

float easeOutElastic(float t) {
    const float c4 = (2 * M_PI) / 3;

    return t == 0 ? 0
      : t == 1 ? 1
      : pow(2, -10 * t) * sin((t * 10 - 0.75) * c4) + 1;
}

float easeInOutQuart(float t) {
    return t < 0.5 ? 8 * t * t * t * t : 1 - pow(-2 * t + 2, 4) / 2;
}

float easeOutBounce(float t) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (t < (1 / d1)) {
        return n1 * t * t;
    } else if (t < (2 / d1)) {
        return n1 * (t - 1.5 / d1) * (t - 1.5 / d1) + 0.75;
    } else if (t < (2.5 / d1)) {
        return n1 * (t - 2.25 / d1) * (t - 2.25 / d1) + 0.9375;
    } else {
        return n1 * (t - 2.625 / d1) * (t - 2.625 / d1) + 0.984375;
    }
}

float easeInQuad(float t) {
    return t*t;
}
