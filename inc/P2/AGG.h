#pragma once
#include <AG.h>

class AGG : virtual public AG{

private:

    int nMutate;
    int nCross;

    Population population[2];

    int workingPop;
    
public:

    AGG(cross_operators _crossOp, Problem * Problem);
    virtual ~AGG() = default;

    ResultMH optimize(Problem *problem, int maxevals) override;

//private:
    Population & getPopulation(int i);
    Population & getWorkingPopulation();
    void updateFitness(int i, Problem * prob);
    
    void select() override;

    void cross(Problem * problem) override;

    void mutate(Problem * problem) override;

};