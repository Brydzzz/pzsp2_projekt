#pragma once

#include "NSGA2.h"

#include <limits>
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

    std::vector<std::vector<int>> dominates_points(population_size);
    std::vector<std::vector<int>> pareto_fronts(1);
    std::vector<int> dominated_by_count(population_size, 0);

    for (int i = 0; i < population_size; i++) {
        auto individual = population[i];
        auto objective = target(individual);
        auto individual_point =
            pareto::point<float, 3>(objective.begin(), objective.end());

        for (int j = 0; j < population_size; j++) {
            auto other_individual = population[j];
            auto other_objective = target(other_individual);
            auto other_point = pareto::point<float, 3>(other_objective.begin(),
                                                       other_objective.end());

            if (individual_point.strongly_dominates(other_point)) {
                dominates_points[i].push_back(j);
            } else if (other_point.strongly_dominates(individual_point)) {
                dominated_by_count[i]++;
            }
        }
        if (dominated_by_count[i] == 0) {
            pareto_fronts[0].push_back(i);
        }
    }

    int k = 0;
    while ((int)pareto_fronts.size() > k) {
        std::vector<T> next_front;
        for (int i : pareto_fronts[k]) {
            for (int j = 0; j < (int)dominates_points[i].size(); j++) {
                auto other_individual = dominates_points[i][j];
                dominated_by_count[other_individual]--;
                if (dominated_by_count[other_individual] == 0) {
                    next_front.push_back(other_individual);
                }
            }
        }
        k++;
        if (next_front.size()) {
            pareto_fronts.push_back(next_front);
        }
    }

    std::vector<std::vector<T>> reconstructed_answer;
    for (int i = 0; i < (int)pareto_fronts.size(); i++) {
        reconstructed_answer.push_back({});
        for (auto id : pareto_fronts[i]) {
            reconstructed_answer.back().push_back(population[id]);
        }
    }

    return reconstructed_answer;
}

template <typename T>
std::vector<T> NSGA2<T>::mutate(std::vector<T> &population,
                                const std::vector<float> &params) {
    return population;
}

template <typename T>
std::vector<T> NSGA2<T>::selection(std::vector<T> &population,
                                   target_function<T> target,
                                   const std::vector<float> &params) {
    return population;
}

template <typename T>
std::vector<float> NSGA2<T>::crowding_distance(std::vector<T> &population,
                                               target_function<T> &target) {

    int population_size = population.size();
    if (population_size == 0)
        return {};

    std::vector<std::vector<float>> objective_results(population_size);
    for (int i = 0; i < population_size; i++) {
        objective_results[i] = target(population[i]);
    }

    int objective_count = objective_results[0].size();

    std::vector<std::pair<float, int>> distances(population_size);

    for (int i = 0; i < population_size; i++) {
        distances[i].second = i;
    }

    const float inf = std::numeric_limits<float>::infinity();
    for (int i = 0; i < objective_count; i++) {
        sort(distances.begin(), distances.end(),
             [&](std::pair<float, int> a, std::pair<float, int> b) {
                 return objective_results[a.second][i] <
                        objective_results[b.second][i];
             });
        // if (i == 1) {
        //     std::vector<float> result;
        //     for (int i = 0; i < population_size; i++) {
        //         result.push_back(distances[i].second);
        //     }
        //     return result;
        // }
        distances[0].first = inf;
        distances.back().first = inf;
        float denumerator = abs(objective_results[distances[0].second][i] -
                                objective_results[distances.back().second][i]);

        for (int j = 1; j < population_size - 1; j++) {
            float numerator =
                abs(objective_results[distances[j - 1].second][i] -
                    objective_results[distances[j + 1].second][i]);

            distances[j].first += numerator / denumerator;
        }
    }

    std::vector<float> result(population_size);
    for (int i = 0; i < population_size; i++) {
        result[distances[i].second] = distances[i].first;
    }
    return result;
}

//
// public functions
//

template <typename T>
std::vector<T>
NSGA2<T>::solve(int popsize, int iterations, target_function<T> target,
                strategy<T> cross_strat, population_generator<T> population_gen,
                std::vector<float> &params) {
    std::vector<T> population = generate_init_pop(population_gen, popsize);
    std::vector<T> offspring = selection(population, target, params);
    offspring = mutate(population, params);

    // for (int _ = 0; _ < iterations; _++) {
    //     std::vector<T> combined = join_vector(population, offspring);
    //     auto pareto_fronts = sort_nondominated_algorithm(combined, target);
    //     std::vector<T> new_population;

    //     int i = 1;
    //     while ((int)new_population.size() < popsize and
    //            (int) new_population.size() + (int)pareto_fronts[i].size() <=
    //                popsize) {
    //         auto crowding_distances =
    //             crowding_distance(pareto_fronts[i], target);
    //         for (auto &individual : pareto_fronts[i]) {
    //             new_population.push_back(individual);
    //         }
    //         i++;
    //     }
    //     if ((int)new_population.size() < popsize) {
    //         auto crowding_distances =
    //             crowding_distance(pareto_fronts[i], target);
    //         sort(crowding_distances.begin(), crowding_distances.end());
    //         while ((int)new_population.size() < popsize) {
    //             new_population.push_back(
    //                 population[crowding_distances.back().second]);
    //             crowding_distances.pop_back();
    //         }
    //     }

    //     offspring = selection(new_population, target, params);
    //     offspring = mutate(new_population, params);
    //     population = new_population;
    // }
    return population;
}