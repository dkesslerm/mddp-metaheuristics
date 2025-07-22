#include <cassert>
#include <iostream>
#include <mddp.h>
#include <GA.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

tSolution GeneticAlgorithm::tournament(const vector<tSolution> &poblacion, const vector<tFitness> &fitness, int k) {
    int n = poblacion.size();

    vector<int> index(n);
    for (int i = 0; i < n; i++) {
        index[i] = i;
    }

    Random::shuffle(index.begin(), index.end()); // mezcla los indices

    tSolution best = poblacion[index[0]];
    tFitness best_fitness = fitness[index[0]];

    for (int i = 1; i < k; i++) {
        if (fitness[index[i]] < best_fitness) {
            best = poblacion[index[i]];
            best_fitness = fitness[index[i]];
        }
    }

   return best;
}


void GeneticAlgorithm::mutatePopulation(vector<tSolution> &poblacion, int num_mutations, int n) {
    for (int i = 0; i < num_mutations; i++) {
        int pob_size = poblacion.size();
        
        // elegir una solución aleatoria
        int sol_index = Random::get(0, pob_size-1);
        tSolution &sol = poblacion[sol_index];

        // elegir nodo a quitar
        int sol_size = sol.size();
        int pos = Random::get(0, sol_size - 1);

        // generar uno nuevo que no esté ya en la solución
        int new_node;
        bool exists;
        do {
            new_node = Random::get(0, n - 1);
            exists = false;
            for (int i = 0; i < sol_size && !exists; i++) {
                if (sol[i] == new_node) {
                    exists = true;
                }
            }
        } while (exists);

        sol[pos] = new_node;
    }
}

pair<tSolution, tSolution> GeneticAlgorithm::uniformCrossover(const tSolution father1, const tSolution father2, int n, int m) {
    vector<bool> binary_father1(n, false);
    vector<bool> binary_father2(n, false);

    for (int node : father1){binary_father1[node] = true;}
    for (int node : father2){binary_father2[node] = true;}

    vector<bool> binary_child1(n, false);
    vector<bool> binary_child2(n, false);
    vector<int> modifiables;

    for (int i = 0; i < n; i++){
        if (binary_father1[i] == binary_father2[i]) {
            binary_child1[i] = binary_father1[i];
            binary_child2[i] = binary_father2[i];
        } else {
            binary_child1[i] = Random::get(0, 1);
            binary_child2[i] = Random::get(0, 1);
            modifiables.push_back(i);
        }
    }

    int node_count1 = 0;
    int node_count2 = 0;
    for (int i = 0; i < n; i++){
        if (binary_child1[i]) node_count1++;
        if (binary_child2[i]) node_count2++;
    }

    // reparación, heurística es hacer un shuffle entre las posiciones modificables 

    Random::shuffle(modifiables.begin(), modifiables.end());

    bool finished;
    if (node_count1 > m){
        finished = false;
        for (int i = 0; i < modifiables.size() && !finished; i++) {
            if (binary_child1[modifiables[i]]) {
                binary_child1[modifiables[i]] = false;
                node_count1--;
                finished = (node_count1 - m == 0);
            }
        }
    } else if (node_count1 < m){
        finished = false;
        for (int i = 0; i < modifiables.size() && !finished; i++) {
            if (!binary_child1[modifiables[i]]) {
                binary_child1[modifiables[i]] = true;
                node_count1++;
                finished = (node_count1 - m == 0);
            }
        }
    }

    if (node_count2 > m){
        finished = false;
        for (int i = 0; i < modifiables.size() && !finished; i++) {
            if (binary_child2[modifiables[i]]) {
                binary_child2[modifiables[i]] = false;
                node_count2--;
                finished = (node_count2 - m == 0);
            }
        }
    } else if (node_count2 < m){
        finished = false;
        for (int i = 0; i < modifiables.size() && !finished; i++) {
            if (!binary_child2[modifiables[i]]) {
                binary_child2[modifiables[i]] = true;
                node_count2++;
                finished = (node_count2 - m == 0);
            }
        }
    }
    
    // ya tenemos nuestros binary_childs corregidos, pasamos de vuelta a los valores originales

    tSolution child1, child2;

    for (int i = 0; i < n; i++){
        if (binary_child1[i])
            child1.push_back(i);
        if (binary_child2[i])
            child2.push_back(i);        
    }
    
    return {child1, child2};
}

pair<tSolution, tSolution> GeneticAlgorithm::positionCrossover(const tSolution father1, const tSolution father2, int n, int m){
    vector<bool> binary_father1(n, false);
    vector<bool> binary_father2(n, false);

    for (int node : father1){binary_father1[node] = true;}
    for (int node : father2){binary_father2[node] = true;}

    vector<bool> binary_child1(n, false);
    vector<bool> binary_child2(n, false);
    vector<int> differing_positions;
    vector<bool> bits1, bits2;

    for (int i = 0; i < n; i++){
        if (binary_father1[i] == binary_father2[i]) {
            binary_child1[i] = binary_father1[i];
            binary_child2[i] = binary_father2[i];
        } else {
            // diferentes: guardar posición y valor de cada padre
            differing_positions.push_back(i);
            bits1.push_back(binary_father1[i]);
            bits2.push_back(binary_father2[i]);
        }
    }

    // shuffle
    Random::shuffle(bits1.begin(), bits1.end());
    Random::shuffle(bits2.begin(), bits2.end());

    for (int i = 0; i < differing_positions.size(); ++i) {
        int pos = differing_positions[i];
        binary_child1[pos] = bits1[i];
        binary_child2[pos] = bits2[i];
    }

    tSolution child1, child2;
    for (int i = 0; i < n; i++) {
        if (binary_child1[i]) child1.push_back(i);
        if (binary_child2[i]) child2.push_back(i);
    }

    return {child1, child2};
}

int GeneticAlgorithm::findWorst(const vector<tFitness> &fitness) {
    int worst_index = 0;
    tFitness worst_fitness = fitness[0];

    for (int i = 1; i < fitness.size(); i++) {
        if (fitness[i] > worst_fitness) {
            worst_fitness = fitness[i];
            worst_index = i;
        }
    }

    return worst_index;
}

int GeneticAlgorithm::findBest(const vector<tFitness> &fitness) {
    int best_index = 0;
    tFitness best_fitness = fitness[0];

    for (int i = 1; i < fitness.size(); i++) {
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            best_index = i;
        }
    }

    return best_index;
}