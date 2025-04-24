#include <AGG.h>
#include <iostream>
#include <util.h>

AGG::AGG(cross_operators _crossOp, Problem * problem) : 
    population{Population(POP_SIZE), Population(POP_SIZE)},
    AG(_crossOp,problem) {

    nMutate = ceil(MUTATE_PROB*POP_SIZE);
    nCross = ceil(CROSS_PROB*POP_SIZE/2) * 2;
}

ResultMH AGG::optimize(Problem *problem, int maxevals){
    this->generatePopulation(problem,this->population[0]);
    workingPop = 1;

    tFitness bestSolI = 0;
    float bestFit = 0;
    tSolution bestSol;

    evaluations = 0;
    
    while(evaluations < maxevals){
        this->select();
        this->cross(problem);
        this->mutate(problem);

        int auxWorstFitness = bestFit;
        int auxWorstIt = 0;
        bool downgraded = true;

        for(int i = 0; i < POP_SIZE; ++i){
            if(this->getWorkingPopulation().getFitness(i) >= bestFit){
                downgraded = false;
                bestFit = this->getWorkingPopulation().getFitness(i);
                bestSol = this->getWorkingPopulation().getSolution(i);
            }

            if(this->getWorkingPopulation().getFitness(i) <= auxWorstFitness){
                auxWorstFitness = this->getWorkingPopulation().getFitness(i);
                auxWorstIt = i;
            }
        }

        if(downgraded){
            this->getWorkingPopulation().insert_sol(auxWorstIt,bestSol,bestFit);
        }


        workingPop = (workingPop+1)%2;
    }

    return ResultMH(bestSol,bestFit,evaluations);
}


void AGG::select(){
    pair<tSolution,tFitness> sol;

    for(int i = 0; i < POP_SIZE; ++i){
        sol = selectOp.select(this->getPopulation((workingPop+1)%2));

        this->getWorkingPopulation().insert_sol(i,sol.first,sol.second);
    }
}

void AGG::cross(Problem * problem){
    for(int i = 0; i < nCross; i += 2){
        
        //cross cambia las soluciones
        crosOp->cross(
            this->getWorkingPopulation().getSolution(i), 
            this->getWorkingPopulation().getSolution(i + 1),
            problem
        );
        
        //Siempre que se cambien las soluciones se actualiza el fitness
        this->updateFitness(i,problem);
        this->updateFitness(i+1,problem);
        
    }
}

void AGG::mutate(Problem * problem){
    for(int i = 0; i < nMutate; ++i){

        //Se cambia la solucion
        mutateOp.mutate(this->getWorkingPopulation().getSolution(i));

        //Siempre que se cambie la solucion se actualiza el fitness
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