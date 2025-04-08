#include <cassert>
#include <greedy.h>
#include <iostream>
#include <pinfluencer.h>
#include <map>

using namespace std;

template <class T>
void print_vector(string name, const vector<T> &sol)
{
  cout << name << ": ";

  for (auto elem : sol)
  {
    cout << elem << ", ";
  }
  cout << endl;
}

/**
 * Find the nodes with most influence (respectively) and choose them
 * as the solution nodes
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH GreedySearch::optimize(Problem *problem, int maxevals)
{
  assert(maxevals > 0);

  ProblemInflu *pi = dynamic_cast<ProblemInflu *>(problem);

  //Ordenamos los nodos por cantidad de vecinos para elegir los mejores
  map<int, int> sizes;
  tSolution sol;

  for (int i = pi->getSolutionDomainRange().first; i < pi->getSolutionDomainRange().second; ++i)
  {
    vector<tDomain> neighvours = pi->getInfluence(i);
    tDomain sum = 0;

    for (auto& n : neighvours)
      sum += pi->getInfluence(n).size();

    sum += pi->getInfluence(i).size();
    
    sizes.insert(make_pair(sum,i));
  }

  for (int i = 0; i < pi->getSolutionSize(); ++i)
  {
    sol.push_back(prev(sizes.end())->second);
    sizes.erase(prev(sizes.end()));
  }

  tFitness fitness = problem->fitness(sol);

  return ResultMH(sol, fitness, 1);
}
