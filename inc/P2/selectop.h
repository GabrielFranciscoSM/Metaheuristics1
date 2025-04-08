#pragma once
#include <solution.h>
#include <population.h>
#include <random.hpp>

using Random = effolkronium::random_static;

class SelectOp{
    uniform_int_distribution<int> dist_popSize;

    public:

    SelectOp(int popSize);
    pair<tSolution,tFitness> select(Population & pop);
};