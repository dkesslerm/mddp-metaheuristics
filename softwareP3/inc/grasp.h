#pragma once
#include <mh.h>

using namespace std;

/**
 * Implementation of the Random Local Search metaheuristic
 *
 * @see MH
 * @see Problem
 */
class GreedyRandomizeAdaptativeSearchProcedure : public MH {
private:
    bool useLS; // true if using local search, false if not
public: 
  GreedyRandomizeAdaptativeSearchProcedure(bool useLS) : MH() {this->useLS = useLS;}
  virtual ~GreedyRandomizeAdaptativeSearchProcedure() {}
  bool getUseLS() { return useLS; }
  float getDisp(tSolution selected, tDomain candidate, const vector<vector<float>> &matrix);
  tSolution getGreedySolution(int m, int n, const vector<vector<float>> &matrix);
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
