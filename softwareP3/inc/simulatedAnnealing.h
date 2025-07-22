#pragma once
#include <mhtrayectory.h>

using namespace std;

/**
 * Implementation of the Random Local Search metaheuristic
 *
 * @see MH
 * @see Problem
 */
class SimulatedAnnealing : public MHTrayectory {

public:
    SimulatedAnnealing() : MHTrayectory() {}
    virtual ~SimulatedAnnealing() {}

    vector<pair<int, int>> getNeighbourhood(const tSolution& current, int n);
    // tSolution getNeighbour(const tSolution& current, int n);

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
