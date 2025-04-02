#pragma once
#include <heuristicmh.h>

using namespace std;

/**
 * Implementation of the Greedy Search metaheuristic
 *  - Choose the nodes with most influence
 *
 * @see MH
 * @see Problem
 */
class GreedySearch : public MH {

public:
  GreedySearch() : MH() {}
  virtual ~GreedySearch() {}
  // Implement the MH interface methods
  /**
   * Find the nodes with most influence (respectively) and choose them
   * as the solution nodes
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;
};
