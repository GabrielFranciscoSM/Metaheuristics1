#include <population.h>
#include <util.h>
#include <algorithm>

Population::Population(size_t size){
    this->initializePop(size);
}

void Population::initializePop(size_t size){
    population.resize(size);
    fitness.resize(size);
}

void Population::push_back_sol(tSolution & sol,tFitness fit){
    population.push_back(sol);
    fitness.push_back(fit);
}

void Population::insert_sol(int i, tSolution & sol, tFitness fit){
    population.at(i) = sol;
    fitness.at(i) = fit;
}

void Population::set_fitness(int i, tFitness fit){
    fitness.at(i) = fit;
}

void Population::print_pop(){
    for(int i = 0; i < this->population.size(); ++i){
        std::cout << this->population.at(i) << endl;
    }
}

bool Population::findSolution(tSolution & sol){
    return (find(this->population.begin(),this->population.end(),sol) == this->population.end());
}

vector<int> Population::getSortedIndex(){

    vector<int> index(this->getPopulationSize());

    for(int i = 0; i < this->getPopulationSize(); ++i){
        index.at(i) = i;
    }


    sort(index.begin(),index.end(),[this](int &a, int &b){ return this->getFitness(a) > this->getFitness(b); });

    return index;
}