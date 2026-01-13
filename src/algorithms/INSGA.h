//
// Created by Marek on 28/12/25.
//

#pragma once

#include "Evolutionary_Algorithm.h"
#include "MutationOperators.h"

enum class QoSCriterion { Throughput, Loss, Delay };

template <typename T>
struct INSGASortThroughputComparator;

template <typename T>
struct INSGASortLossComparator;

template <typename T>
struct INSGASortDelayComparator;

template <typename T>
class INSGA : public Evolutionary_Algorithm<T> {
    using Population = std::vector<T>;
    using Front = std::vector<T>;
    using Fronts = std::vector<Front>;

  private:
    QoSCriterion _w;

  protected:
    Population
    random_initialization(int n, population_generator<T> population_gen) const;
    Population empty_offspring_population() const;
    Population extend_population(const Population &base_population,
                                 const Population &offspring_population) const;
    Fronts
    non_dominance_sorting(const Population &base_extended_population) const;
    Fronts sort_insga(const Fronts &fronts, QoSCriterion w) const;
    Population elite_parent_selection(const Fronts &sorted_fronts) const;
    Population selection(const Population &base_new_population) const;
    Population crossover_insga(const Population &offspring_selected,
                               double crossover_probability) const;
    Population mutation(const Population &offspring_selected,
                        double mutation_probabilit,
                        strategy<T> mutation_strategy) const;
    Population combine(const Population &offspring_selected,
                       const Population &crossovered_population,
                       const Population &mutated_population) const;
    Front get_first_front(const Fronts &fronts) const;

  public:
    INSGA(QoSCriterion w);
    Population generate_init_pop(population_generator<T> generator,
                                 int pop) override;
    Population crossover(strategy<T> strat, Population population,
                         const std::vector<float> &params) override;
    Front solve(int popsize, int iterations, target_function<T> target,
                strategy<T> cross_strat, population_generator<T> population_gen,
                std::vector<float> &params) override;
};

#include "INSGA.tpp"