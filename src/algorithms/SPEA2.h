//
// Created by tomek on 12/12/25.
//

#pragma once

#include "Evolutionary_Algorithm.h"

template <typename T>
class SPEA2 : public Evolutionary_Algorithm<T> {
private:
    std::vector<T> generate_init_pop(population_generator) override;
    std::vector<std::vector<float>> calculate_fitness();
    std::vector<T> selection(strategy strat, std::vector<T> population,
                             const std::vector<float> &params) override;

public:
    T solve(target_function target, population_generator population_gen,
            strategy select_strat, strategy cross_stat,
            strategy succ_strat, std::vector<float> &params) override;

};


