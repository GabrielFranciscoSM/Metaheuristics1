#include <cassert>
#include <BLsmall.h>
#include <ILS.h>
#include <iostream>
#include <algorithm>

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
ResultMH ILS::optimize(Problem *problem, int maxevals)
{
  assert(maxevals > 0);
  LocalSearchEarlyStop lse = LocalSearchEarlyStop();

  tSolution initialSolution = problem->createSolution();

  ResultMH best = lse.optimize(problem, initialSolution, problem->fitness(initialSolution), maxevals-1);
  
  initialSolution = best.solution;
  this->mutateSol(problem,initialSolution);

  int totalEvs = best.evaluations;

  for(int i = 0; i < 9; ++i){
    
    ResultMH auxResult = lse.optimize(problem, initialSolution, problem->fitness(initialSolution), maxevals-1);
    
    initialSolution = auxResult.solution;
    this->mutateSol(problem,initialSolution);
    
    totalEvs += auxResult.evaluations;
    
    if (auxResult.fitness > best.fitness) {
      best = auxResult;
    }
  }

  best.evaluations = totalEvs;

  return best;
}

void ILS::mutateSol(Problem * prob, tSolution & sol){
  std::uniform_int_distribution<int> dist_range = std::uniform_int_distribution<int>(
    prob->getSolutionDomainRange().first,
    prob->getSolutionDomainRange().second-1
  );

  vector<int> sol_pos(10);

  for(int i = 0; i < sol_pos.size(); ++i){
    sol_pos.at(i) = i;
  }
  
  Random::shuffle(sol_pos);

  sol.at(sol_pos.at(0)) = Random::get(dist_range);
  sol.at(sol_pos.at(1)) = Random::get(dist_range);

}
