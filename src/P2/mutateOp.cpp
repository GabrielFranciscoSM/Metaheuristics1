#include <mutateOp.h>

MutateOp::MutateOp(Problem * prob){
    dist_range = std::uniform_int_distribution<int>(
        prob->getSolutionDomainRange().first,
        prob->getSolutionDomainRange().second-1
    );

    dist_size = std::uniform_int_distribution<int>(
        0,
        prob->getSolutionSize()-1
    );

}

void MutateOp::mutate(tSolution & sol){

    sol.at(Random::get(dist_size)) = Random::get(dist_range);

}
