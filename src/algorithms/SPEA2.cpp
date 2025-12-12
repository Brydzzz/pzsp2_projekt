//
// Created by tomek on 12/12/25.
//

#include "SPEA2.h"

template <typename T>
T SPEA2<T>::solve(target_function target, population_generator population_gen,
                  strategy select_strat, strategy cross_stat,
                  strategy succ_strat, std::vector<float> &params) {
    std::vector<T> initial_pop = generate_init_pop(population_gen);
    std::vector<T> external_set = {};
    std::vector<std::vector<float>> combined_fitness = calculate_fitness(
        initial_pop, external_set);
    std::vector<T> next_pop = selection(combined_fitness)
}
