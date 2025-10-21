#include <cassert>
#include <GRASP.h>
#include <BLsmall.h>
#include <lsall.h>
#include <iostream>
#include <pinfluencer.h>
#include <map>
#include <set>

using namespace std;

// template <class T>
// void print_vector(string name, const vector<T> &sol)
// {
//   cout << name << ": ";

//   for (auto elem : sol)
//   {
//     cout << elem << ", ";
//   }
//   cout << endl;
// }

/**
 * Find the nodes with most influence (respectively) and choose them
 * as the solution nodes
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH GRASP::optimizeOne(Problem *problem, int maxevals)
{
  assert(maxevals > 0);

  std::uniform_real_distribution<float> real_dist(0,1);

  ProblemInflu *pi = dynamic_cast<ProblemInflu *>(problem);

  vector<vector<int>> graph = pi->getGraph();

  vector<int> vecinos;
  vector<int> vecinosDeVecinos;

  vector<int> LC;

  //CREATE LC

  for(int i = 0; i < graph.size(); ++i){
    vecinos.push_back(graph.at(i).size());

    LC.push_back(i);
  }

  for(int i = 0; i < vecinos.size(); ++i){
    
    vecinosDeVecinos.push_back(vecinos.at(i));
    for(int j = 0; j < graph.at(i).size(); ++j){
      vecinosDeVecinos.back() += vecinos.at(graph.at(i).at(j));
    }
  }

  //TAKE ONE NODE

  Random::shuffle(LC);

  tSolution sol;

  int node = LC.back();
  int vecinosNode = vecinos.at(node);

  //RECALCULATE LC

  set<int> vecinosDeVecinosDeNode;

  sol.push_back(node);

  for(int i = 0; i < graph.at(node).size(); ++i){
    int vecinoDeNodo = graph.at(node).at(i); 
    vecinosDeVecinos.at(vecinoDeNodo) -= (vecinosNode + 1);
    vecinos.at(vecinoDeNodo)--;

    for(int j = 0; j < graph.at(vecinoDeNodo).size(); ++j){
      vecinosDeVecinosDeNode.insert(graph.at(vecinoDeNodo).at(j));
    }
  }

  for (const auto& elemento : vecinosDeVecinosDeNode) {
    vecinosDeVecinos.at(elemento)--;
  }

  LC.pop_back();

  

  for(int i = 1; i < pi->getSolutionSize(); ++i){

    //GENERATE LRC

    vector<int> LRC;

    std::sort(LC.begin(),LC.end(),
    [&](int &a, int &b){ return vecinosDeVecinos.at(a) > vecinosDeVecinos.at(b); });

    float htop = vecinosDeVecinos.at(LC.front());
    float hbot = vecinosDeVecinos.at(LC.back());

    float mu = htop - Random::get(real_dist)*(htop-hbot);

    int iteratorHeuristic = 0;
    bool condition = true;

    while(iteratorHeuristic < LC.size() && condition){
      if(vecinosDeVecinos.at(LC.at(iteratorHeuristic)) < mu){
        condition = true;
      }
      else{
        LRC.push_back(LC.at(iteratorHeuristic));
      }

      iteratorHeuristic++;
    }

    //INSERT NEW NODE
    Random::shuffle(LRC);

    node = LRC.back();
    vecinosNode = vecinos.at(node);

    sol.push_back(node);

    for(int i = 0; i < graph.at(node).size(); ++i){
      int vecinoDeNodo = graph.at(node).at(i); 
      vecinosDeVecinos.at(vecinoDeNodo) -= (vecinosNode + 1);
      vecinos.at(vecinoDeNodo)--;
  
      for(int j = 0; j < graph.at(vecinoDeNodo).size(); ++j){
        vecinosDeVecinosDeNode.insert(graph.at(vecinoDeNodo).at(j));
      }
    }
  
    for (const auto& elemento : vecinosDeVecinosDeNode) {
      vecinosDeVecinos.at(elemento)--;
    }
  
    LC.erase(std::find(LC.begin(),LC.end(),LRC.back()));
    LRC.pop_back();

  }

  return ResultMH(sol, problem->fitness(sol), 1);
}

ResultMH GRASP::optimizeNOBL(Problem *problem, int maxevals){
  ResultMH bestResult = this->optimizeOne(problem,maxevals);

  for(int i = 1; i < 10; ++i){
    ResultMH result = this->optimizeOne(problem,maxevals);

    if(bestResult.fitness < result.fitness){
      bestResult = result;
    }
  }

  bestResult.evaluations = 10;

  return bestResult;
}

ResultMH GRASP::optimizeSIBL(Problem *problem, int maxevals){
  LocalSearchEarlyStop lse = LocalSearchEarlyStop();
  int totalEvs = 10;

  ResultMH bestResult = this->optimizeOne(problem,maxevals);
  bestResult = lse.optimize(problem, bestResult.solution, problem->fitness(bestResult.solution), 99);
  totalEvs += bestResult.evaluations;

  for(int i = 1; i < 10; ++i){
    tSolution resultSol = this->optimizeOne(problem,maxevals).solution;

    ResultMH result = lse.optimize(problem, resultSol, problem->fitness(resultSol), 99);
    totalEvs += result.evaluations;

    if(bestResult.fitness < result.fitness){
      bestResult = result;
    }
  }

  bestResult.evaluations = totalEvs;

  return bestResult;
}

ResultMH GRASP::optimizeSIBLALL(Problem *problem, int maxevals){
  LocalSearchLl ls = LocalSearchLl();
  int lsEvals = (maxevals - 10)/10;
  int totalEvs = 10;

  ResultMH bestResult = this->optimizeOne(problem,maxevals);
  bestResult = ls.optimize(problem, bestResult.solution, problem->fitness(bestResult.solution), lsEvals);
  totalEvs += bestResult.evaluations;

  for(int i = 1; i < 10; ++i){
    tSolution resultSol = this->optimizeOne(problem,maxevals).solution;

    ResultMH result = ls.optimize(problem, resultSol, problem->fitness(resultSol), lsEvals);
    totalEvs += result.evaluations;

    if(bestResult.fitness < result.fitness){
      bestResult = result;
    }
  }

  bestResult.evaluations = totalEvs;

  return bestResult;
}

ResultMH GRASP::optimize(Problem * problem, int maxevals){
  if(this->gt == graspType::NOBL){
    return this->optimizeNOBL(problem,maxevals);
  }
  else if(this->gt == graspType::SIBL){
    return this->optimizeSIBL(problem,maxevals);
  }
  else{
    return this->optimizeSIBLALL(problem,maxevals);
  }
}