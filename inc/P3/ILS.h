#pragma once
#include <mh.h>

using namespace std;

/**
 * Implementation of the Local Search with Early Stop Search metaheuristic
 *  - From a Random solution search for better neighbours, 
 * Stoping if there is no improvement
 *
 * @see MH
 * @see Problem
 */
class ILS : public MH {

public:
  ILS() : MH() {}
  virtual ~ILS() {}
  // Implement the MH interface methods
  /**
   * From a random solution, explore the neighbour solutions changing
   * one node, and chooses to keep the best.
   * This is repeated until the number of evaluations exceed maxevals
   * or the solution doesn´t improve for 20 iterations in a row
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;
  void mutateSol(Problem * prob, tSolution & sol);
};
