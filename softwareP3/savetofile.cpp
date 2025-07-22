#include <iostream>
#include <fstream>
#include <problem.h> 
#include <random.hpp>
#include <string>
#include <util.h>
#include <map>
#include "mddp.h"
#include <chrono>
using namespace chrono;
using namespace std;

#include "greedy.h"
#include "randomLS.h"
#include "GA.h"
#include "GGA.h"
#include "MA.h"
#include "bmb.h"
#include "ils.h"
#include "simulatedAnnealing.h"
#include "grasp.h"

int get_n_from_filename(const string& filename) {
    size_t n_pos = filename.find("_n");
    if (n_pos == string::npos) return -1;
    size_t m_pos = filename.find("_m", n_pos);
    return stoi(filename.substr(n_pos + 2, m_pos - (n_pos + 2)));
}

int main(int argc, char *argv[]) {
    vector<long int> seeds = {42, 0, 31415, 123, 2025};

    vector<string> files = {
        "GKD-b_1_n25_m2", "GKD-b_2_n25_m2", "GKD-b_3_n25_m2", "GKD-b_4_n25_m2", "GKD-b_5_n25_m2",
        "GKD-b_6_n25_m7", "GKD-b_7_n25_m7", "GKD-b_8_n25_m7", "GKD-b_9_n25_m7", "GKD-b_10_n25_m7",
        "GKD-b_11_n50_m5", "GKD-b_12_n50_m5", "GKD-b_13_n50_m5", "GKD-b_14_n50_m5", "GKD-b_15_n50_m5",
        "GKD-b_16_n50_m15", "GKD-b_17_n50_m15", "GKD-b_18_n50_m15", "GKD-b_19_n50_m15", "GKD-b_20_n50_m15",
        "GKD-b_21_n100_m10", "GKD-b_22_n100_m10", "GKD-b_23_n100_m10", "GKD-b_24_n100_m10", "GKD-b_25_n100_m10",
        "GKD-b_26_n100_m30", "GKD-b_27_n100_m30", "GKD-b_28_n100_m30", "GKD-b_29_n100_m30", "GKD-b_30_n100_m30",
        "GKD-b_31_n125_m12", "GKD-b_32_n125_m12", "GKD-b_33_n125_m12", "GKD-b_34_n125_m12", "GKD-b_35_n125_m12",
        "GKD-b_36_n125_m37", "GKD-b_37_n125_m37", "GKD-b_38_n125_m37", "GKD-b_39_n125_m37", "GKD-b_40_n125_m37",
        "GKD-b_41_n150_m15", "GKD-b_42_n150_m15", "GKD-b_43_n150_m15", "GKD-b_44_n150_m15", "GKD-b_45_n150_m15",
        "GKD-b_46_n150_m45", "GKD-b_47_n150_m45", "GKD-b_48_n150_m45", "GKD-b_49_n150_m45", "GKD-b_50_n150_m45"};

    GreedySearch greedy;
    RandomLocalSearch randLS;
    MemeticAlgorithm memeticBEST(SOME_BEST);
    BasicMultiStart bmb;
    IteratedLocalSearch ils(false);
    IteratedLocalSearch ils_sa(true);
    SimulatedAnnealing es;
    GreedyRandomizeAdaptativeSearchProcedure grasp(false);
    GreedyRandomizeAdaptativeSearchProcedure graspLS(true);   

    struct Algorithm {
        string name;
        MH *algorithm;
        int max_evaluations;
    };

    vector<Algorithm> algoritmos = {
        {"GreedySearch", &greedy, 1},
        {"LocalSearchRandom", &randLS, 100000},
        {"MemeticAlgorithm (BEST)", &memeticBEST, 100000},
        {"BasicMultiStart", &bmb, 10000},
        {"IteratedLocalSearch (BL)", &ils, 10000},
        {"IteratedLocalSearch (SA)", &ils_sa, 10000},
        {"SimulatedAnnealing", &es, 100000},
        {"GRASP-NOBL", &grasp, 10000},
        {"GRASP-SIBL", &graspLS, 10000}
    };

    ofstream xlsFile("results.txt");
    xlsFile << "Algorithm\tFile\tFitness\tTime(s)\tEvals" << endl;

    for (const auto& algoritmo : algoritmos) {
        MH *mh = algoritmo.algorithm;
        int maxevals = algoritmo.max_evaluations;

        map<int, float> total_fitness_by_n, total_time_by_n, total_evals_by_n;
        map<int, int> count_by_n;
        float total_fitness_all = 0.0, total_time_all = 0.0, total_evals_all = 0.0;
        int count_all = 0;

        for (const string &file : files) {
            int n_size = get_n_from_filename(file);
            const string filename = "data/" + file + ".txt";
            MDDProblem rproblem(filename);
            Problem *problem = dynamic_cast<Problem *>(&rproblem);

            float tiempo_medio = 0;
            tFitness fitness_medio = 0;
            float evals_medias = 0;

            for (long int s : seeds) {
                Random::seed(s);
                auto start = high_resolution_clock::now();
                ResultMH result = mh->optimize(problem, maxevals);
                auto end = high_resolution_clock::now();
                fitness_medio += result.fitness;
                duration<float> elapsed = end - start;
                tiempo_medio += elapsed.count();
                evals_medias += result.evaluations;
            }

            fitness_medio /= seeds.size();
            tiempo_medio /= seeds.size();
            evals_medias /= seeds.size();

            total_fitness_by_n[n_size] += fitness_medio;
            total_time_by_n[n_size] += tiempo_medio;
            total_evals_by_n[n_size] += evals_medias;
            count_by_n[n_size]++;

            total_fitness_all += fitness_medio;
            total_time_all += tiempo_medio;
            total_evals_all += evals_medias;
            count_all++;

            xlsFile << algoritmo.name << "\t" << file << "\t" << fitness_medio << "\t" << tiempo_medio << "\t" << evals_medias << endl;
        }

        for (const auto& [n_size, count] : count_by_n) {
            float mean_fit = total_fitness_by_n[n_size] / count;
            float mean_time = total_time_by_n[n_size] / count;
            float mean_evals = total_evals_by_n[n_size] / count;
            xlsFile << algoritmo.name << "\tMEDIA_N" << n_size << "\t" << mean_fit << "\t" << mean_time << "\t" << mean_evals << endl;
        }

        float mean_total_fit = total_fitness_all / count_all;
        float mean_total_time = total_time_all / count_all;
        float mean_total_evals = total_evals_all / count_all;
        xlsFile << algoritmo.name << "\tMEDIA_GLOBAL\t" << mean_total_fit << "\t" << mean_total_time << "\t" << mean_total_evals << endl;
    }

    xlsFile.close();
    return 0;
}
