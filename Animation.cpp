#include <vector>
#include "Animation.h"

using namespace std;

vector<animation_infinite> animations;

void register_animation(animation_infinite animation) {
    animations.push_back(animation);
}

void animate(void) {
    for (int i = 0; i < animations.size(); i++) {
        auto a = animations[i];
        *(a.value) += a.increment;
        if (*(a.value) >= a.to || (a.type == ALTERNATE && *(a.value) <= a.from)) {
            switch (a.type) {
                case ALTERNATE:
                    animations[i].increment *= -1;
                    break;
                case RESTART:
                    *(a.value) = a.from;
                    break;
            }
        }
    }
}
