//
// Created by tomek on 12/12/25.
//
#pragma once
#include <functional>
#include <vector>
template <typename T>
using population_generator = std::function<std::vector<T>(int)>;
template <typename T>
using target_function = std::function<std::vector<float>(T)>;
template <typename T>
using strategy = std::function<std::vector<T>(const std::vector<T> &,
                                              std::vector<float> params)>;

template <typename T>
class Evolutionary_Algorithm {
  public:
    virtual ~Evolutionary_Algorithm() = default;
    virtual std::vector<T> generate_init_pop(population_generator<T> generator,
                                             int pop) = 0;
    // virtual std::vector<T> selection(strategy<T> strat,
    //                                  std::vector<T> population,
    //                                  const std::vector<float>& params);
    virtual std::vector<T> mutation(strategy<T> strat,
                                     std::vector<T> population,
                                     const std::vector<float> &params) = 0;
    // virtual std::vector<T> succession(strategy<T> strat,
    //                                   std::vector<T> population,
    //                                   const std::vector<float>& params);
    virtual std::vector<T> solve(int popsize, int iterations,
                                 target_function<T> target,
                                 strategy<T> cross_strat,
                                 population_generator<T> population_gen,
                                 std::vector<float> &params) = 0;
};
