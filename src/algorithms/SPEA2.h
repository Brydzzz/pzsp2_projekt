//
// Created by tomek on 12/12/25.
//

#pragma once
#include "Evolutionary_Algorithm.h"
#include <pareto/front.h>
#include <pareto/kd_tree.h>

template <typename T>
class SPEA2 : public Evolutionary_Algorithm<T> {
    std::function<float(const std::vector<float> &, const std::vector<float> &)>
        distance_function;

  protected:
    std::vector<T> generate_init_pop(population_generator<T> generator,
                                     int pop) override;
    std::vector<T> mutation(strategy<T> strat, std::vector<T> population,
                            const std::vector<float> &params) override;
    std::vector<int>
    calculate_strength(std::vector<std::vector<float>> &objectives,
                       std::vector<T> &population);
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    calculate_fitness(std::vector<std::vector<float>> &objectives,
                      std::vector<T> &combined, target_function<T> target);
    std::vector<int>
    calculate_raw_fitness(std::vector<std::vector<float>> &objectives,
                          std::vector<T> &combined,
                          std::vector<int> &strengths);
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
    calculate_distances(target_function<T> target, std::vector<T> &combined);
    std::vector<T> get_newset(unsigned int setsize,
                              std::vector<std::vector<float>> &objectives,
                              std::vector<T> &combined,
                              std::vector<std::vector<float>> &distances,
                              std::vector<float> &fitness);
    std::vector<T> binary_tournament_selection(unsigned int poolsize,
                                               std::vector<T> &set,
                                               std::vector<float> fitness);
    std::vector<T> choose_final_pool(target_function<T> target, int poolsize,
                                     std::vector<T> &pool1,
                                     std::vector<T> &pool2);

  public:
    using Evolutionary_Algorithm<T>::Evolutionary_Algorithm;
    SPEA2(float (*distance)(const std::vector<float> &,
                            const std::vector<float> &),
          bool logs = false)
        : Evolutionary_Algorithm<T>(logs), distance_function(distance) {}

    std::vector<T> solve(int popsize, int iterations, target_function<T> target,
                         strategy<T> mut_strat,
                         population_generator<T> population_gen,
                         std::vector<float> &params) override;
};

#include "SPEA2.tpp"