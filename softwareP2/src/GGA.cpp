#include <cassert>
#include <iostream>
#include <mddp.h>
#include <GGA.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

vector<tSolution> GenerationalGeneticAlgorithm::generateChildren(const vector<tSolution> &poblacion, const vector<tFitness> &fitness, int n, int m) {
    int population_size = 50;
    vector<tSolution> fathers(population_size);
    for (int i = 0; i < population_size; i++){
        fathers[i] = tournament(poblacion, fitness, 3);
    }

    vector<tSolution> children(population_size);
    int num_crossover = (int)ceil(0.7 * 25); // Pc = 0.7, 25 parejas
    for (int i = 0; i < 2 * num_crossover; i += 2) {
        pair<tSolution, tSolution> crossed_children;
        if (getUniform()){
            crossed_children = uniformCrossover(fathers[i], fathers[i+1], n, m);
        }
        else
            crossed_children = positionCrossover(fathers[i], fathers[i+1], n, m);

        children[i] = crossed_children.first;
        children[i+1] = crossed_children.second;            
    }

    // el resto de hijos sin cruce: copiar directamente
    for (int i = 2 * num_crossover; i < population_size; i++) {
        children[i] = fathers[i];
    }

    // mutación
    int num_mutations = (int)ceil(0.1 * population_size);
    mutatePopulation(children, num_mutations, n);
    
    return children;
}

ResultMH GenerationalGeneticAlgorithm::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);

    MDDProblem *mdd_problem = dynamic_cast<MDDProblem*>(problem);
    int n = mdd_problem->getProblemSize();         // problem size
    int m = problem->getSolutionSize();            // solution size
    int evals = 0;

    // generation of 50 random solutions
    int population_size = 50;
    vector<tSolution> poblacion(population_size);
    for (int i = 0; i < population_size; i++) {
        tSolution sol = problem->createSolution();
        poblacion[i] = sol;
    }
    
    // evaluate the initial population
    vector<tFitness> fitness(population_size);
    int best_index = -1;
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
     
        // evaluar hijos
        vector<tFitness> fitness_children(population_size);
        for (int i = 0; i < population_size; i++) {
            fitness_children[i] = problem->fitness(children[i]);
            evals++;
        }

        // elitismo
        int worst_index = findWorst(fitness_children);
        if (best_fitness < fitness_children[worst_index]) {
            children[worst_index] = poblacion[best_index];
            fitness_children[worst_index] = best_fitness;
        }

        // reemplazo
        poblacion = children;
        fitness = fitness_children;        

        best_index = findBest(fitness);
        best_fitness = fitness[best_index];
    }

    tSolution final = poblacion[best_index];
    return ResultMH(final, best_fitness, evals);
}