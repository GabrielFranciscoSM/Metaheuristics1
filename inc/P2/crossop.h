#pragma once
#include <solution.h>
#include <iostream>
#include <problem.h>
class Crossop{
    public:
    virtual void cross(tSolution & sol1, tSolution & sol2, Problem * prob) = 0;
    virtual ~Crossop() {} // Virtual destructor
};