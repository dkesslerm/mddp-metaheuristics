# 🧠 Metaheuristics – Coursework Portfolio

This repository contains implementations of several metaheuristic algorithms developed as part of the *Metaheuristics* course at **Universidad de Granada**.  
The project addresses the **Minimum Differential Dispersion Problem (MDDP)** across three structured assignments.

> ⚠️ Note: This work was completed for academic purposes. The structure and problem specifications were provided by the course instructors. Assignment briefs (*guiones de prácticas*) are included for context and remain in their original Spanish form.

---

## 📁 Structure

- `softwareP1/` – First assignment  
  Implements:
  - Random Search  
  - Greedy Search  
  - Random Local Search  
  - Heuristic Local Search  

- `softwareP2/` – Second assignment  
  Implements:
  - Multiple variants of Genetic and Memetic Algorithms  

- `softwareP3/` – Third assignment  
  Implements:
  - Basic Multi-Search  
  - Iterated Local Search  
  - Simulated Annealing  
  - GRASP  

- `KesslerMartínezDavid_P1MH.pdf` – Assignment brief for P1 *(in Spanish)*  
- `KesslerMartínezDavid_P2MH.pdf` – Assignment brief for P2 *(in Spanish)*  
- `KesslerMartínezDavid_P3MH.pdf` – Assignment brief for P3 *(in Spanish)*  

---

## ⚙️ How to Run

To compile and run the project, use the provided `Makefile`, which comes from the course template.  
The only modification made was to the `CMakeLists.txt` file — a line was added to enable compiling `savetofile.cpp` independently of `main.cpp`.

### 🔧 Compilation Steps

From the root directory of any of the `softwareP1/`, `softwareP2/`, or `softwareP3/` folders:

```bash
cmake .
make
```

This generates two executables:
- `main` – the primary entry point for running algorithms on datasets.
- `savetofile` – a utility to export results to a text file.

### ▶️ Running the Executables

```bash
./main data/<filename> <optional-seed>
./savetofile
```

- The `main` executable accepts two arguments:
  1. The path to the dataset file (located in the `data/` folder).
  2. *(Optional)* A numeric seed for random number generation.

If no seed is provided, the program defaults to a predefined list of five seeds:  
`{42, 0, 31415, 123, 2025}`.

### 💡 Example Usage

```bash
./main data/GKD_b_7_n25_m7.txt          # uses default seed set
./main data/GKD_b_7_n25_m7.txt 42       # uses only seed 42
./savetofile                            # outputs results to file
```

---

## 🧪 Techniques Used

Across the assignments, the following metaheuristics were explored and implemented:

- Randomized Search Strategies  
- Local Search Variants  
- Genetic Algorithms  
- Memetic Algorithms  
- Simulated Annealing  
- Iterated Local Search  
- GRASP  
- Multi-start Strategies  

Each algorithm was tailored for the MDDP and tested on benchmark instances.

---

## 📌 Disclaimer

- The **assignment structure** and **evaluation criteria** were provided by the instructors of the *Metaheuristics* course.  
- The **problem and algorithm implementations, experimentation, and reports** in this repository are original student work, unless otherwise stated.

---

## 📬 Contact

For inquiries or references, contact: **[dkesslerm@gmail.com]**

---

## 📝 License

This project is licensed under the **MIT License**. See [`LICENSE`](LICENSE) for details.
