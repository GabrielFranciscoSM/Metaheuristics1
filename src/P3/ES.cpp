#include <cassert>
#include <BLsmall.h>
#include <ES.h>
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
ResultMH ES::optimize(Problem *problem, int maxevals)
{

  uniform_int_distribution<int> distRang(problem->getSolutionDomainRange().first, 
                                    problem->getSolutionDomainRange().second-1);
  uniform_int_distribution<int> distSol(0, (int)(problem->getSolutionSize() - 1));

  uniform_real_distribution<float> distProb(0.0, 1.0);

  assert(maxevals > 0);

  tSolution BestoSol = problem->createSolution();
  float bestFit = problem->fitness(BestoSol);

  tSolution sol = BestoSol;
  float solFit = bestFit;

  float temperaturaInicial = (mu*solFit)/(-log(phi));
  float temperaturaFinal = 0.001;

  float temperatura = temperaturaInicial;
  while(temperaturaFinal > temperaturaInicial){
    temperaturaFinal *= 0.1;
  }

  float max_vecinos = 700; // 5 * problemSize;
  float max_exitos = 0.1*max_vecinos;
  float M = (float)maxevals / max_vecinos; //numero de enfriamientos

  float beta = (temperaturaInicial - temperaturaFinal) / (M * temperaturaFinal*temperaturaInicial);

  int evals = 1;

  int m = 0;

  float exitos = 1;


  while (m < M && exitos > 0){
    exitos = 0;
    int vecinos = 0;

    while(exitos < max_exitos && vecinos < max_vecinos && evals < maxevals){
      int newNode = Random::get(distRang);

      // Ineficient but aceptable in small cases
      while (find(sol.begin(), sol.end(), newNode) != sol.end())
      {
        newNode = Random::get(distRang);
      }
  
      int oldNodePos = Random::get(distSol);
      int oldNode = sol.at(oldNodePos);
  
      sol.at(oldNodePos) = newNode;
  
      float newfitness = problem->fitness(sol);
      float oldFitness = solFit;

      float difFitness = oldFitness - newfitness;

      vecinos++;
      evals++;

      if(difFitness < 0 || Random::get(distProb) <= exp(-difFitness/temperatura)){

        exitos++;
        solFit = newfitness;

        if(bestFit < solFit){
          BestoSol = sol;
          bestFit = solFit;
        }
      }
      else{
        sol.at(oldNodePos) = oldNode;
      }
    }

    temperatura = temperatura / (1 + beta * temperatura);
    m++;
  }
  
  return ResultMH(BestoSol, bestFit, evals);
}

