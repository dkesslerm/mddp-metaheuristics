#include <cassert>
#include <grasp.h>
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

float GreedyRandomizeAdaptativeSearchProcedure::getDisp(tSolution selected, tDomain candidate, const vector<vector<float>> &matrix){
	float min_d = 100000;
	float max_d = -1;

	for (tDomain s : selected){
		float d = matrix[s][candidate];
		min_d = min(min_d, d);
		max_d = max(max_d, d);
	}

	return max_d - min_d;
}

tSolution GreedyRandomizeAdaptativeSearchProcedure::getGreedySolution(int m, int n, const vector<vector<float>> &matrix){
	// al principio, todos los elementos están disponibles
	vector<tDomain> available(n);
	for(int i = 0; i < n; i++){
		available[i] = i;
	}

	tDomain e0 = Random::get(0, n-1);
	tDomain e1;
	do {
		e1 = Random::get(0, n-1);
	} while (e1 == e0);

	tSolution selected;
	selected.push_back(e0);
	selected.push_back(e1);
	available.erase(remove(available.begin(), available.end(), e0), available.end());
	available.erase(remove(available.begin(), available.end(), e1), available.end());

	while (selected.size() < m){
		vector<tDomain> lc;
		vector<tFitness> heur;

		for (int i = 0; i < n; i++) {
			if (find(selected.begin(), selected.end(), i) == selected.end()){
				tFitness disp = getDisp(selected, i, matrix);
				lc.push_back(i);
				heur.push_back(disp);
			}
		}

		float hmin = *min_element(heur.begin(), heur.end());
		float hmax = *max_element(heur.begin(), heur.end());
		float mu = hmin + 0.2f*(hmax-hmin);

		vector<int> lrc;
		for (int i = 0; i < lc.size(); i++){
			if (heur[i] <= mu) lrc.push_back(lc[i]);
		}
		
		int lrc_size = lrc.size();
		int chosen = lrc[Random::get(0, lrc_size-1)];
		selected.push_back(chosen);
		available.erase(remove(available.begin(), available.end(), chosen), available.end());
	}

	return selected;
}

ResultMH GreedyRandomizeAdaptativeSearchProcedure::optimize(Problem *problem, int maxevals) {
    assert(maxevals > 0);
    MDDProblem* mdd_problem = dynamic_cast<MDDProblem*>(problem); // cast permitido para conseguir la matriz de distancias
	int m = problem->getSolutionSize();            // tamaño de la solución
    int n = problem->getSolutionDomainRange().second + 1; // para conseguir la n      
	vector<vector<float>> matrix = mdd_problem->getMatrix(); // matriz de distancias
	
    RandomLocalSearch bl;
    int evals = 0;
	bool useLS = getUseLS();
    tSolution best;
    tFitness best_fitness = std::numeric_limits<tFitness>::max();

    for (int i = 0; i < 10; i++) {
        tSolution candidate = getGreedySolution(m, n, matrix);
        tFitness fitness = problem->fitness(candidate);
        evals++;

		// búsqueda local opcional
        if (useLS) {
            ResultMH result = bl.optimize(problem, candidate, fitness, maxevals);
            candidate = result.solution;
            fitness = result.fitness;
            evals += result.evaluations;
        }

        if (fitness < best_fitness) {
            best = candidate;
            best_fitness = fitness;
        }
    }

    return {best, best_fitness, evals};
}