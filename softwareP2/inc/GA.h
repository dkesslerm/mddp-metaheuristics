#pragma once
#include <mh.h>

using namespace std;

/**
 * Implementation of the Genetic Algorithm
 *
 * @see MH
 * @see Problem
 */
class GeneticAlgorithm: public MH {
private:
    bool uniform; // true for uniform crossover, false for position crossover
public:
    GeneticAlgorithm(bool uniform) : MH() {
        this->uniform = uniform;
    }
    virtual ~GeneticAlgorithm() {}
    bool getUniform() const { return uniform; }

    tSolution tournament(const vector<tSolution> &poblacion, const vector<tFitness> &fitness, int k);
    pair<tSolution, tSolution> uniformCrossover(const tSolution father1, const tSolution father2, int n, int m);
    pair<tSolution, tSolution> positionCrossover(const tSolution father1, const tSolution father2, int n, int m);
    void mutatePopulation(vector<tSolution> &poblacion, int num_mutations, int n);
    int findWorst(const vector<tFitness> &fitness);
    int findBest(const vector<tFitness> &fitness);

};
