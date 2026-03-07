#include "Utils.h"
#include "Animation.h"
#include "math.h"
#include <functional>
#include <iostream>
#include <stdlib.h>

using namespace std;

void getShadowMat(float matrix[16], float lpos[4]) {
    float calc[16] = {
        lpos[1], 0, 0, 0,
        -lpos[0], 0, -lpos[2], -1,
        0, 0, lpos[1], 0,
        0, 0, 0, lpos[1]
    };

    for (size_t i = 0; i < 16; i++) {
        matrix[i] = calc[i];
}
}

void updatePendulum(pendulumState *pendulum) {
    const float omega = pendulum->gravity/15;
    pendulum->velocity += omega*sin(M_PI * (pendulum->angle + 180) / 180) * 0.03;
    pendulum->angle += 180 * pendulum->velocity / M_PI;
}

static function<void(int)> fire;
static function<void(int)> wait;
static function<void(int)> winch;
static function<void(int)> load;
static function<void(int)> loadfinish;

void initCatapult(catapultState *catapult) {

    fire = [catapult] (int frame) {
        catapult->occupied = true;
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 6,
            .animation_curve = easeInQuad,
            .from = 0,
            .to = 90,
            .value = &(catapult->angle),
            .callback = wait
        });
    };

    wait = [catapult] (int frame) {
        catapult->occupied = false;
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 30,
            .animation_curve = NULL,
            .from = 90,
            .to = 90,
            .value = &(catapult->angle),
            .callback = winch
        });
    };

    winch = [catapult] (int frame) {
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 30,
            .animation_curve = NULL,
            .from = 90,
            .to = 0,
            .value = &(catapult->angle),
            .callback = load
        });
    };

    load = [catapult] (int frame) {
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 15,
            .animation_curve = NULL,
            .from = 0,
            .to = 0,
            .value = &(catapult->angle),
            .callback = loadfinish
        });
    };

    loadfinish = [catapult] (int frame) {
        catapult->occupied = true;
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 15,
            .animation_curve = NULL,
            .from = 0,
            .to = 0,
            .value = &(catapult->angle),
            .callback = fire
        });
    };

    fire(0);
}

