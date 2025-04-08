#include <cross_ordered.h>
#include <queue>

void CrossOrdered::cross(tSolution & sol1, tSolution & sol2,Problem * prob){

    std::priority_queue<int> orderedNodes;

    for(int i = 0; i < sol1.size(); ++i){
        orderedNodes.push(sol1.at(i));
        orderedNodes.push(sol2.at(i));
    }

    int i = 0;
    while(!orderedNodes.empty()){
        sol1.at(i) = orderedNodes.top();
        orderedNodes.pop();
        sol2.at(i) = orderedNodes.top();
        orderedNodes.pop();
        ++i;
    }
}