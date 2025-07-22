#include <cassert>
#include <greedy.h>
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

ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  
  MDDProblem *mdd_problem = dynamic_cast<MDDProblem*>(problem);
  int n = mdd_problem->getProblemSize();         // tamaño del problema
  int m = problem->getSolutionSize();            // tamaño de la solución

  // al principio, todos los elementos están disponibles
  vector<tDomain> disponibles(n);
  for(int i = 0; i < n; i++){
    disponibles[i] = i;
  }

  // cogemos el primer elemento aleatorio, lo metemos a selección y lo quitamos de disponibles
  tDomain e0 = Random::get(0, n-1); 
  tSolution seleccion;
  seleccion.push_back(e0);
  disponibles.erase(remove(disponibles.begin(), disponibles.end(), e0), disponibles.end());

  // hacemos shuffle porque observamos que el primer elemento siempre era el mejor (ya que la dispersión siempre va a ser 0 con 2 elementos)
  // Random::shuffle(disponibles.begin(), disponibles.end());

  // while |S| < m do
  while (seleccion.size() < m) {
      tFitness mejor_dispersion = numeric_limits<float>::max();
      tDomain mejor_elemento;

      for (tDomain e : disponibles) {
          tSolution candidata = seleccion;
          candidata.push_back(e);

          tFitness dispersion = mdd_problem->fitness(candidata);

          if (dispersion < mejor_dispersion) {
              mejor_dispersion = dispersion;
              mejor_elemento = e;
          }
      }
      
      // añadimos el mejor elemento a la solución y lo borramos de disponibles
      seleccion.push_back(mejor_elemento);
      disponibles.erase(remove(disponibles.begin(), disponibles.end(), mejor_elemento), disponibles.end());
  }

  tFitness final_fitness = mdd_problem->fitness(seleccion);

  return ResultMH(seleccion, final_fitness, 1);
}

