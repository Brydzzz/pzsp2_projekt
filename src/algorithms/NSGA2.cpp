#pragma once

#include "NSGA2.h"

#include <map>
#include <numeric>
#include <vector>

//
// private functions
//

template <typename T>
std::vector<T> NSGA2<T>::join_vector(std::vector<T> &v1, std::vector<T> &v2) {
    std::vector<T> result;
    for (T &element_v1 : v1) {
        result.push_back(element_v1);
    }
    for (T &element_v2 : v2) {
        result.push_back(element_v2);
    }

    return result;
}

//
// protected functions
//

template <typename T>
std::vector<T> NSGA2<T>::generate_init_pop(population_generator<T> generator,
                                           int pop) {
    return generator(pop);
}

template <typename T>
std::vector<T> NSGA2<T>::crossover(strategy<T> strat, std::vector<T> population,
                                   const std::vector<float> &params) {
    return population;
}

template <typename T>
std::vector<std::vector<T>>
NSGA2<T>::sort_nondominated_algorithm(std::vector<T> &population,
                                      target_function<T> target) {

    int population_size = population.size();

    std::vector<std::vector<T>> dominates_points(population_size, {});
    std::vector<std::vector<T>> pareto_fronts(1);
    std::vector<int> dominated_by_count(population_size, 0);

    for (int i = 0; i < population_size; i++) {
        auto individual = population[i];
        auto individual_point = pareto::point<float, 3>(
            target(individual).begin(), target(individual).end());

        for (auto other_individual : population) {
            auto other_point =
                pareto::point<float, 3>(target(other_individual).begin(),
                                        target(other_individual).end());
            if (individual.strongly_dominates(other_point)) {
                dominates_points[i].push_back(other_individual);
            } else if (other_point.strongly_dominates(individual)) {
                dominated_by_count[i]++;
            }
        }

        if (dominated_by_count[i] == 0) {
            pareto_fronts[0].push_back(individual);
        }
    }

    int k = 0;
    while ((int)pareto_fronts.size() > k) {
        std::vector<T> next_front;
        for (int i = 0; i < pareto_fronts[k].size(); i++) {
            auto individual = pareto_fronts[k][i];
            for (int j = 0; j < dominates_points.size(); j++) {
                auto other_individual = dominates_points[i][j];
                dominated_by_count[j]--;
                if (dominated_by_count[j] == 0) {
                    next_front.push_back(other_individual);
                }
            }
        }
        k++;
        if (next_front.size()) {
            pareto_fronts.pop_back(next_front);
        }
    }

    return pareto_fronts;
}

template <typename T>
std::vector<T> NSGA2<T>::mutate(std::vector<T> &population,
                                const std::vector<float> &params) {
    return population;
}

template <typename T>
std::vector<float> NSGA2<T>::crowding_distance(std::vector<T> &population,
                                               target_function<T> &target) {
    int l = population.size();
    std::vector<float> result(l, 0);
    // TODO: implement
    return result;
}

//
// public functions
//

template <typename T>
std::vector<T> NSGA2<T>::solve(int popsize, int iterations,
                               target_function<T> target,
                               population_generator<T> population_gen,
                               std::vector<float> &params) {
    std::vector<T> population = generate_init_pop(population_gen, popsize);
    std::vector<T> offspring = select(population, target, params);
    offspring = mutate(population, params);

    for (int i = 0; i < iterations; i++) {
        std::vector<T> combined = join_vector(population, offspring);
        auto pareto_fronts = sort_nondominated_algorithm(combined, target);
        std::vector<T> new_population;

        int i = 1;
        while (new_population.size() < popsize and
               new_population.size() + pareto_fronts[i] < popsize) {
            auto crowding_distances =
                crowding_distance(pareto_fronts[i], target);
            for (auto &individual : pareto_fronts[i]) {
                new_population.push_back(individual);
            }
            i++;
        }
        if (new_population.size() < popsize) {
            auto crowding_distances = crowding_distance(pareto_fronts[i]);
            sort(crowding_distances.begin(), crowding_distances.end());
            while (new_population.size() < popsize) {
                new_population.push_back(crowding_distances.back());
                crowding_distance.pop_back();
            }
        }

        offspring = select(new_population);
        offspring = mutate(new_population);
        population = new_population;
    }
}