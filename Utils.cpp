#include "Utils.h"
#include <stdlib.h>

void getShadowMat (float matrix[16], float lpos[4]) {
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