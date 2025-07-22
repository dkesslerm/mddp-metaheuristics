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
#include "randomLS.h"
#include "heuristicLS.h"
#include "GA.h"
#include "GGA.h"
#include "SGA.h"
#include "MA.h"

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
  RandomLocalSearch randLS = RandomLocalSearch();
  HeuristicLocalSearch heurLS = HeuristicLocalSearch();
  GenerationalGeneticAlgorithm gga_uniform = GenerationalGeneticAlgorithm(true);
  GenerationalGeneticAlgorithm gga_position = GenerationalGeneticAlgorithm(false);
  SteadyStateGeneticAlgorithm sga_uniform = SteadyStateGeneticAlgorithm(true);
  SteadyStateGeneticAlgorithm sga_position = SteadyStateGeneticAlgorithm(false);
  MemeticAlgorithm memeticALL = MemeticAlgorithm(ALL);
  MemeticAlgorithm memeticSOME = MemeticAlgorithm(SOME_RANDOM);
  MemeticAlgorithm memeticBEST = MemeticAlgorithm(SOME_BEST);


  // Create the specific problem
  MDDProblem rproblem = MDDProblem(file);
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {
                                          make_pair("LocalSearchRandom", &randLS),
                                          make_pair("HeuristicLocalSearch", &heurLS),
                                          make_pair("GenerationalGeneticAlgorithm (Uniform crossover)", &gga_uniform),
                                          make_pair("GenerationalGeneticAlgorithm (Position crossover)", &gga_position),
                                          make_pair("SteadyStateGeneticAlgorithm (Uniform crossover)", &sga_uniform),
                                          make_pair("SteadyStateGeneticAlgorithm (Position crossover)", &sga_position),
                                          make_pair("MemeticAlgorithm (ALL)", &memeticALL),
                                          make_pair("MemeticAlgorithm (SOME)", &memeticSOME),
                                          make_pair("MemeticAlgorithm (BEST)", &memeticBEST)
                                          };

  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  string name;
  int maxevals;
  vector<float> sum_fitness(algoritmos.size(), 0);
  vector<int> sum_evals(algoritmos.size(), 0);

  for (int i = 0; i < algoritmos.size(); i++) {
    name = algoritmos[i].first;
    MH *mh = algoritmos[i].second;
    maxevals = 100000;

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
