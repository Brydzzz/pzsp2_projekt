#pragma once

#include "../utils/utils.hpp"
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

// by default the tournament selection is used
template <typename T>
std::vector<T> NSGA2<T>::default_selection(std::vector<T> &population,
                                           std::vector<float> &params,
                                           target_function<T> target) {
    int population_size = population.size();
    int param_count = params.size();
    int individuals_in_tournament = (param_count >= 1) ? params[0] : 2;

    std::vector<T> result;

    while ((int)result.size() < population_size) {
        std::vector<T> tournament;
        while ((int)tournament.size() < individuals_in_tournament) {
            int id = getRandomFromRange(0, population_size);
            tournament.push_back(population[id]);
        }

        auto pareto_fronts = sort_nondominated_algorithm(population, target);

        int id = getRandomFromRange(0, pareto_fronts[0].size());
        result.push_back(pareto_fronts[0][id]);
    }

    return result;
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

template <typename T>
std::vector<T> NSGA2<T>::select_best(std::vector<T> &population, int best_size,
                                     target_function<T> target) {
    auto pareto_fronts = sort_nondominated_algorithm(population, target);
    std::vector<T> result;
    int front_id = 0;
    while ((int)result.size() + (int)pareto_fronts[front_id].size() <=
           best_size) {
        for (auto &v : pareto_fronts[front_id]) {
            result.push_back(v);
        }
        front_id++;
    }
    auto distance = crowding_distance(pareto_fronts[front_id], target);
    std::vector<int> sorted_data;
    for (int i = 0; i < (int)pareto_fronts[front_id].size(); i++) {
        sorted_data.push_back(i);
    }
    sort(sorted_data.begin(), sorted_data.end(),
         [&](int a, int b) { return distance[a] < distance[b]; });

    while ((int)result.size() < best_size) {
        result.push_back(pareto_fronts[front_id][sorted_data.back()]);
        sorted_data.pop_back();
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
    for (int iter = 0; iter < iterations; iter++) {
        auto new_population = generate_init_pop(population_gen, popsize);
        auto combined_population = join_vector(population, new_population);
        auto best = select_best(combined_population, popsize, target);
        population = best;
    }
    return population;
}

template <typename T>
std::vector<T> NSGA2<T>::solve2(int popsize, int iterations,
                                target_function<T> target,
                                strategy<T> mutation_strategy,
                                population_generator<T> population_gen,
                                std::vector<float> &params) {
    std::vector<T> population = generate_init_pop(population_gen, popsize);
    auto new_population = generate_init_pop(population_gen, popsize);
    auto combined_population = join_vector(population, new_population);
    for (int iter = 0; iter < iterations; iter++) {
        auto best = select_best(combined_population, popsize, target);
        population = best;
        if (iter + 1 < iterations) {
            new_population = mutation_strategy(population, params);
            combined_population = join_vector(population, new_population);
        }
    }
    return population;
}