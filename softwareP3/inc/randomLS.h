#pragma once
#include <mhtrayectory.h>

using namespace std;

/**
 * Implementation of the Random Local Search metaheuristic
 *
 * @see MH
 * @see Problem
 */
class RandomLocalSearch : public MHTrayectory {

public:
  RandomLocalSearch() : MHTrayectory() {}
  virtual ~RandomLocalSearch() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const tSolution &current,
                    tFitness fitness, int maxevals) override;
};
