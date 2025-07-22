#include <cassert>
#include <heuristicLS.h>
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

void ordenaVecindario(vector<pair<int, int>> &vecindario, const vector<float> distances){
    // ordenamos el vecindario según la distancia del nodo seleccionado con el resto de los nodos, de mayor a menor
    sort(vecindario.begin(), vecindario.end(),
        [&](const pair<int, int>& a, const pair<int, int>& b) {
            return distances[a.first] > distances[b.first];
        });
    
    // creamos un vecindario auxiliar añadiendo los elementos de forma alterna 
    vector<pair<int, int>> vecindario_aux;
    for (int i = 0; i < vecindario.size() / 2; i++) {
        vecindario_aux.push_back(vecindario[i]);
        vecindario_aux.push_back(vecindario[vecindario.size()-1-i]);
    }
    // si el tamaño del vecindario es impar, añadimos el elemento del medio
    if (vecindario.size() % 2 != 0) {
        vecindario_aux.push_back(vecindario[vecindario.size()/2]);
    }

    vecindario = vecindario_aux;
}


ResultMH HeuristicLocalSearch::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);

    MDDProblem *mdd_problem = dynamic_cast<MDDProblem*>(problem);
    int n = mdd_problem->getProblemSize();         // tamaño del problema
    int m = problem->getSolutionSize();            // tamaño de la solución
    vector<vector<float>> matriz =  mdd_problem->getMatrix();  // matriz de distancias
    int evals = 0;

    // generamos una primera solución aleatoria y la evaluamos
    tSolution seleccionados = mdd_problem->createSolution();
    tFitness fitness = mdd_problem->fitness(seleccionados);

    // llenamos disponibles
    vector<tDomain> disponibles;
    for (int i = 0; i < n; i++){
        if (find(seleccionados.begin(), seleccionados.end(), i) == seleccionados.end())
            disponibles.push_back(i);
    }

    // factoringinfo
    vector<float> distances;

    for (int i : seleccionados) {
        float suma = 0;
        for (int j : seleccionados) {
            if (i != j) suma += matriz[i][j];
        }
        distances.push_back(suma);
    }

    // generamos vecindario, par (indice del valor a cambiar, nuevo valor)
    vector<pair<int, int>> vecindario;
    for (int i = 0; i < m; i++) {
        for (int j : disponibles) {
            vecindario.emplace_back(i, j);
        }
    }
    ordenaVecindario(vecindario, distances);

    for (int i = 0; i < vecindario.size() && evals < maxevals; i++) {
        // notacion
        int ind_seleccionado = vecindario[i].first;
        int valor_disponible = vecindario[i].second;

        vector<float> new_distances(m);

        for (int i = 0; i < m; i++) {
            if (i != ind_seleccionado){
                int v = seleccionados[i];
                float contrib = distances[i]
                                - matriz[v][seleccionados[ind_seleccionado]]
                                + matriz[v][valor_disponible];
                new_distances[i] = contrib;
            }               
        }
        // recalcular contribución del nuevo elemento en la posición modificada
        float new_contrib = 0;
        for (int i = 0; i < m; i++) {
            if (i != ind_seleccionado)
                new_contrib += matriz[seleccionados[i]][valor_disponible];
        }
        new_distances[ind_seleccionado] = new_contrib;

        // calculamos nuevo fitness
        auto [new_min, new_max] = minmax_element(new_distances.begin(), new_distances.end());
        float new_fitness = *new_max - *new_min;
        evals++;
        
        // si mejora
        if (new_fitness < fitness) {
            
            // hacer el cambio
            int valor_eliminado = seleccionados[ind_seleccionado];
            seleccionados[ind_seleccionado] = valor_disponible;
            for (int i = 0; i < disponibles.size(); i++) {
                if (disponibles[i] == valor_disponible) {
                    disponibles[i] = valor_eliminado;
                }
            }
            
            // actualizamos distancias y fitness
            distances = new_distances;
            fitness = new_fitness;

            // como es búsqueda del primer mejor, saltamos a la siguiente iteración calculando el vecindario para la nueva solución
            vecindario.clear();
            for (int i = 0; i < m;i++) {
                for (int j : disponibles) {
                    vecindario.emplace_back(i, j);
                }
            }
            ordenaVecindario(vecindario, distances); // vecindario ordenado según la heurística

            i = -1; // reiniciar bucle principal                
        }
    }
    return {seleccionados, fitness, evals};    
}