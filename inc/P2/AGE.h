#pragma once
#include <AG.h>

class AGE : virtual public AG{

private:

    Population population;
    std::pair<tSolution, tSolution> sols;

    int bestSol;
    int worstSol;

    std::uniform_real_distribution<float> realDist;
    
public:

    AGE(cross_operators _crossOp, Problem * Problem);
    virtual ~AGE() = default;

    ResultMH optimize(Problem *problem, int maxevals) override;

//private:
    tFitness getFitness(Problem * prob, tSolution & sol);    

    void select() override;

    void cross(Problem * problem) override;

    void mutate(Problem * problem) override;

};