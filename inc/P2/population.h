#pragma once
#include <solution.h>
#include <iostream>

using namespace std;

class Population {

  vector<tSolution> population;

  vector<tFitness> fitness;

public:

  Population(size_t size);

  void initializePop(size_t size);

  /*------------GETTERS------------*/

  size_t getPopulationSize() { return population.size(); }

  tFitness getFitness(int i){return fitness.at(i);}

  tSolution & getSolution(int i){return population.at(i);}

  bool findSolution(tSolution sol);

  /*------------OPERATORS------------*/

  void push_back_sol(tSolution & sol,tFitness fit);

  void insert_sol(int i, tSolution & sol, tFitness fit);

  void set_fitness(int i, tFitness fit);

  void print_pop();

  vector<int> getSortedIndex();
};
