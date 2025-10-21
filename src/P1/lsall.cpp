#include <cassert>
#include <lsall.h>
#include <iostream>

using namespace std;

ResultMH LocalSearchLl::optimize(Problem * problem, const tSolution & current,tFitness fitness, int maxevals){
  assert(maxevals > 0);

  tSolution best = current;
  float best_fitness = problem->fitness(best);

  // We pick a random node, checking that it isn't in the solution
  uniform_int_distribution<int> distRang(problem->getSolutionDomainRange().first, 
                                    problem->getSolutionDomainRange().second-1);
  uniform_int_distribution<int> distSol(0, (int)(problem->getSolutionSize() - 1));

  for (int i = 0; i < maxevals-1; i++)
  {

    int newNode = Random::get(distRang);

    // Ineficient but aceptable in small cases
    while (find(best.begin(), best.end(), newNode) != best.end())
    {
      newNode = Random::get(distRang);
    }

    // Esto es aleatorio:
    // Se podria ver cual de los nodos tiene menor influencia para hacer un cambio más eficiente??
    // We change teh solution to compute the new fitness, and we keep the best

    int oldNodePos = Random::get(distSol);
    int oldNode = best.at(oldNodePos);

    best.at(oldNodePos) = newNode;

    float fitness = problem->fitness(best);

    if (fitness <= best_fitness)
    {
      best.at(oldNodePos) = oldNode;
    }
    else
    {
      best_fitness = fitness;
    }

  }

  return ResultMH(best, best_fitness, maxevals);
}
