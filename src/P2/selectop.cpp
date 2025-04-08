#include <selectop.h>


SelectOp::SelectOp(int popSize){
    this->dist_popSize = std::uniform_int_distribution(0,popSize-1);
}
/// @brief 
/// @pre Population pop should be Totally Populated
/// @param pop 
/// @return 
pair<tSolution,tFitness> SelectOp::select(Population & pop){
    int solIndex1 = Random::get(dist_popSize);
    int solIndex2 = Random::get(dist_popSize);

    if(pop.getFitness(solIndex1) > pop.getFitness(solIndex2)){
        return {tSolution(pop.getSolution(solIndex1)),tFitness(pop.getFitness(solIndex1))};
    }else{
        return {tSolution(pop.getSolution(solIndex2)),tFitness(pop.getFitness(solIndex2))};
    }
}