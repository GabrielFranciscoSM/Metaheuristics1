#pragma once
#include <solution.h>
#include <problem.h> 
#include <random.hpp>

using Random = effolkronium::random_static;

class MutateOp{
    
    std::uniform_int_distribution<int> dist_range;
    std::uniform_int_distribution<int> dist_size;

    public:

    MutateOp(Problem * prob);
    void mutate(tSolution & sol);
};