#pragma once

#include <pareto/front.h>
#include <pareto/kd_tree.h>
#include <vector>

#include "Evolutionary_Algorithm.h"

template <typename T>
class NSGA2 : public Evolutionary_Algorithm<T> {
  protected:
    // std::function<float(T, T)> distance_function;
    std::vector<T> join_vector(std::vector<T> &v1, std::vector<T> &v2);

    std::vector<T> generate_init_pop(population_generator<T> generator,
                                     int pop) override;

    std::vector<T> crossover(strategy<T> strat, std::vector<T> population,
                             const std::vector<float> &params) override;

    std::vector<T> mutate(std::vector<T> &population,
                          const std::vector<float> &params);
    std::vector<T> selection(std::vector<T> &population,
                             target_function<T> target,
                             const std::vector<float> &params);

    std::vector<std::vector<T>>
    sort_nondominated_algorithm(std::vector<T> &population,
                                target_function<T> target);

    std::vector<float> crowding_distance(std::vector<T> &population,
                                         target_function<T> &target);

  public:
    NSGA2() {}
    // : distance_function(distance_function) {}
    std::vector<T> solve(int popsize, int iterations, target_function<T> target,
                         strategy<T> cross_strat,
                         population_generator<T> population_gen,
                         std::vector<float> &params) override;
};

#include "NSGA2.tpp"