#include <iostream>
#include <vector>
#include <cstdlib> // Para rand() y RAND_MAX
#include <ctime>   // Para srand()
#include <fstream> // Para escribir en archivos
#include <utility> // Para std::pair

using namespace std;

// Definir los estados de las celdas
enum Estado {
    ACTIVO_RIESGO = 0,   // 0: Con activos riesgosos
    EVALUACION = 1,      // 1: Evaluación de cambio
    SIN_RIESGO = 2       // 2: Sin activos riesgosos
};

// Generar conexiones aleatorias para una celda
vector<pair<int, int>> generarConexionesAleatorias(int L, int numConexiones) {
    vector<pair<int, int>> conexiones;
    for (int i = 0; i < numConexiones; ++i) {
        int randomX = rand() % L; // Coordenada X aleatoria
        int randomY = rand() % L; // Coordenada Y aleatoria
        conexiones.emplace_back(randomX, randomY);
    }
    return conexiones;
}

// Contar vecinos aleatorios en un estado específico
int contarVecinosAleatorios(const vector<vector<int>>& grid, const vector<pair<int, int>>& conexiones, int estado) {
    int cuenta = 0;
    for (const auto& conexion : conexiones) {
        int nx = conexion.first;
        int ny = conexion.second;
        if (grid[nx][ny] == estado) {
            ++cuenta;
        }
    }
    return cuenta;
}

// Simulación de una iteración del autómata celular
void actualizarGrid(vector<vector<int>>& grid, int L, float probEvaluacion, float probRegreso, const vector<vector<vector<pair<int, int>>>>& conexionesAleatorias) {
    vector<vector<int>> nuevoGrid = grid;

    // Iterar por cada celda
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            int vecinosActivos = contarVecinosAleatorios(grid, conexionesAleatorias[i][j], ACTIVO_RIESGO);
            int vecinosSinRiesgo = contarVecinosAleatorios(grid, conexionesAleatorias[i][j], SIN_RIESGO);
            int vecinosEvaluando = contarVecinosAleatorios(grid, conexionesAleatorias[i][j], EVALUACION);

            // Aplicar las reglas de transición
            if (grid[i][j] == ACTIVO_RIESGO && vecinosSinRiesgo == 0 && vecinosEvaluando == 0) {
                nuevoGrid[i][j] = ACTIVO_RIESGO; // Regla 1
            } else if (grid[i][j] == ACTIVO_RIESGO && vecinosSinRiesgo >= vecinosEvaluando) {
                nuevoGrid[i][j] = EVALUACION; // Regla 2
            } else if (grid[i][j] == EVALUACION && (float(rand()) / RAND_MAX) <= probEvaluacion) {
                nuevoGrid[i][j] = SIN_RIESGO; // Regla 3
            } else if (grid[i][j] == SIN_RIESGO && vecinosActivos >= vecinosSinRiesgo && (float(rand()) / RAND_MAX) <= probRegreso) {
                nuevoGrid[i][j] = ACTIVO_RIESGO; // Regla 4
            } else if (grid[i][j] == SIN_RIESGO && vecinosActivos < vecinosSinRiesgo && (float(rand()) / RAND_MAX) > probRegreso) {
                nuevoGrid[i][j] = SIN_RIESGO; // Regla 5
            }
        }
    }

    // Actualizar el grid
    grid = nuevoGrid;
}

// Función para contar celdas en cada estado y mostrar en consola
void contarEstados(const vector<vector<int>>& grid, int L, int& count0, int& count1, int& count2) {
    count0 = count1 = count2 = 0;

    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            if (grid[i][j] == ACTIVO_RIESGO) count0++;
            else if (grid[i][j] == EVALUACION) count1++;
            else if (grid[i][j] == SIN_RIESGO) count2++;
        }
    }
}

int main(int argc, char* argv[]) {
    int L = atoi(argv[1]);  
    float probEvaluacion = atof(argv[2]); 
    float probRegreso = atof(argv[3]);   
    int numConexiones = 6; // Número de conexiones aleatorias por celda

    srand(time(0)); // Semilla para números aleatorios

    vector<vector<int>> grid(L, vector<int>(L, ACTIVO_RIESGO));
    string nombreArchivo1 = "datos_t_inter_aleatorios6.txt";

    // Generar conexiones aleatorias para cada celda
    vector<vector<vector<pair<int, int>>>> conexionesAleatorias(L, vector<vector<pair<int, int>>>(L));
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            conexionesAleatorias[i][j] = generarConexionesAleatorias(L, numConexiones);
        }
    }

    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            grid[i][j] = (i == L / 2 && j == L / 2) ? 2 : 0; // Inicializar con estado 2 en el centro
        }
    }

    ofstream archivo1(nombreArchivo1, ios::app);

    // Variables para contar los estados
    int count0, count1, count2;
    int t_inter = 0;
    int iteraciones = 1000;

    for (int t = 0; t <= iteraciones; ++t) {
        contarEstados(grid, L, count0, count1, count2);
        if (count2 <= count0) t_inter = t;
        actualizarGrid(grid, L, probEvaluacion, probRegreso, conexionesAleatorias);
    }

    archivo1 << L << " " << probEvaluacion << " " << probRegreso << " " << t_inter << endl;
    archivo1.close();

    return 0;
}
