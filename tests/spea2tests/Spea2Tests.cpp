//
// Created by tomek on 12/14/25.
//

#include "../../src/algorithms/SPEA2.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
using ::testing::FloatNear;
using ::testing::Pointwise;
#include <cmath>

std::vector<float> mock_target(int number) {
    return std::vector{static_cast<float>(std::pow(number, 2)),
                       static_cast<float>(std::pow(number, 3)),
                       static_cast<float>(std::pow(number, 4))};
}

std::vector<int> mock_population(int number) { return {1, 2 - number, 3, 0}; }

std::vector<int> mock_cross(std::vector<int> pop, std::vector<float> params) {
    if (params.size() > 2) {
        return pop;
    }
    return pop;
}

float distance(int n1, int n2) { return std::abs(n1 - n2); }

template <typename T>
class SPEA2Wrapper : SPEA2<T> {
  public:
    SPEA2Wrapper(std::function<float(T, T)> distance) : SPEA2<T>(distance) {}

    std::vector<int> wrap_calculate_strength(target_function<T> target,
                                             std::vector<T> pop) {
        return this->calculate_strength(target, pop);
    }
    std::vector<int> wrap_calculate_raw_fitness(target_function<T> target,
                                                std::vector<T> combined,
                                                std::vector<T> pop_strength) {
        return this->calculate_raw_fitness(target, combined, pop_strength);
    }
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    wrap_calculate_distances(std::vector<T> pop) {
        return this->calculate_distances(pop);
    }
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    wrap_calculate_fitness(target_function<T> target,
                           std::vector<T> &population, std::vector<T> &set) {
        return this->calculate_fitness(target, population, set);
    }
    std::vector<int> wrap_get_newset(unsigned int setsize,
                                     target_function<T> target,
                                     std::vector<T> &population,
                                     std::vector<T> &set,
                                     std::vector<std::vector<float>> &distances,
                                     std::vector<float> &fitness) {
        return this->get_newset(setsize, target, population, set, distances,
                                fitness);
    }
    std::vector<int> wrap_binary_tour(int popsize, std::vector<T> &external_set,
                                      target_function<T> target) {
        return this->binary_tournament_selection(popsize, external_set, target);
    }
    std::vector<int> wrap_choose_final_pool(target_function<T> target,
                                            int pooolsize,
                                            std::vector<T> &pool1,
                                            std::vector<T> &pool2) {
        return this->choose_final_pool(target, pooolsize, pool1, pool2);
    }
    std::vector<int> wrap_solve(int popsize, int iterations,
                                target_function<T> target,
                                strategy<T> cross_strat,
                                population_generator<T> population_gen,
                                std::vector<float> &params) {

        return this->solve(popsize, iterations, target, cross_strat,
                           population_gen, params);
    }
};

// TEST(SPEA2Tests, testBasicUsage) {
//     auto spea2 = SPEA2<int>();
//     spea2.solve(mock_target, mock_population(4), );
// }

TEST(SPEA2Tests, test_calculate_strength) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {1, -2, 3, 0};
    std::vector strength = spea2.wrap_calculate_strength(mock_target, pop);
    std::vector results = {1, 1, 0, 2};
    ASSERT_EQ(strength, results);
}

TEST(SPEA2Tests, test_calculate_raw_fitness) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {1, -2, 3, 0};
    std::vector strength = spea2.wrap_calculate_strength(mock_target, pop);
    std::vector raw_fitnesses =
        spea2.wrap_calculate_raw_fitness(mock_target, pop, strength);
    std::vector results = {2, 0, 4, 0};
    ASSERT_EQ(raw_fitnesses, results);
}

TEST(SPEA2Tests, test_calculate_distances) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {1, -2, 3, 0};
    std::pair<std::vector<float>, std::vector<std::vector<float>>> distances =
        spea2.wrap_calculate_distances(pop);
    std::vector<float> results_first = {0.25, 0.2, 0.2, 0.25};
    std::vector<std::vector<float>> results_second = {
        {0, 3, 2, 1}, {3, 0, 5, 2}, {2, 5, 0, 3}, {1, 2, 3, 0}};
    ASSERT_EQ(results_second, distances.second);
    EXPECT_THAT(results_first, Pointwise(FloatNear(1e-4), distances.first));
}

TEST(SPEA2Tests, test_calculate_fitness) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {
        1,
        -2,
    };
    std::vector<int> set = {3, 0};
    std::vector<float> fitness =
        spea2.wrap_calculate_fitness(mock_target, set, pop).first;
    std::vector<float> results = {4.2, 0.25, 2.25, 0.2};
    EXPECT_THAT(results, Pointwise(FloatNear(1e-4), fitness));
}

TEST(SPEA2Tests, test_get_newset_not_enough) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {
        1,
        -2,
    };
    std::vector<int> set = {3, 0};
    auto fitness_result = spea2.wrap_calculate_fitness(mock_target, set, pop);
    std::vector<int> newset = spea2.wrap_get_newset(
        3, mock_target, pop, set, fitness_result.second, fitness_result.first);
    std::vector<int> result = {0, -2, 1};
    ASSERT_EQ(result, newset);
}

TEST(SPEA2Tests, test_get_newset_too_many) {
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pop = {
        1,
        -2,
    };
    std::vector<int> set = {3, 0};
    auto fitness_result = spea2.wrap_calculate_fitness(mock_target, set, pop);
    std::vector<int> newset = spea2.wrap_get_newset(
        1, mock_target, pop, set, fitness_result.second, fitness_result.first);
    std::vector<int> result = {-2};
    ASSERT_EQ(result, newset);
}

TEST(SPEA2Tests, test_binary_tornament_selection) {
    std::srand(42);
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> set = {1, -2, 3, 0};
    std::vector<int> pool = spea2.wrap_binary_tour(4, set, mock_target);
    std::vector<int> result = {1, -2, 1, 1};
    ASSERT_EQ(result, pool);
}

TEST(SPEA2Tests, test_choose_final_pool) {
    std::srand(42);
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<int> pool1 = {1, -2, 3, 0};
    std::vector<int> pool2 = {2, -5, 5, 0};
    std::vector<int> final_pool =
        spea2.wrap_choose_final_pool(mock_target, 4, pool1, pool2);
    std::vector<int> result = {-5, -2, 0, 0};
    ASSERT_EQ(result, final_pool);
}

TEST(SPEA2Tests, test_solve) {
    std::srand(42);
    auto spea2 = SPEA2Wrapper<int>(distance);
    std::vector<float> params = {0.0};
    auto solved = spea2.wrap_solve(4, 1, mock_target, mock_cross,
                                   mock_population, params);
    std::vector<int> result = {-2, 0};
    ASSERT_EQ(result, solved);
}
