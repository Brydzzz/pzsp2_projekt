//
// Created by tomek on 12/12/25.
//
# pragma once
#include <vector>
#include <functional>
//T will represent the individual, strategies need to be parsed into the functions below as function pointers and they require the population and parameteres, which for now are int only because maybe that will be enough
template <typename T>
using strategy = std::function<std::vector<T>(const std::vector<T> &)>;
template <typename T>
using target_function = std::function<std::vector<float>(T)>;
template <typename T>
using population_generator = std::function<std::vector<T>(int)>;

template <typename T>
class Evolutionary_Algorithm {
public:
    virtual ~Evolutionary_Algorithm() = default;
    virtual std::vector<T> generate_init_pop(population_generator);
    virtual std::vector<T> selection(strategy strat, std::vector<T> population,
                                     const std::vector<float> &params);
    virtual std::vector<T> crossover(strategy strat, std::vector<T> population,
                                     const std::vector<float> &params);
    virtual std::vector<T> succsesion(strategy strat, std::vector<T> population,
                                      const std::vector<float> &params);
    virtual T solve(target_function target, population_generator population_gen,
                    strategy select_strat, strategy cross_stat,
                    strategy succ_strat, std::vector<float> &params);
};