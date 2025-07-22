#include <cassert>
#include <bmb.h>
#include <randomLS.h>
#include <iostream>
#include <mddp.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

ResultMH BasicMultiStart::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);
    
    int size = 10;
    int evals = 0;
    vector<tSolution> solutions(size);
    vector<tFitness> fitness(size);
    for (int i = 0; i < size; i++){
        tSolution sol = problem->createSolution();
        solutions[i] = sol;
        fitness[i] = problem->fitness(sol);
        evals++;
    }
    
    RandomLocalSearch bl = RandomLocalSearch();

    tSolution best = solutions[0];
    tFitness best_fitness = fitness[0];
    for (int i = 0; i < size; i++){
        ResultMH result = bl.optimize(problem, solutions[i], fitness[i], maxevals);
        if (result.fitness < best_fitness){
            best = result.solution;
            best_fitness = result.fitness;
        }
        evals += result.evaluations;
    }

    return {best, best_fitness, evals};
}