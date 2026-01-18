//
// Created by Marek on 11/01/26.
//

#pragma once

#include "Evolutionary_Algorithm.h"
#include "Individual.h"

std::vector<Individual>
INSGAMutationVariantAStrategy(const std::vector<Individual> &xs,
                              const std::vector<float> &params);
