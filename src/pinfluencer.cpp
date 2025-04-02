#include <algorithm>
#include <pinfluencer.h>
#include <random.hpp>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

tFitness ProblemInflu::fitness(const tSolution &solution)
{

  // 314 is an arbitrary seed
  mt19937 rngfit(234670);

  float totalNodos = 0;

  for(int j = 0; j < 10; ++j){

    unordered_set<tDomain> nodosTotales;
    queue<tDomain> nodosTemp;
  
    uniform_real_distribution<float> dist(0, 1);
  
    // Inizialitation of Total nodes infected (nodosTotales)
    // and nodes "waitng to infect"(nodosTemp)
    for (int i = 0; i < solution.size(); i++)
    {
      nodosTotales.insert(solution.at(i));
      nodosTemp.push(solution.at(i));
    }
  
    while (!nodosTemp.empty())
    {
  
      // Pick node waiting to infect
      tDomain aux = nodosTemp.front();
      nodosTemp.pop();
  
      // Try to infect its neightbours
      for (int i = 0; i < graph.at(aux).size(); ++i)
      {
        // Make sure they are not already infected
        if (nodosTotales.find(graph.at(aux).at(i)) == nodosTotales.end())
        {
          // Aplay chance to infect
          if (dist(rngfit) < this->PROB_INFLU)
          {
            nodosTotales.insert(graph.at(aux).at(i));
            nodosTemp.push(graph.at(aux).at(i));
          }
        }
      }
    }

    totalNodos += nodosTotales.size();

  }

  return (tFitness)(totalNodos/10.0);
}

tSolution ProblemInflu::createSolution()
{

  tSolution solution(this->getSolutionSize());
  uniform_int_distribution<int> dist_int(0, getSolutionDomainRange().second - 1);

  for (int i = 0; i < this->getSolutionSize(); i++)
  {

    int aux = Random::get(dist_int);

    // For the solution to be valid it needs to have not repeated nodes
    while (find(solution.begin(), solution.end(), aux) != solution.end())
    {
      aux = Random::get(dist_int);
    }
    solution[i] = aux;
  }

  return solution;
}

pair<tDomain, tDomain> ProblemInflu::getSolutionDomainRange()
{
  return make_pair(0, graph.size());
}

/// @brief Generate a random small graph (size 1000)
void ProblemInflu::generateRandomGraph()
{
  // Distribution of posible neighbours
  uniform_int_distribution<int> dist_int1(0, 10);

  uniform_int_distribution<int> dist_int2(0, 1000);

  int graphSize = 1000;

  for (int i = 0; i < graphSize; ++i)
  {
    vector<int> aux;

    int isize = Random::get(dist_int1);

    for (int j = 0; j < isize; ++j)
    {
      aux.push_back(Random::get(dist_int2));
    }
    graph.push_back(aux);
  }
}

int ProblemInflu::setData(ifstream &file)
{

  if (!file.is_open())
  {
    std::cerr << "Failed to open the file." << std::endl;
    return 1;
  }

  graph.clear();

  string line;

  getline(file, line);
  getline(file, line);
  getline(file, line);
  istringstream iss(line);

  string aux;
  int nodes;
  iss >> aux;
  iss >> aux;
  iss >> nodes;
  graph.resize(nodes, vector<int>());

  getline(file, line);

  while (getline(file, line))
  {
    istringstream iss(line);
    int fromNodeId, toNodeId;

    if (iss >> fromNodeId >> toNodeId)
    {
      graph.at(fromNodeId).push_back(toNodeId);
    }
    else
    {
      std::cerr << "Failed to parse line: " << line << std::endl;
    }
  }

  file.close();

  return 0;
}
