//
// Created by tomek on 12/12/25.
//
# pragma once
#include <vector>

template <typename T>
class Evol_Algo {
   int population_size = 0;
public:
    virtual std::vector<T> generate_init_pop();
    virtual std::vector<T> selection(std::vector<T>(*sel_strat)(std::vector<T>,std::vector<int>));
    virtual std::vector<T> mutation(std::vector<T>(*mut_strat)(std::vector<T>,std::vector<int>));
    virtual std::vector<T> crossover(std::vector<T>(*cross_strat)(std::vector<T>,std::vector<int>));
    virtual std::vector<T> succsesion(std::vector<T>(*succ_strat)(std::vector<T>,std::vector<int>));
};