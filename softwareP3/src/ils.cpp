#include <cassert>
#include <ils.h>
#include <randomLS.h>
#include <simulatedAnnealing.h>
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

tSolution IteratedLocalSearch::mutate(const tSolution &original_sol, float mut_rate, int n) {
    tSolution mutated = original_sol;  // crear copia para no modificar el original
    int m = mutated.size();        // tamaño del subconjunto
    
    int num_changes = max(2, int(mut_rate * m));

    for (int i = 0; i < num_changes; i++) {
        // elegir una posición aleatoria de la solución actual
        int pos = Random::get(0, m - 1);

        // generar un nuevo nodo que no esté en la solución
        int new_node;
        bool exists;
        do {
            new_node = Random::get(0, n - 1);
            exists = false;
            for (int j = 0; j < m && !exists; j++) {
                if (mutated[j] == new_node) {
                    exists = true;
                }
            }
        } while (exists);

        mutated[pos] = new_node;
    }

    return mutated;
}

ResultMH IteratedLocalSearch::optimize(Problem *problem, const tSolution &current,
                                       tFitness fit, int maxevals) {
    assert(maxevals > 0);
    int n = problem->getSolutionDomainRange().second + 1; // para conseguir la n sin hacer cast     
    RandomLocalSearch bl = RandomLocalSearch();
    SimulatedAnnealing sa = SimulatedAnnealing();
    int evals = 0;
    bool useSA = getUseSA();

    tSolution sol = current;
    tFitness fitness = fit;

    ResultMH result = useSA ? sa.optimize(problem, sol, fitness, maxevals) : bl.optimize(problem, sol, fitness, maxevals);
    sol = result.solution;
    fitness = result.fitness;
    evals += result.evaluations;

    tSolution best = sol;
    tFitness best_fitness = fitness;
    
    for (int i = 0; i < 9; i++){
        tSolution mutated = mutate(best, 0.2f, n);  // cambia 20% de los elementos
        tFitness mutated_fitness = problem->fitness(mutated);
        evals++;

        ResultMH r = useSA ? sa.optimize(problem, mutated, mutated_fitness, maxevals) : bl.optimize(problem, mutated, mutated_fitness, maxevals);
        evals += r.evaluations;
        if (r.fitness < best_fitness){
            best = r.solution;
            best_fitness = r.fitness;
        }
    }

    return {best, best_fitness, evals};  
}