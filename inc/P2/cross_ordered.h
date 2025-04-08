#pragma once
#include <crossop.h>

class CrossOrdered : public virtual Crossop{
    public:
    CrossOrdered() : Crossop() {};
    ~CrossOrdered() {};
    
    void cross(tSolution & sol1, tSolution & sol2, Problem * prob) override;
};