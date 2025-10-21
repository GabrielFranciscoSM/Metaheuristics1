#include <cassert>
#include <BLsmall.h>
#include <BMB.h>
#include <iostream>

using namespace std;

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
ResultMH BMB::optimize(Problem *problem, int maxevals)
{
  assert(maxevals > 0);
  LocalSearchEarlyStop lse = LocalSearchEarlyStop();

  tSolution initialSolution = problem->createSolution();

  ResultMH best = lse.optimize(problem, initialSolution, problem->fitness(initialSolution), maxevals-1);

  int totalEvs = best.evaluations;

  for(int i = 0; i < 9; ++i){
    initialSolution = problem->createSolution();
    ResultMH auxResult = lse.optimize(problem, initialSolution, problem->fitness(initialSolution), maxevals-1);
    totalEvs += auxResult.evaluations;
    if (auxResult.fitness > best.fitness) {
      best = auxResult;
    }
  }

  best.evaluations = totalEvs;

  return best;
}
