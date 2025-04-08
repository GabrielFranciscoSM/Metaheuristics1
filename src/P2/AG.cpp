#include <AG.h>
#include <iostream>
#include <util.h>

AG::AG(cross_operators _crossOp, Problem * problem) : 
    selectOp(SelectOp(POP_SIZE)), 
    mutateOp(MutateOp(problem))
     {
    

    switch (_crossOp)
    {
    case cross_operators::ordered:
        crosOp = new CrossOrdered();
        break;
    
    case cross_operators::unordered:
        crosOp = new CrossUnordered(problem);
        break;
    }

    evaluations = 0;
}


void AG::generatePopulation(Problem * problem, Population & prob){

    for(int i = 0; i < POP_SIZE; ++i){
        tSolution solution = problem->createSolution();
        prob.insert_sol(i,solution, problem->fitness(solution));
        evaluations++;
    }
}