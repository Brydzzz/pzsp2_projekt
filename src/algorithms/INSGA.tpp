//
// Created by Marek on 28/12/25.
//

#ifdef __INTELLISENSE__
#include "INSGA.h"
#endif


template<typename T>
typename INSGA<T>::Population
INSGA<T>::random_initialization(int n, population_generator<T> population_gen) const
{

}

template<typename T>
typename INSGA<T>::Population
INSGA<T>::empty_offspring_population() const
{

}


template<typename T>
typename INSGA<T>::Population 
INSGA<T>::extend_population(const typename INSGA<T>::Population& base_population, 
                            const typename INSGA<T>::Population& offspring_population) const
{

}

template<typename T>
typename INSGA<T>::Fronts 
INSGA<T>::non_dominance_sorting(const typename INSGA<T>::Population& base_extended_population) const
{

}

template<typename T>
typename INSGA<T>::Fronts 
INSGA<T>::sort_insga(const typename INSGA<T>::Fronts& fronts, QoSCriterion w) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::elite_parent_selection(const typename INSGA<T>::Fronts& sorted_fronts) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::selection(const typename INSGA<T>::Population& base_new_population) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::crossover_insga(const typename INSGA<T>::Population& offspring_selected, 
                          double crossover_probability) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::mutation(const typename INSGA<T>::Population& offspring_selected, 
                   double mutation_probability) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::combine(const typename INSGA<T>::Population& offspring_selected, 
                  const typename INSGA<T>::Population& crossovered_population, 
                  const typename INSGA<T>::Population& mutated_population) const    
{

}

template<typename T>
typename INSGA<T>::Front 
INSGA<T>::get_first_front(const typename INSGA<T>::Fronts& fronts) const
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::generate_init_pop(population_generator<T> generator,
                            int pop) 
{

}

template<typename T>
typename INSGA<T>::Population 
INSGA<T>::crossover(strategy<T> strat,
                    typename INSGA<T>::Population population,
                    const std::vector<float>& params) 
{

}

template<typename T>
typename INSGA<T>::Front 
INSGA<T>::solve(int popsize, int iterations, target_function<T> target, strategy<T> cross_strat,
                population_generator<T> population_gen, std::vector<float>& params) 
{
    if (params.size() < 2) {
        throw std::invalid_argument("Params vector must contain at least two elements: mutation and crossover probabilities.");
    }
    double mutation_probability = params[0];
    double crossover_probability = params[1];

    typename INSGA<T>::Population base_population, offspring_population, base_extended_population, base_new_population, 
                                  offspring_selected, crossovered_population, mutated_population, offspring_new_population;
    typename INSGA<T>::Fronts fronts, sorted_fronts;
    typename INSGA<T>::Front first_front;

    int iter = 1;
    base_population = random_initialization(popsize, population_gen);
    offspring_population = empty_offspring_population();
    while (iter <= iterations)
    {
        base_extended_population = extend_population(base_population, offspring_population);
        fronts = non_dominance_sorting(base_extended_population);
        sorted_fronts = sort_insga(fronts, _w);
        base_new_population = elite_parent_selection(sorted_fronts);
        offspring_selected = selection(base_new_population);
        crossovered_population = crossover_insga(offspring_selected, crossover_probability);
        mutated_population = mutation(offspring_selected, mutation_probability);
        offspring_new_population = combine(offspring_selected, crossovered_population, mutated_population);
        base_population = base_new_population;
        offspring_population = offspring_new_population;
    }
    return get_first_front(fronts);
}
