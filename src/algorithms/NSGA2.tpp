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
    result.reserve(v1.size() + v2.size());
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
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
std::vector<T> NSGA2<T>::mutation(strategy<T> strat, std::vector<T> population,
                                  const std::vector<float> &params) {
    return strat(population, params);
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

    std::vector<pareto::point<float, 3>> points(population_size);
    for (int i = 0; i < population_size; i++) {
        auto objective_value = target(population[i]);
        auto tmp = pareto::point<float, 3>(objective_value.begin(),
                                           objective_value.end());
        points[i] = std::move(tmp);
    }

    for (int i = 0; i < population_size; i++) {
        auto &individual_point = points[i];

        for (int j = 0; j < population_size; j++) {
            if (i == j)
                continue;
            auto &other_point = points[j];

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
        std::vector<int> next_front;
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
std::vector<T> NSGA2<T>::solve(int popsize, int iterations,
                               target_function<T> target, strategy<T> mut_strat,
                               population_generator<T> population_gen,
                               std::vector<float> &params) {
    std::vector<T> population = generate_init_pop(population_gen, popsize);
    auto new_population = generate_init_pop(population_gen, popsize);
    auto combined_population = join_vector(population, new_population);
    for (int iter = 0; iter < iterations; iter++) {
        // std::cout << "NSGA2 Iteration: " << iter << std::endl;
        population = select_best(combined_population, popsize, target);
        if (iter + 1 < iterations) {
            new_population = mutation(mut_strat, population, params);
            combined_population = join_vector(population, new_population);
        }
    }
    return population;
}