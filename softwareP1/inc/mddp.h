#pragma once
#include <algorithm>
#include <problem.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <random.hpp>

using namespace std;

class MDDProblem : public Problem { // Se hereda de problema 
private:
  size_t n; // tamaño del problema
  size_t m; // tamaño de la solución
  vector<vector<float>> matrix;
public:
  MDDProblem(size_t size);
  MDDProblem(const string &file_name);

  tFitness fitness(const tSolution &solution) override;
  tSolution createSolution() override;
  
  size_t getSolutionSize() override { return m; }
  size_t getProblemSize() { return n; }
  const vector<vector<float>> & getMatrix() const { return matrix; }

  std::pair<tDomain, tDomain> getSolutionDomainRange() override {
    return std::make_pair(false, true);
  }
};