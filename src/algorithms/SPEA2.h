//
// Created by tomek on 12/12/25.
//

#pragma once
#include <pareto/front.h>
#include <pareto/kd_tree.h>
#include "Evolutionary_Algorithm.h"


template <typename T>
class SPEA2 : public Evolutionary_Algorithm<T>
{
protected:
    std::vector<T>
    generate_init_pop(population_generator<T> generator, int pop) override;
    std::vector<T> crossover(strategy<T> strat,
                             std::vector<T> population,
                             const std::vector<float>& params) override;
    std::pair<std::vector<float>, std::vector<float>> calculate_fitness(
        std::vector<T>& population,
        std::vector<T>& set);
    std::vector<int> calculate_strength(target_function<T> target,
                                        std::vector<T>& population);
    std::pair<std::vector<float>, std::vector<std::vector<float>>> calculate_fitness(
        target_function<T> target,
        std::vector<T>& population, std::vector<T>& set);
    std::vector<int> calculate_raw_fitness(
        std::vector<T>& combined, std::vector<int>& strengths);
    std::pair<std::vector<float>, std::vector<std::vector<float>>> calculate_distances(std::vector<T>& combined);
    std::vector<T> get_newset(int setsize, target_function<T> target, std::vector<T>& population,
                              std::vector<T>& set, std::vector<std::vector<float>>& distances,
                              std::vector<float>& fitness);
    std::vector<T> binary_tournament_selection(int poolsize, std::vector<T>& set);
    std::vector<T> choose_final_pool(target_function<T> target, int poolsize, std::vector<T>& pool1,
                                     std::vector<T>& pool2);

public:
    SPEA2<T>();
    std::vector<T> solve(int popsize, int iterations, target_function<T> target,
                         population_generator<T> population_gen, std::vector<float>& params) override;
};


#include "SPEA2.tpp"
