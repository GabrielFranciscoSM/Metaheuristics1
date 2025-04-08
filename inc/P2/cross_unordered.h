#pragma once
#include <crossop.h>
#include <random.hpp>
#include <problem.h>

using Random = effolkronium::random_static;

class CrossUnordered : public virtual Crossop{

    const int MIN_LENGTH = 2;
    std::uniform_int_distribution<int> dist_SizeSol;

    public:
    CrossUnordered(Problem * prob);
    ~CrossUnordered() {}// Virtual destructor

    void cross(tSolution & sol1, tSolution & sol2,Problem * prob) override;
};