#pragma once
#include <mhtrayectory.h>

using namespace std;

/**
 * Implementation of the Local Search metaheuristic
 *  - From a random solution search for the best first in 
 * its neightborhood
 *
 * @see MH
 * @see Problem
 */
class LocalSearchLl : public MHTrayectory {

public:
    LocalSearchLl() : MHTrayectory() {}
    virtual ~LocalSearchLl() {}
  // Implement the MH interface methods
  /**
   * From a random solution, generate a neighbour solution changing
   * a node from one outside the solution, compare both solutions
   * and pick the best
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) override;
};
