//
// Created by Marek on 11/01/26.
//

#pragma once

#include "Individual.h"
#include "Evolutionary_Algorithm.h"

std::vector<Individual>
INSGAMutationVariantAStrategy(const std::vector<Individual> &xs,
                              const std::vector<float> &params);
