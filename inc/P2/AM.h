#pragma once

#include <AGG.h>
#include <lsall.h>

class AM : public AGG{
    
private:

    int LSfase;
    int LSsize;
    bool orderPob;
    int LSmaxEvals;

    LocalSearchLl LS;

public:

    AM(int LSfase, float LSpercentage, bool orderPob, int LSmaxEvals,cross_operators _crossOp, Problem * problem);
    virtual ~AM() = default;

    ResultMH optimize(Problem *problem, int maxevals) override;
    int applyLocalSearch(Problem *problem, int evaluations, int maxEvals);
};