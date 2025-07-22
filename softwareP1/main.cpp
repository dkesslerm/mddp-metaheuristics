#include <iostream>
#include <iomanip>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <chrono>

// Real problem class
#include "mddp.h"

// All all algorithms
#include "randomsearch.h"
#include "greedy.h"
#include "randomLS.h"
#include "heuristicLS.h"

using namespace std;

void print_result(ResultMH result, long int seed) {
  cout << "----------------------------------------" << endl;
  cout << "Using seed: " << seed << endl;
  cout << "Best solution: " << result.solution << endl;
  cout << "Best fitness: " << result.fitness << endl;
  cout << "Evaluations: " << result.evaluations << endl; 
}


int main(int argc, char *argv[]) {
  string file;
  long int seed;
  vector<long int> seeds;
  
  if (argc < 2 || argc > 3) {
    cerr << "Uso: " << argv[0] << " data/<archivo> <seed>(opcional)" << endl;
    return 1;
  } else if (argc == 2) {
    file = argv[1];
    seeds = {42, 0, 31415, 123, 2025};
    cout << "Usando las semillas por defecto" << endl;
  } else {
    file = argv[1];
    seeds = {atol(argv[2])};
    cout << "Usando la semilla: " << atol(argv[2]) << endl;
  }

  // Create the algorithms
  RandomSearch ralg = RandomSearch();
  GreedySearch rgreedy = GreedySearch();
  RandomLocalSearch randLS_1 = RandomLocalSearch();
  HeuristicLocalSearch heurLS = HeuristicLocalSearch();

  // Create the specific problem
  MDDProblem rproblem = MDDProblem(file);
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                           make_pair("GreedySearch", &rgreedy),
                                           make_pair("LocalSearchRandom", &randLS_1),
                                           make_pair("LocalSearchHeuristic", &heurLS)
                                          };

  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  string name;
  int maxevals;
  vector<float> sum_fitness(algoritmos.size(), 0);
  vector<int> sum_evals(algoritmos.size(), 0);

  for (int i = 0; i < algoritmos.size(); i++) {
    name = algoritmos[i].first;
    MH *mh = algoritmos[i].second;
    maxevals = (algoritmos[i].first != "GreedySearch") ? 100000 : 1;

    cout << endl << name << endl;
    for (int j = 0; j < seeds.size(); j++){
      seed = seeds[j];
      Random::seed(seed);

      ResultMH result = mh->optimize(problem, maxevals);

      print_result(result, seed);
      sum_fitness[i] += result.fitness;
      sum_evals[i] += result.evaluations;
    }
  }

  cout << "----------------------------------------" << endl;
  cout << "Average results for file: " << file << endl;
  for (int i = 0; i < algoritmos.size(); i++) {
    cout << endl << algoritmos[i].first << endl << "Fitness: " << sum_fitness[i] / seeds.size() << endl 
         << "Evaluations: " << sum_evals[i] / seeds.size() << endl;
  }



  return 0;
}
