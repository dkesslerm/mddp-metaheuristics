#include <algorithm>
#include <mddp.h>

MDDProblem::MDDProblem(size_t size) : Problem() { 
    this->n = size; 
}

MDDProblem::MDDProblem(const string &file_name) : Problem() {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Error opening file: " << file_name << endl;
        exit(1);
    }

    file >> n;
    file >> m;

    matrix = vector<vector<float>>(n, vector<float>(n, 0));

    int i, j;
    float aux;

    while (file >> i >> j >> aux) {
        matrix[i][j] = aux;
        matrix[j][i] = aux;
    }

    file.close();
}

tFitness MDDProblem::fitness(const tSolution &solution) {
    
    vector<tFitness> sumas;
    int size = solution.size();

    for (int i = 0; i < size; i++){
        float suma_total = 0;
        for (int j = 0; j < size; j++){
            suma_total += matrix[solution[i]][solution[j]];
        }
        sumas.push_back(suma_total);
    }

    auto minmax = minmax_element(sumas.begin(), sumas.end());
    return *minmax.second - *minmax.first;
}

tSolution MDDProblem::createSolution() {
    // hacemos una lista del 0 al n-1, hacemos shuffle y tomamos los primeros m elementos
    tSolution sol;

    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    Random::shuffle(indices.begin(), indices.end());
    
    for (int i = 0; i < m; i++) {
        sol.push_back(indices[i]);
    }

    return sol;
}
