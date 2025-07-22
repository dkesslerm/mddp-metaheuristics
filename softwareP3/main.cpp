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
#include "greedy.h"
#include "randomLS.h"
#include "GA.h"
#include "GGA.h"
#include "MA.h"
#include "bmb.h"
#include "ils.h"
#include "simulatedAnnealing.h"
#include "grasp.h"

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
    cerr << "Use: " << argv[0] << " data/<file> <seed>(optional)" << endl;
    return 1;
  } else if (argc == 2) {
    file = argv[1];
    seeds = {42, 0, 31415, 123, 2025};
    cout << "Using default seeds" << endl;
  } else {
    file = argv[1];
    seeds = {atol(argv[2])};
    cout << "Using seed: " << atol(argv[2]) << endl;
  }

  // Create the algorithms
  GreedySearch greedy = GreedySearch();
  RandomLocalSearch randLS = RandomLocalSearch();

  // GenerationalGeneticAlgorithm gga_uniform = GenerationalGeneticAlgorithm(true);
  // GenerationalGeneticAlgorithm gga_position = GenerationalGeneticAlgorithm(false);
  // SteadyStateGeneticAlgorithm sga_uniform = SteadyStateGeneticAlgoritithm(false);
  // MemeticAlgorithm memeticALL = MemeticAlgorithm(ALL);
  // MemeticAlgorithm memeticSOME = MemeticAlgorithm(SOME_RANDOM);
  MemeticAlgorithm memeticBEST = MemeticAlgorithm(SOME_BEST);

  BasicMultiStart bmb = BasicMultiStart();
  IteratedLocalSearch ils = IteratedLocalSearch(false);
  IteratedLocalSearch ils_sa = IteratedLocalSearch(true);
  SimulatedAnnealing es = SimulatedAnnealing();
  GreedyRandomizeAdaptativeSearchProcedure grasp = GreedyRandomizeAdaptativeSearchProcedure(false);
  GreedyRandomizeAdaptativeSearchProcedure graspLS = GreedyRandomizeAdaptativeSearchProcedure(true);

  // Create the specific problem
  MDDProblem rproblem = MDDProblem(file);

  // Solve using evaluations
  struct Algorithm {
    string name;
    MH *algorithm;
    int max_evaluations;
  };

  vector<Algorithm> algoritmos = {
    {"GreedySearch", &greedy, 1},
    {"LocalSearchRandom", &randLS, 100000},
    {"MemeticAlgorithm_(BEST)", &memeticBEST, 100000},
    {"BasicMultiStart", &bmb, 10000},
    {"IteratedLocalSearch_(BL)", &ils, 10000},
    {"IteratedLocalSearch_(SA)", &ils_sa, 10000},
    {"SimulatedAnnealing", &es, 100000},
    {"GRASP-NOBL", &grasp, 10000},
    {"GRASP-SIBL", &graspLS, 10000}
  };

  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  string name;
  int maxevals;
  vector<float> sum_fitness(algoritmos.size(), 0);
  vector<int> sum_evals(algoritmos.size(), 0);

  for (int i = 0; i < algoritmos.size(); i++) {
    name = algoritmos[i].name;
    MH *mh = algoritmos[i].algorithm;
    maxevals = algoritmos[i].max_evaluations;

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
    cout << endl << algoritmos[i].name << endl << "Fitness: " << sum_fitness[i] / seeds.size() << endl 
         << "Evaluations: " << sum_evals[i] / seeds.size() << endl;
  }



  return 0;
}
