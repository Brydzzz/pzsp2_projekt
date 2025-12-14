//
// Created by tomek on 12/12/25.
//

#include "SPEA2.h"

using point = front<float, 3, T>;

template <typename T>
std::vector<T> SPEA2<T>::generate_init_pop(population_generator<T> generator,
                                           int pop) {
    return generator(pop);
}


template <typename T>
T SPEA2<T>::solve(int popsize, target_function<T> target,
                  population_generator<T> population_gen,
                  strategy<T> select_strat, strategy<T> cross_stat,
                  strategy<T> succ_strat, std::vector<float> &params) {
    std::vector<T> initial_pop = generate_init_pop(population_gen);
    std::vector<T> external_set = {};
    std::vector<float> combined_fitness = calculate_fitness(target,
        initial_pop, external_set);
    std::vector<T> next_pop = selection(combined_fitness);
}

template <typename T>
std::vector<float> SPEA2<T>::calculate_fitness(
    target_function<T> target,
    std::vector<T> &population, std::vector<T> &set) {
    std::vector<T> combined = population;
    population.insert(population.end(), set.begin(), set.end());
    std::vector<int> pop_strength = calculate_strength(target, combined);
    std::vector<int> raw_fitness = {combined.size()};
    for (int i = 0; i < combined.size(); i++) {
        for (int j = 0; j < combined.size(); j++) {
            if (point(
                target(combined[j]).strongly_dominates(
                    point(target(combined[i]), true)))) {
                raw_fitness[i] += pop_strength[j];
            }
        }
    }

}

template <typename T>
std::vector<int> calculate_strength(target_function<T> target,
                                    std::vector<T> &population) {

    std::vector<int> strength = {population.size()};
    int i = 0;
    for (auto pop1 : population) {
        for (auto pop2 : population) {
            if (point(
                target(pop1).strongly_dominates(point(target(pop2), true)))) {
                strength[i]++;
            }
        }
        i++;
    }
    return strength;
}

