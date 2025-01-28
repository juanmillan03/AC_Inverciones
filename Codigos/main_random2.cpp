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

// Generar conexiones con una probabilidad para cada celda
vector<pair<int, int>> generarConexionesProbabilisticas(int x, int y, int L, double probConexion) {
    vector<pair<int, int>> conexiones;

    // Recorrer todas las posibles celdas
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            // Evitar conectar una celda consigo misma
            if (i == x && j == y) continue;

            // Conectar con probabilidad probConexion
            if ((double)rand() / RAND_MAX < probConexion) {
                conexiones.emplace_back(i, j);
            }
        }
    }

    return conexiones;
}

// Contar vecinos probabilísticos en un estado específico
int contarVecinosProbabilisticos(const vector<vector<int>>& grid, const vector<pair<int, int>>& conexiones, int estado) {
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
void actualizarGrid(vector<vector<int>>& grid, int L, float probEvaluacion, float probRegreso, const vector<vector<vector<pair<int, int>>>>& conexionesProbabilisticas) {
    vector<vector<int>> nuevoGrid = grid;

    // Iterar por cada celda
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            int vecinosActivos = contarVecinosProbabilisticos(grid, conexionesProbabilisticas[i][j], ACTIVO_RIESGO);
            int vecinosSinRiesgo = contarVecinosProbabilisticos(grid, conexionesProbabilisticas[i][j], SIN_RIESGO);
            int vecinosEvaluando = contarVecinosProbabilisticos(grid, conexionesProbabilisticas[i][j], EVALUACION);

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
    double probConexion = 0.01; // Probabilidad de conexión entre celdas

    srand(time(0)); // Semilla para números aleatorios

    vector<vector<int>> grid(L, vector<int>(L, ACTIVO_RIESGO));
    string nombreArchivo1 = "datos_t_inter_probabilisticos"+ to_string(probConexion) + ".txt";

    // Generar conexiones probabilísticas para cada celda
    vector<vector<vector<pair<int, int>>>> conexionesProbabilisticas(L, vector<vector<pair<int, int>>>(L));
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            conexionesProbabilisticas[i][j] = generarConexionesProbabilisticas(i, j, L, probConexion);
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
        actualizarGrid(grid, L, probEvaluacion, probRegreso, conexionesProbabilisticas);
    }

    archivo1 << L << " " << probEvaluacion << " " << probRegreso  << " " << t_inter << endl;
    archivo1.close();

    return 0;
}
