#include <cassert>
#include <iostream>
#include <mddp.h>
#include <MA.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

ResultMH MemeticAlgorithm::optimize(Problem *problem, int maxevals) {
    MDDProblem *mdd_problem = dynamic_cast<MDDProblem*>(problem);
    int n = mdd_problem->getProblemSize();
    int m = problem->getSolutionSize();
    int evals = 0;
    int gen = 0;
    RandomLocalSearch bl = RandomLocalSearch();

    int population_size = 50;
    vector<tSolution> poblacion(population_size);
    for (int i = 0; i < population_size; ++i)
        poblacion[i] = problem->createSolution();

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
        gen++;
        vector<tSolution> children = generateChildren(poblacion, fitness, n, m);

        vector<tFitness> fitness_children(population_size);
        for (int i = 0; i < population_size; ++i) {
            fitness_children[i] = problem->fitness(children[i]);
            evals++;
        }

        // cada 10 generaciones, búsqueda local
        if (gen % period == 0) {
            vector<int> to_improve; 
            
            if (strategy == hibridation::ALL) {
                to_improve.resize(population_size);
                for (int i = 0; i < population_size; i++)
                    to_improve[i] = i;
            } else {
                vector<pair<tFitness, int>> fitness_index(population_size);
                for (int i = 0; i < population_size; ++i)
                    fitness_index[i] = {fitness_children[i], i};

                if (strategy == hibridation::SOME_RANDOM)
                    Random::shuffle(fitness_index.begin(), fitness_index.end());
                else
                    sort(fitness_index.begin(), fitness_index.end());

                int num_to_improve = pLS * population_size;
                for (int i = 0; i < num_to_improve; ++i)
                    to_improve.push_back(fitness_index[i].second);
            }

            // aplicación de búsqueda local
            for (int i = 0; i < to_improve.size(); i++) {
                int real_evals = min(maxevals - evals, 400);
                if (real_evals > 0) {
                    ResultMH r = bl.optimize(problem, children[to_improve[i]], fitness_children[to_improve[i]], real_evals);
                    children[to_improve[i]] = r.solution;
                    fitness_children[to_improve[i]] = r.fitness;
                    evals += r.evaluations;
                }
            }
        }

        // elitismo
        int worst_index = findWorst(fitness);
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