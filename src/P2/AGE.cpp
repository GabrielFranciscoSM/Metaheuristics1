#include <AGE.h>
#include <iostream>
#include <util.h>

AGE::AGE(cross_operators _crossOp, Problem * problem) : 
    population(Population(POP_SIZE)),
    AG(_crossOp,problem) {

    realDist = std::uniform_real_distribution<float>(0,1);

    this->generatePopulation(problem,this->population);

    int auxBestFit = 0;
    int auxWorstFit = 1000;
    for(int i = 0; i < POP_SIZE; ++i){
        if(population.getFitness(i) > auxBestFit){
            auxBestFit = population.getFitness(i);
            bestSol = i;
        }
        if(population.getFitness(i) < auxWorstFit){
            auxWorstFit = population.getFitness(i);
            worstSol = i;
        }

    }

}

ResultMH AGE::optimize(Problem *problem, int maxevals){
    
    evaluations = 0;

    while(evaluations < maxevals){

        this->select();        
        this->cross(problem);
        this->mutate(problem);

        tFitness fit1 = this->getFitness(problem, sols.first);
        tFitness fit2 = this->getFitness(problem, sols.second);

        if(fit1 < fit2){
            fit1 = fit2;
            sols.first = sols.second;
        }
            

        if(this->population.getFitness(bestSol) < fit1){
            bestSol = worstSol;
        }

        if(this->population.getFitness(worstSol) < fit1){
            this->population.insert_sol(worstSol,sols.first,fit1);
        }
    }

    return ResultMH(this->population.getSolution(bestSol),this->population.getFitness(bestSol),evaluations);
}

void AGE::select(){
    sols.first = selectOp.select(this->population).first;
    sols.second = selectOp.select(this->population).first;
}

void AGE::cross(Problem * problem){
    crosOp->cross(
        this->sols.first,
        this->sols.second,
        problem
    );
}


void AGE::mutate(Problem * problem){
    if(Random::get(realDist) < MUTATE_PROB)
        mutateOp.mutate(sols.first);
    if(Random::get(realDist) < MUTATE_PROB)
        mutateOp.mutate(sols.second);
}

tFitness AGE::getFitness(Problem * prob, tSolution & sol){
    this->evaluations++;
    return prob->fitness(sol);
}