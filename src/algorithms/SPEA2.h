//
// Created by tomek on 12/12/25.
//

#pragma once
#include <pareto/front.h>
#include <pareto/kd_tree.h>
#include "Evolutionary_Algorithm.h"

template <typename T>
using distance_function = std::function<float(T, T)>;


template <typename T>
class SPEA2 : public Evolutionary_Algorithm<T> {
    distance_function<T> distance_function;

protected:
    std::vector<T>
    generate_init_pop(population_generator<T> generator, int pop) override;
    std::vector<T> selection(strategy<T> strat, std::vector<T> population,
                             const std::vector<float> &params) override;
    std::vector<float> calculate_fitness(target_function<T> target,
                                         std::vector<T> &population,
                                         std::vector<T> &set);
    std::vector<int> calculate_strength(target_function<T> target,
                                        std::vector<T> &population);
    std::vector<int> calculate_raw_fitness(
        std::vector<T> &combined, std::vector<int> &strengths);
    std::vector<float> calculate_distances(std::vector<T> &combined);
    std::vector<T> get_newpop(target_function<T> target,
                              std::vector<T> &population,
                              std::vector<T> &set,
                              std::vector<float> &fitness);

public:
    SPEA2<T>(distance_function<T> distance_function);
    T solve(int popsize, target_function<T> target,
            population_generator<T> population_gen,
            strategy<T> select_strat, strategy<T> cross_stat,
            strategy<T> succ_strat, std::vector<float> &params) override;

};


#include "SPEA2.tpp"