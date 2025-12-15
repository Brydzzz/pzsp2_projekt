#pragma once

#include <algorithm>
#include <random>
#include <stdlib.h>
#include <vector>

inline int getRandomFromRange(int from, int to) {
    if (from == to) {
        return from;
    }
    return rand() % (to - from) + from;
}

inline std::vector<int>
createRandomizedPermutation(int to, std::mt19937 &randomDevice) {
    std::vector<int> result(to);
    for (int i = 0; i < to; i++) {
        result[i] = i;
    }

    shuffle(result.begin(), result.end(), randomDevice);
    return result;
}

inline std::mt19937 createRandomDevice() {
    std::random_device rd;
    std::mt19937 randomDevice(rd());
    return randomDevice;
}