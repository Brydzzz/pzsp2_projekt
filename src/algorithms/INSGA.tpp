//
// Created by Marek on 28/12/25.
//

#ifdef __INTELLISENSE__
#include "INSGA.h"
#endif

#include "Individual.h"
#include <vector>

struct INSGASortDelayComparator {
    target_function<Individual> target;

    explicit INSGASortDelayComparator(target_function<Individual> t)
        : target(std::move(t)) {}

    bool operator()(const Individual &a, const Individual &b) const {
        return target(a)[0] < target(b)[0];
    }
};

struct INSGASortLossComparator {
    target_function<Individual> target;

    explicit INSGASortLossComparator(target_function<Individual> t)
        : target(std::move(t)) {}

    bool operator()(const Individual &a, const Individual &b) const {
        return target(a)[1] < target(b)[1];
    }
};

struct INSGASortJitterComparator {
    target_function<Individual> target;

    explicit INSGASortJitterComparator(target_function<Individual> t)
        : target(std::move(t)) {}

    bool operator()(const Individual &a, const Individual &b) const {
        return target(a)[2] < target(b)[2];
    }
};

template <typename T>
typename INSGA<T>::Population
INSGA<T>::random_initialization(int n,
                                population_generator<T> population_gen) const {
    return population_gen(n);
}

template <typename T>
typename INSGA<T>::Population INSGA<T>::empty_offspring_population() const {
    std::vector<T> empty_population;
    return empty_population;
}

template <typename T>
typename INSGA<T>::Population INSGA<T>::extend_population(
    const typename INSGA<T>::Population &base_population,
    const typename INSGA<T>::Population &offspring_population) const {
    std::vector<T> extended_population;
    extended_population.reserve(base_population.size() +
                                offspring_population.size());

    for (const T &specimen : base_population) {
        extended_population.push_back(specimen);
    }
    for (const T &specimen : offspring_population) {
        extended_population.push_back(specimen);
    }
    return extended_population;
}

template <typename T>
typename INSGA<T>::Fronts
INSGA<T>::non_dominance_sorting(const typename INSGA<T>::Population &population,
                                target_function<T> target) const {
    int population_size = population.size();

    std::vector<std::vector<int>> dominates_points(population_size);
    std::vector<std::vector<int>> pareto_fronts(1);
    std::vector<int> dominated_by_count(population_size, 0);

    std::vector<pareto::point<float, 3>> points(population_size);
    for (int i = 0; i < population_size; i++) {
        auto objective_value = target(population[i]);
        points[i] = pareto::point<float, 3>(objective_value.begin(),
                                            objective_value.end());
    }

    for (int i = 0; i < population_size; i++) {
        auto individual_point = points[i];

        for (int j = 0; j < population_size; j++) {
            if (i == j)
                continue;
            auto other_point = points[j];

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
typename INSGA<T>::Fronts
INSGA<T>::sort_insga(const typename INSGA<T>::Fronts &fronts, QoSCriterion w,
                     target_function<T> target) const {
    Fronts sorted = fronts; // copy

    for (auto &front : sorted) {
        switch (w) {
        case QoSCriterion::Jitter:
            std::sort(front.begin(), front.end(),
                      INSGASortJitterComparator(target));
            break;

        case QoSCriterion::Loss:
            std::sort(front.begin(), front.end(),
                      INSGASortLossComparator(target));
            break;

        case QoSCriterion::Delay:
            std::sort(front.begin(), front.end(),
                      INSGASortDelayComparator(target));
            break;

        default:
            break;
        }
    }

    return sorted;
}

template <typename T>
typename INSGA<T>::Population
INSGA<T>::elite_parent_selection(const typename INSGA<T>::Fronts &sorted_fronts,
                                 int n) const {
    typename INSGA<T>::Population base_new_population;
    base_new_population.reserve(static_cast<size_t>(n));

    for (const auto &front : sorted_fronts) {
        if (base_new_population.size() >= static_cast<size_t>(n)) {
            break;
        }

        const size_t remaining =
            static_cast<size_t>(n) - base_new_population.size();
        if (front.size() <= remaining) {
            base_new_population.insert(base_new_population.end(), front.begin(),
                                       front.end());
        } else {
            base_new_population.insert(base_new_population.end(), front.begin(),
                                       front.begin() + remaining);
        }
    }

    return base_new_population;
}

template <typename T>
typename INSGA<T>::Population INSGA<T>::selection(
    const typename INSGA<T>::Population &base_new_population) const {
    return base_new_population;
}

template <typename T>
typename INSGA<T>::Population
INSGA<T>::mutation(strategy<T> strat, Population population,
                   const std::vector<float> &params) {
    auto mutated_population = strat(population, params);
    return mutated_population;
}

template <typename T>
typename INSGA<T>::Population INSGA<T>::combine(
    const typename INSGA<T>::Population &offspring_selected,
    const typename INSGA<T>::Population &crossovered_population,
    const typename INSGA<T>::Population &mutated_population) const {
    std::vector<T> new_offspring;
    new_offspring.reserve(offspring_selected.size() +
                          crossovered_population.size() +
                          mutated_population.size());

    for (const T &specimen : offspring_selected) {
        new_offspring.push_back(specimen);
    }
    for (const T &specimen : crossovered_population) {
        new_offspring.push_back(specimen);
    }
    for (const T &specimen : mutated_population) {
        new_offspring.push_back(specimen);
    }
    return new_offspring;
}

template <typename T>
typename INSGA<T>::Front
INSGA<T>::get_first_front(const typename INSGA<T>::Fronts &fronts) const {
    return fronts[0];
}

template <typename T>
typename INSGA<T>::Population
INSGA<T>::generate_init_pop(population_generator<T> generator, int pop) {
    return generator(pop);
}

template <typename T>
typename INSGA<T>::Front
INSGA<T>::solve(int popsize, int iterations, target_function<T> target,
                strategy<T> mut_strat, population_generator<T> population_gen,
                std::vector<float> &params) {
    if (params.size() < 1) {
        throw std::invalid_argument(
            "Params vector must contain mutation_probability");
    }

    typename INSGA<T>::Population base_population, offspring_population,
        base_extended_population, base_new_population, offspring_selected,
        crossovered_population, mutated_population, offspring_new_population;
    typename INSGA<T>::Fronts fronts, sorted_fronts;
    typename INSGA<T>::Front first_front;

    int iter = 1;
    base_population = random_initialization(popsize, population_gen);
    offspring_population = empty_offspring_population();
    std::cout << "starting insga" << std::endl;
    while (iter <= iterations) {
        std::cout << iter << " iter" << std::endl;
        base_extended_population =
            extend_population(base_population, offspring_population);
        fronts = non_dominance_sorting(base_extended_population, target);
        sorted_fronts = sort_insga(fronts, _w, target);
        base_new_population = elite_parent_selection(sorted_fronts, popsize);
        offspring_selected = selection(base_new_population);
        mutated_population = mutation(mut_strat, offspring_selected, params);
        offspring_new_population = combine(
            offspring_selected, crossovered_population, mutated_population);
        base_population = base_new_population;
        offspring_population = offspring_new_population;
        iter++;
        if (this->logs) {
            this->logs_vec.push_back(offspring_population);
        }
    }
    return get_first_front(fronts);
}

template <typename T>
INSGA<T>::INSGA(QoSCriterion w, bool logs)
    : Evolutionary_Algorithm<T>(logs), _w(w) {}
