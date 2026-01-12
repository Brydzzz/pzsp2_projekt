//
// Created by tomek on 12/14/25.
//

#include <cmath>
#include <gtest/gtest.h>

#include "../../src/algorithms/NSGA2.h"
#include "../../src/utils/utils.hpp"

std::vector<float> mock_target(int number) {
    return std::vector{static_cast<float>(std::pow(number, 2)),
                       static_cast<float>(std::pow(number, 3)),
                       static_cast<float>(std::pow(number, 4))};
}

std::vector<int> mock_population(int number) {
    std::vector<int> out;
    for (int i = 0; i < number; i++) {
        out.push_back(getRandomFromRange(0, 100));
    }
    return out;
}

std::vector<int> mock_mutation(const std::vector<int> &population,
                               std::vector<float> params) {
    std::vector<int> result(population);
    params.push_back(-1);
    if (population[0] < params[0]) {
        return population;
    }
    // float mutation_possibility = 0;
    for (int i = 0; i < (int)result.size(); i++) {
        result[i] = std::max(result[i] - 1, 0);
    }
    return result;
}

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

    std::vector<T> wrap_select_best(std::vector<T> &population, int best_size,
                                    target_function<T> target) {
        return this->select_best(population, best_size, target);
    }
};

// data common for multiple tests
std::vector<int> pop = {1, -2, 3, 0};
std::vector<int> pop2 = {1, -2, -3, -4, -5, -6, -7, -8, -9, -10, 3, 0};
const float inf = std::numeric_limits<float>::infinity();
NSGA2Wrapper<int> nsga2;

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

TEST(NSGA2Tests, test_select_best) {
    std::vector<std::vector<int>> fronts =
        nsga2.wrap_sort_nondominated_algorithm(pop2, mock_target);
    std::vector<std::vector<int>> answer1 = {
        {-2, -3, -4, -5, -6, -7, -8, -9, -10, 0}, {1}, {3}};
    ASSERT_EQ(fronts, answer1);

    std::vector<float> distances =
        nsga2.wrap_crowding_distance(answer1[0], mock_target);
    std::vector<float> answer2 = {0.1251, 0.2,         0.312399983, 0.456,
                                  0.6356, 0.855999947, 1.122,       1.4384,
                                  inf,    inf};
    ASSERT_EQ(distances, answer2);

    std::vector<int> best =
        nsga2.wrap_select_best(pop2, (int)pop2.size() / 2, mock_target);
    std::vector<int> answer3 = {0, -10, -9, -8, -7, -6};
    ASSERT_EQ(best, answer3);
}

TEST(NSGA2Tests, test_solve) {
    srand(0);
    std::vector<float> params = {};

    // the chance of every part of result not being sub 50 is less than:
    // ((1/2)^5)^995, thus we can assume, that each value in result is always
    // less then 50
    std::vector<int> result = nsga2.solve(5, 1000, mock_target, mock_mutation,
                                          mock_population, params);
    for (auto v : result) {
        ASSERT_LT(v, 50);
    }
}

// TEST(NSGA2Tests, test_solve2) {
//     srand(11);
//     std::vector<float> params = {};

//     std::vector<int> result = nsga2.solve2(5, 1000, mock_target,
//     mock_mutation,
//                                            mock_population, params);

//     for (auto v : result) {
//         ASSERT_EQ(v, 0);
//     }
// }
