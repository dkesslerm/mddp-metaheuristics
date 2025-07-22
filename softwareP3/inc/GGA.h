#pragma once
#include <GA.h>

using namespace std;

/**
 * Implementation of the Generational Genetic Algorithm
 *
 * @see MH
 * @see Problem
 */
class GenerationalGeneticAlgorithm: public GeneticAlgorithm {
public:
  GenerationalGeneticAlgorithm(bool uniform) : GeneticAlgorithm(uniform) {}
  virtual ~GenerationalGeneticAlgorithm() {}

  vector<tSolution> generateChildren(const vector<tSolution> &poblacion, const vector<tFitness> &fitness, int n, int m);

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
