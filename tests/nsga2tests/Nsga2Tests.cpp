//
// Created by tomek on 12/14/25.
//

#include "../../src/algorithms/NSGA2.h"

#include <gtest/gtest.h>

#include <cmath>

std::vector<float> mock_target(int number) {
    return std::vector{static_cast<float>(std::pow(number, 2)),
                       static_cast<float>(std::pow(number, 3)),
                       static_cast<float>(std::pow(number, 4))};
}

std::vector<int> mock_population(int number) { return {1, 1 + number, 3, 0}; }

float distance(int n1, int n2) { return std::abs(n1 - n2); }

template <typename T>
class NSGA2Wrapper : public NSGA2<T> {
  public:
    NSGA2Wrapper() {}
    std::vector<std::vector<T>>
    wrap_sort_nondominated_algorithm(std::vector<T> &population,
                                     target_function<T> target) {
        return this->sort_nondominated_algorithm(population, target);
    }

    std::vector<float> wrap_crowding_distance(std::vector<T> &population,
                                              target_function<T> target) {
        return this->crowding_distance(population, target);
    }
};

// data common for multiple tests
std::vector<int> pop = {1, -2, 3, 0};
std::vector<int> pop2 = {1, -2, -3, -4, -5, -6, -7, -8, -9, -10, 3, 0};
const float inf = std::numeric_limits<float>::infinity();

// quick sanity check making sure the library works as intended
TEST(NSGA2Tests, lib_test) {
    target_function<float> target = mock_target;
    auto svalues = target(pop[1]);
    auto tvalues = target(pop[2]);
    auto spoint = pareto::point<float, 3>(svalues.begin(), svalues.end());
    auto tpoint = pareto::point<float, 3>(tvalues.begin(), tvalues.end());
    pareto::point<float, 3> point({9, 27, 81});
    ASSERT_EQ(tpoint, point);
    ASSERT_EQ(spoint.strongly_dominates(tpoint), true);
}

TEST(NSGA2Tests, test_nondominated_sorting_algorihtm) {
    NSGA2Wrapper<int> nsga2;
    auto result = nsga2.wrap_sort_nondominated_algorithm(pop, mock_target);
    std::vector<std::vector<int>> answer = {{-2, 0}, {1}, {3}};

    // the exact order in each pareto front is not important, so each pareto
    // front is begin sorted by value to chekc against the answer
    for (auto &v : result)
        sort(v.begin(), v.end());

    for (auto &v : answer)
        sort(v.begin(), v.end());

    ASSERT_EQ(result, answer);
}

TEST(NSGA2Tests, test_nondominated_sorting_algorihtm_2) {
    NSGA2Wrapper<int> nsga2;
    auto result = nsga2.wrap_crowding_distance(pop2, mock_target);
    std::vector<float> answer = {0.0678901598, 0.114290163, 0.111027755,
                                 0.309823543,  0.452003896, 0.629868746,
                                 0.848218083,  1.11185193,  1.42557037,
                                 inf,          inf,         inf};

    ASSERT_EQ(answer.size(), result.size());
    const float eps = 0.00001;
    for (int i = 0; i < (int)answer.size(); i++) {
        ASSERT_GE(result[i], answer[i] - eps);
        ASSERT_LE(result[i], answer[i] + eps);
    }
}
