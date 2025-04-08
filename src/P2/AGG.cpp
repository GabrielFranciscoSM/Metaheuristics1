#include <AGG.h>
#include <iostream>
#include <util.h>

AGG::AGG(cross_operators _crossOp, Problem * problem) : 
    population{Population(POP_SIZE), Population(POP_SIZE)},
    AG(_crossOp,problem) {

    nMutate = ceil(MUTATE_PROB*POP_SIZE);
    nCross = ceil(CROSS_PROB*POP_SIZE/2) * 2;

    workingPop = 1;
    this->generatePopulation(problem,this->population[0]);

}

ResultMH AGG::optimize(Problem *problem, int maxevals){
    int bestSolI = 0;
    float bestFit = 0;
    tSolution bestSol;
    
    while(this->evaluations < maxevals){
        cerr << "EVALUATIONS: " << evaluations << ", Working pop: " << workingPop <<  endl;
        cerr << "OLD POP: " << endl;
        this->getWorkingPopulation().print_pop();

        this->select();
        cerr << "SELECT POP: " << endl;
        this->getWorkingPopulation().print_pop();
        
        this->cross(problem);
        cerr << "CROSS POP: " << this->nCross << endl;
        this->getWorkingPopulation().print_pop();

        this->mutate(problem);
        cerr << "MUTATE POP: " << this->nMutate << endl;
        this->getWorkingPopulation().print_pop();


        workingPop = (workingPop+1)%2;
    }

    for(int i = 0; i < POP_SIZE; ++i){
        if(this->getPopulation((workingPop+1)%2).getFitness(i) > bestFit){
            bestFit = this->getPopulation((workingPop+1)%2).getFitness(i);
            bestSolI = i;
        }
    }

    return ResultMH(bestSol,bestFit,10);
}


void AGG::select(){
    pair<tSolution,tFitness> sol_fit;

    for(int i = 0; i < POP_SIZE; ++i){
        sol_fit = selectOp.select(this->getPopulation((workingPop+1)%2));

        this->getWorkingPopulation().insert_sol(i,sol_fit.first,sol_fit.second);
    }
}

void AGG::cross(Problem * problem){
    for(int i = 0; i < nCross; i += 2){
        crosOp->cross(
            this->getWorkingPopulation().getSolution(i), 
            this->getWorkingPopulation().getSolution(i + 1),
            problem
        );
        this->updateFitness(i,problem);
        this->updateFitness(i+1,problem);
    }
}

void AGG::mutate(Problem * problem){
    for(int i = 0; i < nMutate; ++i){

        mutateOp.mutate(this->getWorkingPopulation().getSolution(i));

        this->updateFitness(i,problem);
    }
}

Population & AGG::getPopulation(int i){
    return population[i];
}

Population & AGG::getWorkingPopulation(){
    return this->getPopulation(workingPop);
}

void AGG::updateFitness(int i, Problem * problem){
    this->getWorkingPopulation().set_fitness(i,problem->fitness(this->getWorkingPopulation().getSolution(i)));
    evaluations++;
}