//
// Created by tomek on 12/12/25.
//
# pragma once
#include <vector>
#include <functional>

template <typename T>
using population_generator = std::function<std::vector<T>(int)>;
template <typename T>
using target_function = std::function<std::vector<float>(T)>;


template <typename T>
class Evolutionary_Algorithm
{
public:
    virtual ~Evolutionary_Algorithm() = default;
    virtual std::vector<T> generate_init_pop(population_generator<T> generator,
                                             int pop);
    // virtual std::vector<T> selection(strategy<T> strat,
    //                                  std::vector<T> population,
    //                                  const std::vector<float>& params);
    // virtual std::vector<T> crossover(strategy<T> strat,
    //                                  std::vector<T> population,
    //                                  const std::vector<float>& params);
    // virtual std::vector<T> succession(strategy<T> strat,
    //                                   std::vector<T> population,
    //                                   const std::vector<float>& params);
    virtual T solve(int popsize, target_function<T> target,
                    population_generator<T> population_gen, std::vector<float>& params);
};
