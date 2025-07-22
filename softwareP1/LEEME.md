## En el directorio software encontramos:
- `common` es el mismo directorio de la plantilla. Solo hemos modificado solution.h para cambiar el tipo de dato que usamos. 
- `data` es un directorio que hemos creado nosotros para incluir en el proyecto los 50 archivos de datos sobre los que trabajamos.
- `inc` es un directorio incluido en la plantilla. Lo hemos modificado para incluir:
    - `greedy.h` contiene la declaración de la clase `GreedySearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
    - `heuristicLS.h` contiene la declaración de la clase `HeuristicLocalSearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
    - `mddp.h` contiene la declaración de la clase `MDDProblem`, heredada de `Problem`. Aquí definimos nuestro problema. Incluye los atributos con los que vamos a trabajar, dos constructores, el método `fitness`, el método `createSolution`, los getters de los atributos y el método `getSolutionDomainRange`. 
    - `randomLS.h` contiene la declaración de la clase `RandomLocalSearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
    - `randomsearch.h` contiene la declaración de la clase `RandomSearch`, heredada de `MH`. Tenemos un constructor, un destructor y el método `optimize`
- `src` es un directorio incluido en la plantilla. Lo hemos modificado para incluir:
    - `greedy.cpp` contiene la implementación de `optimize` para el algoritmo greedy
    - `heuristicLS.cpp` contiene la implementación de `optimize` para el algoritmo de búsqueda local heurística
    - `mddp.cpp` contiene la implementación de los constructores, el método `fitness` y el método `createSolution`
    - `randomLS.cpp` contiene la implementación de `optimize` para el algoritmo de búsqueda local aleatoria
    - `randomsearch.cpp` contiene la implementación (de la plantilla) de `optimize` para el algoritmo de búsqueda aleatoria

También incluye los archivos: `main.cpp`, donde se lleva a cabo la ejecución principal del programa, `savetofile.cpp`, para guardar los resultados de las ejecuciones en un archivo de texto, `resultados.txt` que es el archivo donde se guarda la ejecución de savetofile, los ejecutables `main` y `savetofile`, el `README` del repositorio de la plantilla, al igual que la licencia, `CMakeLists.txt`y también este `LEEME.md`
