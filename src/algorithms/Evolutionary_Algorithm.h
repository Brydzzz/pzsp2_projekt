//
// Created by tomek on 12/12/25.
//
# pragma once
#include <vector>
#include <functional>
//T will represent the individual, strategies need to be parsed into the functions below as function pointers and they require the population and parameteres, which for now are int only because maybe that will be enough
template <typename T>
class Evol_Algo {
   int population_size = 0;
    using strategy = std::function<std::vector<T>(const std::vector<T>&)>;
public:
    virtual std::vector<T> generate_init_pop();
    virtual std::vector<T> selection(strategy, std::vector<T> population, const std::vector<int>& params);
    virtual std::vector<T> mutation(strategy,std::vector<T> population, const std::vector<int>& params);
    virtual std::vector<T> crossover(strategy,std::vector<T> population, const std::vector<int>& params);
    virtual std::vector<T> succsesion(strategy,std::vector<T> population, const std::vector<int>& params);
};