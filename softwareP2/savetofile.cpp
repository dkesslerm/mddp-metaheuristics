#include <iostream>
#include <fstream>
#include <problem.h> 
#include <random.hpp>
#include <string>
#include <util.h>
#include "mddp.h"
#include <chrono>
using namespace chrono;
using namespace std;

#include "randomLS.h"
#include "GA.h"
#include "GGA.h"
#include "SGA.h"
#include "MA.h"

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

    vector<pair<string, MH *>> algoritmos = {
        {"GenerationalGeneticAlgorithm (Uniform crossover)", new GenerationalGeneticAlgorithm(true)},
        {"GenerationalGeneticAlgorithm (Position crossover)", new GenerationalGeneticAlgorithm(false)},
        {"SteadyStateGeneticAlgorithm (Uniform crossover)", new SteadyStateGeneticAlgorithm(true)},
        {"SteadyStateGeneticAlgorithm (Position crossover)", new SteadyStateGeneticAlgorithm(false)},
        {"MemeticAlgorithm (ALL)", new MemeticAlgorithm(ALL)},
        {"MemeticAlgorithm (SOME_RANDOM)", new MemeticAlgorithm(SOME_RANDOM)},
        {"MemeticAlgorithm (SOME_BEST)", new MemeticAlgorithm(SOME_BEST)}
    };

    ofstream xlsFile("results.txt");
    xlsFile << "Algorithm\tFile\tFitness\tTime(s)" << endl;

    for (auto &algoritmo : algoritmos) {
        MH *mh = algoritmo.second;
        for (const string &file : files) {
            const string filename = "data/" + file + ".txt";
            MDDProblem rproblem(filename);
            Problem *problem = dynamic_cast<Problem *>(&rproblem);
            float tiempo_medio = 0;
            tFitness fitness_medio = 0;

            for (long int s : seeds) {
                Random::seed(s);
                auto start = high_resolution_clock::now();
                ResultMH result = mh->optimize(problem, 100000);
                auto end = high_resolution_clock::now();
                fitness_medio += result.fitness;
                duration<float> elapsed = end - start;
                tiempo_medio += elapsed.count();
            }

            fitness_medio /= seeds.size();
            tiempo_medio /= seeds.size();
            xlsFile << algoritmo.first << "\t" << file << "\t" << fitness_medio << "\t" << tiempo_medio << endl;
        }
    }

    xlsFile.close();
    return 0;
}