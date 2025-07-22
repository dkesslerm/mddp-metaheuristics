## En el directorio software encontramos:
- `common` es el mismo directorio de la plantilla. Solo hemos modificado solution.h para cambiar el tipo de dato que usamos. 
- `data` es un directorio que hemos creado nosotros para incluir en el proyecto los 50 archivos de datos sobre los que trabajamos.
- `inc` es un directorio incluido en la plantilla. Lo hemos modificado para incluir:
    - `GA.h` contiene la declaración de la clase `GeneticAlgorithm`, heredada de `MH`. Tenemos un constructor que acepta el parámetro que indicará el tipo de cruce, un destructor y la declaración de los métodos comunes a los algoritmos genéticos, explicados en la memoria.
    - `heuristicLS.h` contiene la declaración de la clase `HeuristicLocalSearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
    - `mddp.h` contiene la declaración de la clase `MDDProblem`, heredada de `Problem`. Aquí definimos nuestro problema. Incluye los atributos con los que vamos a trabajar, dos constructores, el método `fitness`, el método `createSolution`, los getters de los atributos y el método `getSolutionDomainRange`. 
    - `randomLS.h` contiene la declaración de la clase `RandomLocalSearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
    - `GGA.h` contiene la declaración de la clase `GenerationalGeneticAlgorithm`, heredada de `GeneticAlgorithm`. Tenemos un constructor (heredado), un destructor y la declaración de los métodos `optimize` y `generateChildren`
    - `SGA.h` contiene la declaración de la clase `SteadyStateGeneticAlgorithm`, heredada de `GeneticAlgorithm`. Tenemos un constructor (heredado), un destructor y la declaración de los métodos `optimize` y `generateChildren`
    - `MA.h` contiene la declaración de la clase `MemeticAlgorithm`, heredada de `GenerationalGeneticAlgorithm`. Tenemos un constructor que añade al heredado, un destructor y la declaración del método `optimize`
- `src` es un directorio incluido en la plantilla. Lo hemos modificado para incluir:
    - `GA.cpp` contiene la implementación de los métodos comunes a los algoritmos genéticos, explicados en la memoria.
    - `heuristicLS.cpp` contiene la implementación de `optimize` para el algoritmo de búsqueda local heurística
    - `mddp.cpp` contiene la implementación de los constructores, el método `fitness` y el método `createSolution`
    - `randomLS.cpp` contiene la implementación de `optimize` para el algoritmo de búsqueda local aleatoria
    - `GGA.cpp` contiene la implementación de `optimize` y `generateChildren` para el algoritmo genético generacional
    - `SGA.cpp` contiene la implementación de `optimize` y `generateChildren` para el algoritmo genético estacionario
    - `MA.cpp` contiene la implementación de `optimize` para los algoritmos meméticos

También incluye los archivos: `main.cpp`, donde se lleva a cabo la ejecución principal del programa, `savetofile.cpp`, para guardar los resultados de las ejecuciones en un archivo de texto, `resultados.txt`, el archivo donde se guarda la ejecución de savetofile, `memetic_position_results.txt`, el archivo donde se ha guardado la ejecución última de los algoritmos meméticos con cruce de posición, los ejecutables `main` y `savetofile`, el `README` del repositorio de la plantilla, al igual que la licencia, `CMakeLists.txt` y sus archivos generados al compilar, y otros archivos propios al repositorio. Incluye también este `LEEME.md` y el archivo `KesslerMartínezDavid_P2MH.pdf` que es la memoria de prácticas.