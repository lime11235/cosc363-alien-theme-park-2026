#include "Utils.h"
#include "Animation.h"
#include "math.h"
#include <functional>
#include <stdlib.h>
#include "Global.h"

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
static function<void(int)> winch;
static function<void(int)> load;
static function<void(int)> loadfinish;

static void throwAlien(int frame) {
    catapult.occupied = false;
    registerDynamicAnimation((animation) {
        .start_ts = frame,
        .end_ts = frame + 20,
        .animation_curve = NULL,
        .from = thrown.start[0],
        .to = thrown.end[0],
        .value = &(thrown.x),
        .callback = winch
    });
    registerDynamicAnimation((animation) {
        .start_ts = frame + 1,
        .end_ts = frame + 20,
        .animation_curve = easeInQuad,
        .from = thrown.start[1],
        .to = thrown.end[1],
        .value = &(thrown.y),
        .callback = NULL
    });
    registerDynamicAnimation((animation) {
        .start_ts = frame + 2,
        .end_ts = frame + 20,
        .animation_curve = NULL,
        .from = -90,
        .to = 180,
        .value = &(thrown.rotation),
        .callback = NULL
    });
}

void initCatapult(catapultState *catapult) {

    fire = [catapult] (int frame) {
        catapult->occupied = true;
        catapult->moving = false;
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 10,
            .animation_curve = easeInQuad,
            .from = 0,
            .to = 90,
            .value = &(catapult->angle),
            .callback = throwAlien
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
        catapult->moving = true;
        catapult->grab = true;
        registerDynamicAnimation((animation) {
            .start_ts = frame,
            .end_ts = frame + 30,
            .animation_curve = NULL,
            .from = 0,
            .to = 1.2,
            .value = &(catapult->load),
            .callback = loadfinish
        });
        registerDynamicAnimation((animation) {
            .start_ts = frame+1,
            .end_ts = frame + 30,
            .animation_curve = NULL,
            .from = 4.0,
            .to = 0,
            .value = &(catapult->qoffset),
            .callback = NULL
        });
    };

    loadfinish = [catapult] (int frame) {
        catapult->occupied = true;
        catapult->grab = false;
        registerDynamicAnimation((animation) {
            .start_ts = frame + 10,
            .end_ts = frame + 40,
            .animation_curve = NULL,
            .from = 1,
            .to = 0,
            .value = &(catapult->load),
            .callback = fire
        });
    };

    fire(0);
}

