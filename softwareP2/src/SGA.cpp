#include <cassert>
#include <iostream>
#include <mddp.h>
#include <SGA.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

vector<tSolution> SteadyStateGeneticAlgorithm::generateChildren(const vector<tSolution> &poblacion, const vector<tFitness> &fitness, int n, int m){
    tSolution father1 = tournament(poblacion, fitness, 3);
    tSolution father2 = tournament(poblacion, fitness, 3);

    pair<tSolution, tSolution> children;
    if (getUniform()){
        children = uniformCrossover(father1, father2, n, m);
    } else children = positionCrossover(father1, father2, n, m);
    
    tSolution child1 = children.first;
    tSolution child2 = children.second;

    // aleatoriamente, se decide si mutar ambos hijos, uno solo, o ninguno

    if (Random::get(0.0, 1.0) < 0.1) {
        vector<tSolution> children_vector =  {child1};
        mutatePopulation(children_vector, 1, n);
        child1 = children_vector[0];
    }
    if (Random::get(0.0, 1.0) < 0.1) {
        vector<tSolution> children_vector =  {child2};
        mutatePopulation(children_vector, 1, n);
        child2 = children_vector[0];
    }

    return {child1, child2};
}


ResultMH SteadyStateGeneticAlgorithm::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);

    MDDProblem *mdd_problem = dynamic_cast<MDDProblem*>(problem);
    int n = mdd_problem->getProblemSize();         // problem size
    int m = problem->getSolutionSize();            // solution size
    int evals = 0;

    // generation of 50 random solutions
    int population_size = 50;
    vector<tSolution> poblacion(population_size);
    for (int i = 0; i < population_size; i++) {
        tSolution selected = problem->createSolution();
        poblacion[i] = selected;
    }
    
    // evaluate the initial population
    vector<tFitness> fitness(population_size);
    int best_index = 0;
    tFitness best_fitness = 1000000;
    for (int i = 0; i < poblacion.size(); i++) {
        fitness[i] = problem->fitness(poblacion[i]);
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            best_index = i;
        }
        evals++;
    }

    while (evals < maxevals) {        
        vector<tSolution> children = generateChildren(poblacion, fitness, n, m);
        tSolution child1 = children[0];
        tSolution child2 = children[1];

        tFitness fit1 = problem->fitness(child1);
        tFitness fit2 = problem->fitness(child2);
        evals += 2;

        tSolution best_child = child1;
        tFitness fit_best_child = fit1;
        if (fit2 < fit1) {
            best_child = child2;
            fit_best_child = fit2;
        }

        // substitution
        int worst_index = findWorst(fitness);
        if (fit_best_child < fitness[worst_index]) {
            poblacion[worst_index] = best_child;
            fitness[worst_index] = fit_best_child;
        }

        // update global best
        best_index = findBest(fitness);
        best_fitness = fitness[best_index];
    }

    tSolution final = poblacion[best_index];

    return ResultMH(final, best_fitness, evals);
}