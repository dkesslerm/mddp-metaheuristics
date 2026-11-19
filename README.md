# Metaheuristics – Coursework Portfolio

C++ implementations of several metaheuristic algorithms for the **Minimum Differential Dispersion Problem (MDDP)**, developed across three assignments of the *Metaheuristics* course at **Universidad de Granada** (2024/2025).

> Note: The problem specification, project template and evaluation criteria were provided by the course instructors. The algorithm implementations, experiments and reports are my own work. The reports (`KesslerMartínezDavid_P*MH.pdf`, in Spanish) contain the full design description and results analysis for each assignment.

---

## The Problem

Given `n` elements with pairwise distances, select a subset of exactly `m` elements that minimises the difference between the largest and smallest aggregate dispersion within the subset (each element's dispersion is the sum of its distances to the other selected elements). The problem is NP-hard.

## Structure

- `softwareP1/` – First assignment
  - Random Search
  - Greedy Search
  - Random Local Search
  - Heuristic Local Search

- `softwareP2/` – Second assignment
  - Generational and steady-state Genetic Algorithms, each with two crossover operators (uniform with repair, and position-based)
  - Memetic Algorithms with three hybridisation schemes: local search on the whole population, on 10% of it chosen at random, and on the best 10%

- `softwareP3/` – Third assignment
  - Basic Multi-Start Search
  - Iterated Local Search
  - Simulated Annealing
  - GRASP

- `KesslerMartínezDavid_P1MH.pdf`, `KesslerMartínezDavid_P2MH.pdf`, `KesslerMartínezDavid_P3MH.pdf` – My reports for each assignment *(in Spanish)*

---

## Experimental Setup

- **50 benchmark instances** (GKD-b), with `n` ∈ {25, 50, 100, 125, 150} (10 instances per size)
- Each algorithm run with **5 seeds** per instance: `{42, 0, 31415, 123, 2025}`
- Metrics: average deviation (%) from the best-known solution and execution time

## Key Result (Assignment 2)

The assignment asked for memetic algorithms built only on the crossover that performed best in the generational GA (uniform crossover). Because both crossovers performed almost identically in the GAs and the position-based one was faster, I also built the memetic variants with **position-based crossover** as an extra experiment.

Contrary to what the GA results suggested, the position-based memetics outperformed the uniform ones **at every problem size**. The best algorithm of the assignment was **AM-(10, 0.1best) with position-based crossover** (34.25 on the quality metric, 0.311 s on average), while the genetic algorithms needed more than 1.5 s on average and did not beat heuristic local search.

My hypothesis, stated in the report but not tested experimentally: the near-random repair step of uniform crossover undoes part of the bit-by-bit improvements made by local search, while position-based crossover needs no repair and disturbs those solutions less.

---

## How to Run

Use the provided `Makefile`, which comes from the course template. The only modification is in `CMakeLists.txt`, where a line was added so that `savetofile.cpp` compiles independently of `main.cpp`.

From any of the `softwareP1/`, `softwareP2/` or `softwareP3/` folders:

```bash
cmake .
make
```

This produces two executables:
- `main` – runs the algorithms on a dataset
- `savetofile` – runs the experiments and writes the results to a text file

```bash
./main data/GKD-b_11_n50_m5.txt          # uses the default seed set
./main data/GKD-b_11_n50_m5.txt 42       # uses only seed 42
./savetofile                              # writes results to file
```

---

## Contact

dkesslerm@gmail.com

## License

MIT License. See [`softwareP1/LICENSE`](softwareP1/LICENSE).
