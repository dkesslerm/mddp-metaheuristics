#pragma once
#include <mh.h>

using namespace std;

/**
 * Implementation of the Heuristic Local Search metaheuristic
 *
 * @see MH
 * @see Problem
 */
class HeuristicLocalSearch : public MH {

public:
  HeuristicLocalSearch() : MH() {}
  virtual ~HeuristicLocalSearch() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;
};
