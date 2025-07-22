#include <cassert>
#include <simulatedAnnealing.h>
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

vector<pair<int, int>> SimulatedAnnealing::getNeighbourhood(const tSolution &current, int n){
    int m = current.size();
    vector<pair<int, int>> neighbourhood;

    // llenamos disponibles
    vector<tDomain> available;
    for (int i = 0; i < n; i++){
        if (find(current.begin(), current.end(), i) == current.end())
            available.push_back(i);
    }

    for (int i = 0; i < m; i++) {
        for (int j : available) {
            neighbourhood.emplace_back(i, j);
        }
    }

    Random::shuffle(neighbourhood.begin(), neighbourhood.end()); // hacemos shuffle para random

    return neighbourhood;
}

ResultMH SimulatedAnnealing::optimize(Problem *problem, const tSolution &current,
                                      tFitness fit, int maxevals) {
    assert(maxevals > 0);
    float mu = 0.2;
    float phi = 0.3;
    float Tf = 0.001;

    int m = problem->getSolutionSize();
    int n = problem->getSolutionDomainRange().second + 1; // para conseguir la n sin hacer cast     
    int max_neighbours = 100 * m;
    int max_improvements = 0.1 * max_neighbours;
    int max_iters = maxevals / max_neighbours;

    tSolution sol = current;
    tFitness fitness = fit;
    int evals = 1;

    tSolution best = sol;
    tFitness best_fitness = fitness;

    double T0 = (mu*fitness)/ (-log(phi));

    while (T0 < Tf) Tf /= 10;

    double T = T0;
    double beta = (T0 - Tf) / (max_iters * T0 * Tf);

    int improvement_num = -1;
    while (evals < maxevals && Tf < T && improvement_num != 0) {
        improvement_num = 0;
        int neighbour_num = 0;
        vector<pair<int, int>> neighbourhood = getNeighbourhood(sol, n);
        int neighbour_index = 0;
        
        while (improvement_num < max_improvements && neighbour_num < max_neighbours && evals < maxevals && neighbour_index < neighbourhood.size()) {
            tSolution neighbour = sol;
            neighbour[neighbourhood[neighbour_index].first] = neighbourhood[neighbour_index].second;
            tFitness neighbour_fitness = problem->fitness(neighbour);

            evals++;
            neighbour_num++;

            float delta = neighbour_fitness - fitness;
            if (delta < 0 || Random::get(0.0, 1.0) <= exp(-delta / T)) {
                sol = neighbour;
                fitness = neighbour_fitness;
                improvement_num++;

                neighbourhood = getNeighbourhood(sol, n);
                neighbour_index = -1; // reiniciamos el índice del vecindario

                if (fitness < best_fitness) {
                    best = sol;
                    best_fitness = fitness;
                }
            }
            neighbour_index++;
        }
        T = T / (1 + beta * T);
    }

    return {best, best_fitness, evals};
}