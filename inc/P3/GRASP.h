#pragma once
#include <heuristicmh.h>

using namespace std;

/**
 * Implementation of the Greedy Search metaheuristic
 *  - Choose the nodes with most influence
 *
 * @see MH
 * @see Problem
 */
class GRASP : public MH {

public:
  enum graspType {NOBL, SIBL, SIBLALL};

  GRASP(graspType gt) : MH(), gt(gt) {}
  virtual ~GRASP() {}
  // Implement the MH interface methods
  /**
   * Find the nodes with most influence (respectively) and choose them
   * as the solution nodes
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimizeOne(Problem *problem, int maxevals);
  ResultMH optimizeNOBL(Problem *problem, int maxevals);
  ResultMH optimizeSIBL(Problem *problem, int maxevals);
  ResultMH optimize(Problem *problem, int maxevals) override;
  ResultMH optimizeSIBLALL(Problem *problem, int maxevals);

private:
    
  graspType gt;

};
