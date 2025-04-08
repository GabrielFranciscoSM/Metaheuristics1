#include <cassert>
#include <BLsmall.h>
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
ResultMH LocalSearchEarlyStop::optimize(Problem *problem, int maxevals)
{
  assert(maxevals > 0);

  tSolution best = problem->createSolution();
  float best_fitness = problem->fitness(best);

  int noImprove = 0;
  int i = 0;

  // We pick a random node, checking that it isn't in the solution
  uniform_int_distribution<int> distRang(problem->getSolutionDomainRange().first,
                                      problem->getSolutionDomainRange().second-1);
  uniform_int_distribution<int> distSol(0, (int)(problem->getSolutionSize() - 1));

  while (i < maxevals && noImprove < 20)
  {

    int newNode = Random::get(distRang);

    // Ineficient but aceptable in small cases
    while (find(best.begin(), best.end(), newNode) != best.end())
    {
      newNode = Random::get(distRang);
    }

    // We change teh solution to compute the new fitness, and we keep the best

    int oldNodePos = Random::get(distSol);
    int oldNode = best.at(oldNodePos);

    best.at(oldNodePos) = newNode;

    float fitness = problem->fitness(best);

    if (fitness <= best_fitness)
    {
      best.at(oldNodePos) = oldNode;
      ++noImprove;
    }
    else
    {
      best_fitness = fitness;
      noImprove = 0;
    }

    ++i;
  }

  return ResultMH(best, best_fitness, i);
}
