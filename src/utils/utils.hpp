#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

int getRandomFromRange(int from, int to) {
    if (from == to) {
        return from;
    }
    return rand() % (to - from) + from;
}

#endif