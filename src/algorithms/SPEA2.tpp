//
// Created by tomek on 12/12/25.
//
#include <numeric>
template <typename T>
using point = pareto::front<float, 3, T>;

template <typename T>
std::vector<T> SPEA2<T>::generate_init_pop(population_generator<T> generator,
                                           int pop)
{
    return generator(pop);
}


template <typename T>
T SPEA2<T>::solve(int popsize, target_function<T> target,
                  population_generator<T> population_gen, std::vector<float>& params)
{
    params[0] = 0;
    int setsize = popsize / 2;
    std::vector<T> initial_pop = generate_init_pop(population_gen, popsize);
    std::vector<T> external_set = {};
    std::vector<T> combined = initial_pop;
    combined.insert(combined.end(), external_set.begin(), external_set.end());
    auto fitness_result = calculate_fitness(target,
                                            initial_pop, external_set);
    auto combined_fitness = fitness_result.first;
    auto distances = fitness_result.second;
    std::vector<int> newset =
        get_newpop(setsize, target, initial_pop, external_set, distances, combined_fitness);
    std::vector<int>
    return T();
}


template <typename T>
std::vector<int> get_newpop(int setsize, target_function<T> target, std::vector<T>& population,
                            std::vector<T>& set, std::vector<std::vector<float>>& distances,
                            std::vector<float>& fitness)
{
    std::vector<T> combined = population;
    combined.insert(combined.end(), set.begin(), set.end());
    std::vector<T> non_dominated;
    for (int i = 0; i < combined.size(); i++)
    {
        if (fitness[i] < 1)
        {
            non_dominated.push_back(i);
        }
    }
    if (non_dominated.size() > setsize)
    {
        int worst = 0;
        for (int u = 0; u < non_dominated.size() - setsize; u++)
        {
            for (int j = 0; j < non_dominated.size(); j++)
            {
                for (unsigned int l = 1; l < distances[0].size(); l++)
                {
                    if (distances[j][l] < distances[worst][l])
                    {
                        worst = j;
                    }
                    else if (distances[j][l] == distances[worst][l])
                    {
                        continue;
                    }
                    else
                    {
                        j++;
                        l = 1;
                    }
                }
            }
        }
        auto to_remove = std::find(non_dominated, worst);
        non_dominated.erase(to_remove);
    }
    else if (non_dominated.size() < setsize)
    {
        std::map<int, std::vector<int>> ranks;
        for (int i = 0; i < target(combined[non_dominated[0]]).size(); i++)
        {
            std::vector<int> v(combined.size());
            std::iota(v.begin(), v.end(), 0);
            std::sort(v.begin(), v.end(), [target, i](const int& a, const int& b)
            {
                return target(a)[i] < target(b)[i];
            });
            for (unsigned int j = 0; j < v.size(); j++)
            {
                ranks[v[j]].push_back(static_cast<int>(j));
            }
        }
        std::sort(ranks.begin(), ranks.end(), [](const std::vector<int>& a, const std::vector<int>& b)
        {
            return std::accumulate(a.begin(), a.end(), 0) > std::accumulate(b.begin(), b.end(), 0);
        });
        for (auto it = ranks.begin(); it != ranks.begin() + (setsize - non_dominated.size()); it++)
        {
            if (std::find(non_dominated, it->first) == ranks.end())
            {
                non_dominated.push_back(it->first);
            }
        }
    }
    return non_dominated;
}


template <typename T>
std::pair<std::vector<float>, std::vector<std::vector<float>>> SPEA2<T>::calculate_fitness(
    target_function<T> target,
    std::vector<T>& population, std::vector<T>& set)
{
    std::vector<T> combined = population;
    combined.insert(combined.end(), set.begin(), set.end());
    std::vector<float> fitness(combined.size());
    std::vector<int> pop_strength = calculate_strength(target, combined);
    std::vector<int> pop_raw_fitness = calculate_raw_fitness(
        combined, pop_strength);
    std::pair<std::vector<float>, std::vector<std::vector<float>>> distances_results = calculate_distances(combined);
    std::vector<float> distances = distances_results.first;
    for (int i = 0; i < pop_raw_fitness.size(); i++)
    {
        fitness[i] = distances[i] + static_cast<float>(pop_raw_fitness[i]);
    }
    return {fitness, distances_results.second};
}


template <typename T>
std::vector<int> SPEA2<T>::calculate_raw_fitness(
    std::vector<T>& combined, std::vector<int>& strengths)
{
    std::vector<int> raw_fitness(combined.size());
    for (int i = 0; i < combined.size(); i++)
    {
        for (int j = 0; j < combined.size(); j++)
        {
            if (point(
                target(combined[j]).strongly_dominates(
                    point(target(combined[i]), true))))
            {
                raw_fitness[i] += strengths[j];
            }
        }
    }
    return raw_fitness;
}


template <typename T>
std::vector<int> SPEA2<T>::calculate_strength(target_function<T> target,
                                              std::vector<T>& population)
{
    std::vector<int> strength(population.size());
    int i = 0;
    for (auto pop1 : population)
    {
        for (auto pop2 : population)
        {
            if (point(
                target(pop1).strongly_dominates(point(target(pop2), true))))
            {
                strength[i]++;
            }
        }
        i++;
    }
    return strength;
}


template <typename T>
std::pair<std::vector<float>, std::vector<std::vector<float>>> SPEA2<T>::calculate_distances(std::vector<T>& combined)
{
    std::vector<float> result_distances(combined.size());
    std::vector<std::vector<float>> distances(combined.size());
    for (int i = 0; i < combined.size(); i++)
    {
        for (int j = 0; j < combined.size(); j++)
        {
            distances[i].push_back(distance_function(combined[i], combined[j]));
        }
    }
    int k = sqrt(combined.size());
    for (int i = 0; i < distances.size(); i++)
    {
        auto sorted_distances = distances[i];
        std::sort(sorted_distances.begin(), sorted_distances.end());
        result_distances[i] = 1 / (distances[i][k] + 2);
    }
    return {result_distances, distances};
}

// template <typename T>
// float calculate_distance_points(T point1, T point2) {
//
// }
