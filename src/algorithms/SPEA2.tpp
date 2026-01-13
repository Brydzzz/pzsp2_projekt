//
// Created by tomek on 12/12/25.
//
#pragma once
#include <map>
#include <numeric>
#include <vector>

// template <typename T>
// using point = pareto::front<float, 3, T>;

template <typename T>
std::vector<T> SPEA2<T>::generate_init_pop(population_generator<T> generator,
                                           int pop) {
    return generator(pop);
}

template <typename T>
std::vector<std::vector<float>> calculate_targets(target_function<T> target,
                                                  std::vector<T> &combined) {
    std::vector<std::vector<float>> objectives(combined.size());
    for (unsigned int i = 0; i < combined.size(); i++) {
        objectives[i] = target(combined[i]);
    }
    return objectives;
}

template <typename T>
std::vector<T>
SPEA2<T>::solve(int popsize, int iterations, target_function<T> target,
                strategy<T> cross_strat, population_generator<T> population_gen,
                std::vector<float> &params) {
    // params[0] = 0;
    int t = 0;
    int setsize = popsize;
    std::vector<T> population = generate_init_pop(population_gen, popsize);
    std::vector<T> external_set = {};
    while (true) {
        std::cout << "SPEA2 Iteration: " << t << std::endl;
        std::vector<T> combined = population;
        combined.insert(combined.end(), external_set.begin(),
                        external_set.end());
        auto objectives = calculate_targets(target, combined);
        auto fitness_result = calculate_fitness(objectives, combined);
        auto combined_fitness = fitness_result.first;
        auto distances = fitness_result.second;
        std::vector<T> newset = get_newset(setsize, objectives, combined,
                                           distances, combined_fitness);
        if (t > iterations) {
            return newset;
        }
        std::vector<T> combined_set = newset;
        combined_set.insert(combined_set.end(), external_set.begin(),
                            external_set.end());
        auto objectives_pool1 = calculate_targets(target, external_set);
        std::vector<T> pool1 = binary_tournament_selection(
            popsize, external_set,
            calculate_fitness(objectives_pool1, external_set).first);
        auto objectives_pool2 = calculate_targets(target, combined_set);
        std::vector<T> pool2 = binary_tournament_selection(
            popsize, combined_set,
            calculate_fitness(objectives_pool2, combined_set).first);
        std::vector<T> final_pool =
            choose_final_pool(target, popsize, pool1, pool2);
        std::vector<T> next_pop = crossover(cross_strat, final_pool, params);
        external_set = newset;
        population = next_pop;
        t++;
    }
    return std::vector<T>();
}

template <typename T>
std::vector<T> SPEA2<T>::crossover(strategy<T> strat, std::vector<T> population,
                                   const std::vector<float> &params) {
    return strat(population, params);
}

template <typename T>
std::vector<T> SPEA2<T>::choose_final_pool(target_function<T> target,
                                           int poolsize, std::vector<T> &pool1,
                                           std::vector<T> &pool2) {
    pool1.insert(pool1.end(), pool2.begin(), pool2.end());
    auto objectives = calculate_targets(target, pool1);
    auto fitness_result = calculate_fitness(objectives, pool1);
    std::vector<size_t> indices(pool1.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return fitness_result.first[a] < fitness_result.first[b];
    });
    std::vector<T> result;
    for (int i = 0; i < poolsize; i++) {
        result.push_back(pool1[indices[i]]);
    }
    return result;
}

template <typename T>
std::vector<T> SPEA2<T>::binary_tournament_selection(
    unsigned int poolsize, std::vector<T> &set, std::vector<float> fitness) {
    std::vector<T> result;
    if (set.empty()) {
        return set;
    }
    while (result.size() != poolsize) {
        auto first = std::rand() % set.size();
        auto second = std::rand() % set.size();
        if (fitness[first] < fitness[second]) {
            result.push_back(set[first]);
        } else {
            result.push_back(set[second]);
        }
    }
    return result;
}

template <typename T>
std::vector<T> SPEA2<T>::get_newset(unsigned int setsize,
                                    std::vector<std::vector<float>> &objectives,
                                    std::vector<T> &combined,
                                    std::vector<std::vector<float>> &distances,
                                    std::vector<float> &fitness) {
    std::vector<int> non_dominated;
    for (unsigned int i = 0; i < combined.size(); i++) {
        if (fitness[i] < 1) {
            non_dominated.push_back(i);
        }
    }
    if (non_dominated.size() > setsize) {
        unsigned int old_nd_size = non_dominated.size();
        std::vector<std::vector<float>> sorted_distances;
        for (int i : non_dominated) {
            std::vector<float> dist = distances[i];
            std::sort(dist.begin(), dist.end());
            sorted_distances.push_back(dist);
        }
        for (unsigned int u = 0; u < old_nd_size - setsize; u++) {
            int worst = 0;
            for (unsigned int j = 1; j < non_dominated.size(); j++) {
                for (unsigned int k = 1; k < sorted_distances[j].size(); k++) {
                    float dist_j = sorted_distances[j][k];
                    float dist_worst = sorted_distances[worst][k];

                    if (dist_j < dist_worst) {
                        worst = j;
                        break;
                    } else if (dist_j > dist_worst) {
                        break;
                    }
                }
            }
            sorted_distances.erase(sorted_distances.begin() + worst);
            non_dominated.erase(non_dominated.begin() + worst);
        }

    } else if (non_dominated.size() < setsize) {
        std::vector<std::pair<int, std::vector<int>>> ranks(combined.size());
        for (size_t i = 0; i < ranks.size(); ++i) {
            ranks[i].first = i;
        }
        for (unsigned int i = 0; i < objectives[0].size(); i++) {
            std::vector<int> v(combined.size());
            std::iota(v.begin(), v.end(), 0);
            std::sort(v.begin(), v.end(),
                      [objectives, i](const int &a, const int &b) {
                          return objectives[a][i] < objectives[b][i];
                      });
            for (unsigned int j = 0; j < v.size(); j++) {
                ranks[v[j]].second.push_back(static_cast<int>(j));
            }
        }
        std::sort(ranks.begin(), ranks.end(), [](const auto &a, const auto &b) {
            return std::accumulate(a.second.begin(), a.second.end(), 0) <
                   std::accumulate(b.second.begin(), b.second.end(), 0);
        });
        unsigned int old_nd_size = non_dominated.size();
        for (auto it = ranks.begin();
             it != ranks.begin() + (setsize - old_nd_size); it++) {
            if (std::find(non_dominated.begin(), non_dominated.end(),
                          it->first) == non_dominated.end()) {
                non_dominated.push_back(it->first);
            } else {
                old_nd_size--;
            }
        }
    }
    std::vector<T> result;
    for (unsigned int i = 0; i < non_dominated.size(); i++) {
        result.push_back(combined[non_dominated[i]]);
    }
    return result;
}

template <typename T>
std::pair<std::vector<float>, std::vector<std::vector<float>>>
SPEA2<T>::calculate_fitness(std::vector<std::vector<float>> &objectives,
                            std::vector<T> &combined) {
    std::vector<float> fitness(combined.size());
    std::vector<int> pop_strength = calculate_strength(objectives, combined);
    std::vector<int> pop_raw_fitness =
        calculate_raw_fitness(objectives, combined, pop_strength);
    std::pair<std::vector<float>, std::vector<std::vector<float>>>
        distances_results = calculate_distances(combined);
    std::vector<float> distances = distances_results.first;
    for (unsigned int i = 0; i < pop_raw_fitness.size(); i++) {
        fitness[i] = distances[i] + static_cast<float>(pop_raw_fitness[i]);
    }
    return {fitness, distances_results.second};
}

template <typename T>
std::vector<int>
SPEA2<T>::calculate_raw_fitness(std::vector<std::vector<float>> &objectives,
                                std::vector<T> &combined,
                                std::vector<int> &strengths) {
    std::vector<int> raw_fitness(combined.size());
    for (unsigned int i = 0; i < combined.size(); i++) {
        auto target_i = objectives[i];
        auto point_i =
            pareto::point<float, 3>(target_i.begin(), target_i.end());
        for (unsigned int j = 0; j < combined.size(); j++) {
            auto target_j = objectives[j];
            auto point_j =
                pareto::point<float, 3>(target_j.begin(), target_j.end());
            if (point_j.strongly_dominates(point_i)) {
                raw_fitness[i] += strengths[j];
            }
        }
    }
    return raw_fitness;
}

template <typename T>
std::vector<int>
SPEA2<T>::calculate_strength(std::vector<std::vector<float>> &objectives,
                             std::vector<T> &population) {
    std::vector<int> strength(population.size());
    int i = 0;
    for (size_t j = 0; j < population.size(); j++) {
        for (size_t k = 0; k < population.size(); k++) {
            auto target_pop1 = objectives[j];
            auto target_pop2 = objectives[k];
            auto point1 =
                pareto::point<float, 3>(target_pop1.begin(), target_pop1.end());
            auto point2 =
                pareto::point<float, 3>(target_pop2.begin(), target_pop2.end());
            if ((point1).strongly_dominates(point2)) {
                strength[i]++;
            }
        }
        i++;
    }
    return strength;
}

template <typename T>
std::pair<std::vector<float>, std::vector<std::vector<float>>>
SPEA2<T>::calculate_distances(std::vector<T> &combined) {
    std::vector<float> result_distances(combined.size());
    std::vector<std::vector<float>> distances(combined.size());
    for (unsigned int i = 0; i < combined.size(); i++) {
        for (unsigned int j = 0; j < combined.size(); j++) {
            distances[i].push_back(distance_function(combined[i], combined[j]));
        }
    }
    int k = sqrt(combined.size());
    for (unsigned int i = 0; i < distances.size(); i++) {
        auto sorted_distances = distances[i];
        std::sort(sorted_distances.begin(), sorted_distances.end());
        result_distances[i] = 1 / (sorted_distances[k] + 2);
    }
    return {result_distances, distances};
}

// template <typename T>
// float calculate_distance_points(T point1, T point2) {
//
// }