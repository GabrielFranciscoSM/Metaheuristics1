#pragma once
#include <algorithm>
#include <problem.h>
#include <random.hpp>
#include <fstream>

using namespace std;

class ProblemInflu : public Problem {

  /// @brief Probility of influence node to node
  const float PROB_INFLU = 0.01;

  /// @brief Size of the solution
  size_t size;

  /// @brief Vector the vecinos de cada nodo
  vector<vector<int>> graph;

  int evs;
public:

  //----------CONSTRUCTORES----------
  
  //USAR SOLO PARA TESTEAR
  ProblemInflu(size_t size): size(size) {
    this->generateRandomGraph();
  }
  
  ProblemInflu(size_t size, ifstream &file): size(size) {
    this->setData(file);
  }

  //----------Getters----------

  size_t getSolutionSize() { return size; }
  pair<tDomain, tDomain> getSolutionDomainRange() override;

  /// @brief Getter para los vecinos de un nodo
  /// @param node 
  /// @return Vector de los vecinos del nodo
  vector<tDomain> getInfluence(int node){return graph.at(node);}

  //----------Problem Functions----------

  tFitness fitness(const tSolution &solution) override;
  tSolution createSolution() override;

private:


  /// @brief Generate a random small graph (size 1000)
  void generateRandomGraph();

  /// @brief Reads the graph data from a file and inicialize it
  /// @param file 
  /// @return 0 if everything is ok, 1 otherwise
  int setData(ifstream &file);

};
