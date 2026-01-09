//
// Created by tomek on 12/12/25.
//

#pragma once
#include "Evolutionary_Algorithm.h"
#include <pareto/front.h>
#include <pareto/kd_tree.h>

template <typename T>
class SPEA2 : public Evolutionary_Algorithm<T> {
    std::function<float(T, T)> distance_function;

  protected:
    std::vector<T> generate_init_pop(population_generator<T> generator,
                                     int pop) override;
    std::vector<T> crossover(strategy<T> strat, std::vector<T> population,
                             const std::vector<float> &params) override;
    std::pair<std::vector<float>, std::vector<float>>
    calculate_fitness(std::vector<T> &population, std::vector<T> &set);
    std::vector<int> calculate_strength(target_function<T> target,
                                        std::vector<T> &population);
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    calculate_fitness(target_function<T> target, const std::vector<T> &population,
                      const std::vector<T> &set);
    std::vector<int> calculate_raw_fitness(target_function<T> target,
                                           std::vector<T> &combined,
                                           std::vector<int> &strengths);
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    calculate_distances(std::vector<T> &combined);
    std::vector<T> get_newset(unsigned int setsize, target_function<T> target,
                              std::vector<T> &population, std::vector<T> &set,
                              std::vector<std::vector<float>> &distances,
                              std::vector<float> &fitness);
    std::vector<T> binary_tournament_selection(unsigned int poolsize,
                                               std::vector<T> &set,
                                               std::vector<float> fitness);
    std::vector<T> choose_final_pool(target_function<T> target, int poolsize,
                                     std::vector<T> &pool1,
                                     std::vector<T> &pool2);

  public:
    SPEA2(std::function<float(T, T)> distance) : distance_function(distance) {}

    std::vector<T> solve(int popsize, int iterations, target_function<T> target,
                         strategy<T> cross_strat,
                         population_generator<T> population_gen,
                         std::vector<float> &params) override;
};

#include "SPEA2.tpp"