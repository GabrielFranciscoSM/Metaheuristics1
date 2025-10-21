#include <selectop.h>
#include <population.h>


SelectOp::SelectOp(int popSize){
    this->dist_popSize = std::uniform_int_distribution(0,popSize-1);
}
/// @brief 
/// @pre Population pop should be Totally Populated
/// @param pop 
/// @return 
pair<tSolution,tFitness> SelectOp::select(Population & pop,int tournamentsSize){

    vector<int> index(tournamentsSize);

    for(int i = 0; i < tournamentsSize; ++i){
        index.at(i) = Random::get(dist_popSize);
    }

    sort(index.begin(),index.end(),[&pop](int &a, int &b){ return pop.getFitness(a) > pop.getFitness(b); });
    return {tSolution(pop.getSolution(index.at(0))),tFitness(pop.getFitness(index.at(0)))};
    
    
    /*
        int solIndex1 = Random::get(dist_popSize);
        int solIndex2 = Random::get(dist_popSize);

        if(pop.getFitness(solIndex1) > pop.getFitness(solIndex2)){
            return {tSolution(pop.getSolution(solIndex1)),tFitness(pop.getFitness(solIndex1))};
        }else{
            return {tSolution(pop.getSolution(solIndex2)),tFitness(pop.getFitness(solIndex2))};
        }   
    */
}