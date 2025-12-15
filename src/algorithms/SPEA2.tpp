//
// Created by tomek on 12/12/25.
//

template <typename T>
using point = pareto::front<float, 3, T>;

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
    params[0] = 0;
    std::vector<T> initial_pop = generate_init_pop(population_gen, popsize);
    select_strat();
    cross_stat();
    succ_strat();
    std::vector<T> external_set = {};
    std::vector<float> combined_fitness = calculate_fitness(target,
        initial_pop, external_set);
    std::vector<T> newpop =
        get_newpop(target, initial_pop, external_set, combined_fitness);
    return T();
}


template <typename T>
std::vector<T> get_newpop(target_function<T> target, std::vector<T> &population,
                          std::vector<T> &set,
                          std::vector<float> &fitness) {
    std::vector<T> combined = population;
    combined.insert(combined.end(), set.begin(), set.end());
    pareto::front<float, 3, T> non_dominated;
    for (auto pop : population) {
        std::vector<float> score = target(pop);
        non_dominated(score[0], score[1], score[2]) = pop;
    }
    if (non_dominated.size() > population.size()) {

    }
}


template <typename T>
std::vector<float> SPEA2<T>::calculate_fitness(
    target_function<T> target,
    std::vector<T> &population, std::vector<T> &set) {
    std::vector<T> combined = population;
    combined.insert(combined.end(), set.begin(), set.end());
    std::vector<float> fitness(combined.size());
    std::vector<int> pop_strength = calculate_strength(target, combined);
    std::vector<int> pop_raw_fitness = calculate_raw_fitness(
        combined, pop_strength);
    std::vector<float> distances = calculate_distances(combined);
    for (int i = 0; i < pop_raw_fitness.size(); i++) {
        fitness[i] = distances[i] + static_cast<float>(pop_raw_fitness[i]);
    }
    return fitness;
}


template <typename T>
std::vector<int> SPEA2<T>::calculate_raw_fitness(
    std::vector<T> &combined, std::vector<int> &strengths) {
    std::vector<int> raw_fitness(combined.size());
    for (int i = 0; i < combined.size(); i++) {
        for (int j = 0; j < combined.size(); j++) {
            if (point(
                target(combined[j]).strongly_dominates(
                    point(target(combined[i]), true)))) {
                raw_fitness[i] += strengths[j];
            }
        }
    }
    return raw_fitness;
}


template <typename T>
std::vector<int> SPEA2<T>::calculate_strength(target_function<T> target,
                                              std::vector<T> &population) {

    std::vector<int> strength(population.size());
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


template <typename T>
std::vector<float> SPEA2<T>::calculate_distances(std::vector<T> &combined) {
    std::vector<float> result_distances(combined.size());
    std::vector<std::vector<float>> distances(combined.size());
    for (int i = 0; i < combined.size(); i++) {
        for (int j = 0; j < combined.size(); j++) {
            distances[i].push_back(distance_function(combined[i], combined[j]));
        }
    }
    int k = sqrt(combined.size());
    for (int i = 0; i < distances.size(); i++) {
        std::sort(distances[i].begin(), distances[i].end());
        result_distances[i] = 1 / (distances[i][k] + 2);
    }
    return result_distances;
}

// template <typename T>
// float calculate_distance_points(T point1, T point2) {
//
// }
