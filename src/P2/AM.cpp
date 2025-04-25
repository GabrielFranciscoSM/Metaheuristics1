#include <AM.h>
#include <iostream>
#include <util.h>

int iteraciones = 0;

AM::AM(int LSfase, float LSpercentage, bool orderPob, int LSmaxEvals,cross_operators _crossOp, Problem * problem): 
AG(_crossOp,problem),
AGG(_crossOp,problem),
LSfase(LSfase),
orderPob(orderPob),
LSmaxEvals(LSmaxEvals){
    assert(orderPob >= 0 && orderPob <= 1);

    LSsize = ceil(POP_SIZE*orderPob);
}

ResultMH AM::optimize(Problem *problem, int maxevals){

    this->generatePopulation(problem,this->population[0]);
    workingPop = 1;

    tFitness bestSolI = 0;
    float bestFit = 0;
    tSolution bestSol;
    int generations = 0;    

    evaluations = POP_SIZE;

    while(evaluations < maxevals){
        
        this->select();
        this->cross(problem);
        this->mutate(problem);
        this->evaluate(problem);

        //Busqueda local
        generations++;
        if(generations%LSfase == 0){
            evaluations = applyLocalSearch(problem, evaluations, maxevals);
        }


        //Elitismo
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

        //Cambiamos de poblacion
        workingPop = (workingPop+1)%2;
    }

    return ResultMH(bestSol,bestFit,evaluations);
}

int AM::applyLocalSearch(Problem *problem, int evaluations, int maxEvals){
    
    int realMaxEvals = min(LSmaxEvals,maxEvals-evaluations);

    int i = 0;

    if(this->orderPob){
        vector<int> orderedIndex = this->getWorkingPopulation().getSortedIndex();

        while(i < LSsize && realMaxEvals > 0){
            ResultMH res = LS.optimize(problem,this->getWorkingPopulation().getSolution(orderedIndex.at(i)),this->getWorkingPopulation().getFitness(orderedIndex.at(i)),realMaxEvals);
            this->getWorkingPopulation().insert_sol(orderedIndex.at(i),res.solution,res.fitness);
            evaluations += res.evaluations;
    
            realMaxEvals = min(LSmaxEvals,maxEvals-evaluations);
    
            ++i;
        }
    }
    else{
        while(i < LSsize && realMaxEvals > 0){
            ResultMH res = LS.optimize(problem,this->getWorkingPopulation().getSolution(i),this->getWorkingPopulation().getFitness(i),realMaxEvals);
            this->getWorkingPopulation().insert_sol(i,res.solution,res.fitness);
            evaluations += res.evaluations;
    
            realMaxEvals = min(LSmaxEvals,maxEvals-evaluations);
    
            ++i;
        }
    }

    return evaluations;
}
