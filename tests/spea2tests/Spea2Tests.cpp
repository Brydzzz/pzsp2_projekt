//
// Created by tomek on 12/14/25.
//

#include "../../src/algorithms/SPEA2.h"

#include <gtest/gtest.h>

#include <cmath>

std::vector<float> mock_target(int number) {
    return std::vector{static_cast<float>(std::pow(number, 2)),
                       static_cast<float>(std::pow(number, 3)),
                       static_cast<float>(std::pow(number, 4))};
}

std::vector<int> mock_population(int number) {
    return {1, 2, 3, 4};
}


TEST(SPEA2Tests, testBasicUsage) {
    auto spea2 = SPEA2<int>();
    spea2.solve(mock_target, mock_population(4), );
}
