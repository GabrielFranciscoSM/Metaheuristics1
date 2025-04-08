#pragma once
#include <mh.h>
#include <population.h>
#include <crossop.h>
#include <cross_ordered.h>
#include <cross_unordered.h>
#include <selectop.h>
#include <problem.h>
#include <mutateOp.h>


class AG : public MH{
public:
    enum cross_operators {ordered,unordered};

protected:
    const float CROSS_PROB = 0.7;
    const float MUTATE_PROB = 0.1;
    const int POP_SIZE = 30;

    Crossop * crosOp;
    SelectOp selectOp;
    MutateOp mutateOp;

    int evaluations;
    
public:

    AG(cross_operators _crossOp, Problem * Problem);
    virtual ~AG() = default;

    virtual ResultMH optimize(Problem *problem, int maxevals) = 0;

//private:
    
    void generatePopulation(Problem * problem, Population & population);

    virtual void select() {};

    virtual void cross(Problem * problem) {};

    virtual void mutate(Problem * problem) {};

};