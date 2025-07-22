#pragma once
#include <mh.h>
#include <GGA.h>
#include <randomLS.h>

using namespace std;

enum hibridation {ALL, SOME_RANDOM, SOME_BEST};

/**
 * Implementation of the Genetic Algorithm
 *
 * @see MH
 * @see Problem
 */
class MemeticAlgorithm: public GenerationalGeneticAlgorithm {
private:
    hibridation strategy;
    const int period = 10;
    float pLS;
public:
    MemeticAlgorithm(hibridation strat) : GenerationalGeneticAlgorithm(false){
        strategy = strat;
        pLS = (strat == hibridation::SOME_RANDOM ? 0.1f : 1.0f);
    }
    virtual ~MemeticAlgorithm() {}
    
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
