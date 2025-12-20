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
    number++;
    return {1, 2, 3, 0};
}

template <typename T>
class SPEA2Wrapper : SPEA2<T> {
public:
    std::vector<int> wrap_calculate_strength(
        target_function<T> target, std::vector<T> pop) {

        return this->calculate_strength(target, pop);
    }
};


// TEST(SPEA2Tests, testBasicUsage) {
//     auto spea2 = SPEA2<int>();
//     spea2.solve(mock_target, mock_population(4), );
// }


TEST(SPEA2Tests, test_calculate_strength) {
    auto spea2 = SPEA2Wrapper<int>();
    std::vector<int> pop = {1, -2, 3, 0};
    std::vector strength = spea2.wrap_calculate_strength(mock_target, pop);
    std::vector results = {1, 1, 0, 2};
    ASSERT_TRUE(std::equal(strength.begin(), strength.end(), results.end()));
}
