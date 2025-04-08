#include <cross_unordered.h>
#include <vector>
#include <pinfluencer.h>
using namespace std;

CrossUnordered::CrossUnordered(Problem * prob): Crossop(){
    this->dist_SizeSol = uniform_int_distribution<int>(1,prob->getSolutionSize() - 2);
}

void CrossUnordered::cross(tSolution & sol1, tSolution & sol2,Problem * prob) {

    ProblemInflu * infuProb = dynamic_cast<ProblemInflu *>(prob);
  
    //Get the random segment [pivot1, pivot2]

    int pivot1 = Random::get(dist_SizeSol);
    int pivot2 = Random::get(dist_SizeSol);
    
    while(abs(pivot1 - pivot2) < MIN_LENGTH)
        pivot2 = Random::get(dist_SizeSol);

    if(pivot1 > pivot2){
        pivot1 = pivot1 + pivot2;
        pivot2 = pivot1 - pivot2;
        pivot1 = pivot1 - pivot2;
    }

    //Insertion with no repetition
    vector<int> unchanged1;
    vector<int> unchanged2;

    for(int i = 0; i < sol1.size(); ++i){
        if(i < pivot1 && i > pivot2){
            unchanged1.push_back(sol1.at(i));
            unchanged2.push_back(sol2.at(i));
        }
    }

    int aux1;
    int aux2;
    int j = 0;

    //INterchange in the segement
    for(int i = pivot1; i <= pivot2; ++i){

        //Check insertion of sol2(i) into sol1(i)
        aux1 = sol2.at(i);
        j = 0;
        while(find(unchanged1.begin(),unchanged1.end(),aux1) != unchanged1.end())
            aux1 = unchanged2.at(j++);
        
        unchanged1.push_back(aux1);

        //Check insertion of sol1(i) into sol2(i)
        aux2 = sol1.at(i);
        j = 0;
        while(find(unchanged2.begin(),unchanged2.end(),aux2) != unchanged2.end())
            aux2 = unchanged1.at(j++);
        
        unchanged2.push_back(aux2);

        sol1.at(i) = aux1;
        sol2.at(i) = aux2;
    }

}