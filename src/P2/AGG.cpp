#include <AGG.h>
#include <iostream>
#include <util.h>

AGG::AGG(cross_operators _crossOp, Problem * problem) : 
    population{Population(POP_SIZE), Population(POP_SIZE)},
    AG(_crossOp,problem),
    sols_mutated() {

    nMutate = ceil(MUTATE_PROB*POP_SIZE);
    nCross = ceil(CROSS_PROB*POP_SIZE/2) * 2;
}

ResultMH AGG::optimize(Problem *problem, int maxevals){
    this->generatePopulation(problem,this->population[0]);
    workingPop = 1;

    float bestFit = 0;
    tSolution bestSol;

    evaluations = POP_SIZE;
    while(evaluations < maxevals){
        this->select();
        this->cross(problem);
        this->mutate(problem);
        this->evaluate(problem);

        //elitismo
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

        workingPop = (workingPop + 1) % 2;
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
        
    }
}

void AGG::mutate(Problem * problem){

    sols_mutated.clear();
    std::uniform_int_distribution<int> dist_pop = std::uniform_int_distribution<int>(
        0,
        POP_SIZE-1
    );

    for(int i = 0; i < nMutate; ++i){

        int index = Random::get(dist_pop);
        //Se cambia la solucion
        mutateOp.mutate(this->getWorkingPopulation().getSolution(index));

        //Siempre que se cambie la solucion se actualiza el fitness
        if(index >= nCross){
            sols_mutated.push_back(index);
        }
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


void AGG::evaluate(Problem * problem){
    for(int i = 0; i < nCross; ++i){
        updateFitness(i,problem);
    }

    for(int i = 0; i < sols_mutated.size(); ++i){
        
        updateFitness(sols_mutated.at(i),problem);
    }

    sols_mutated.clear();
}