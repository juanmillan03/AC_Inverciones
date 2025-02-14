#include <iostream>
#include <vector>
#include <cstdlib> // Para rand() y RAND_MAX
#include <ctime>   // Para srand()
#include <fstream> // Para escribir en archivos

using namespace std;

// Definir los estados de las celdas
enum Estado {
    ACTIVO_RIESGO = 0,   // 0: Con activos riesgosos
    EVALUACION = 1,      // 1: Evaluación de cambio
    SIN_RIESGO = 2       // 2: Sin activos riesgosos
};

// Función para obtener el número de vecinos en un estado específico
int contarVecinos(const vector<vector<int>>& grid, int x, int y, int estado, int L) {
    int cuenta = 0;

    // Recorrer el vecindario de radio 2 (distancia de Manhattan <= 2)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Calcular la distancia de Manhattan
            // if (abs(dx) + abs(dy) > 3) continue;

            // Saltar la celda central
            if (dx == 0 && dy == 0) continue;

            // Coordenadas periódicas
            int nx = (x + dx + L) % L; // Coordenada X periódica
            int ny = (y + dy + L) % L; // Coordenada Y periódica

            // Contar si el vecino está en el estado deseado
            if (grid[nx][ny] == estado) {
                ++cuenta;
            }
        }
    }

    return cuenta;
}


// Simulación de una iteración del autómata celular
void actualizarGrid(vector<vector<int>>& grid, int L, float probEvaluacion, float probRegreso) {
    vector<vector<int>> nuevoGrid = grid;

    // Iterar por cada celda
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            int vecinosActivos = contarVecinos(grid, i, j, ACTIVO_RIESGO, L);
            int vecinosSinRiesgo = contarVecinos(grid, i, j, SIN_RIESGO, L);
            int vecinosEvaluando = contarVecinos(grid, i, j, EVALUACION, L);
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
// Función para guardar la matriz completa en el archivo cada 10 pasos de tiempo
void guardarMatrizCada10Iteraciones(const vector<vector<int>>& grid, int L, int iteracion, const string& nombreArchivo) {
    // Solo guardar en iteraciones múltiplos de 10
    if (iteracion % 10 == 0) {
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo) {
            cerr << "Error al abrir el archivo " << nombreArchivo << endl;
            return;
        }
        archivo << "Iteración " << iteracion << " - Representación de la matriz:" << endl;
        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < L; ++j) {
                archivo << grid[i][j] << " ";
            }
            archivo << endl;
        }
        archivo << endl; // Separador entre matrices
        archivo.close();
    }
}



int main(int argc, char* argv[] ) {

    int L = atoi(argv[1]);  
    float probEvaluacion = atof(argv[2]); 
    float probRegreso = atof(argv[3]);   
    srand(time(0)); // Semilla para números aleatorios
    
    vector<vector<int>> grid(L, vector<int>(L, ACTIVO_RIESGO));
    string nombreArchivo = "datos_matrix.txt";
    string nombreArchivo_1= "datos_t_inter_Moore1.txt";

    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            grid[i][j] = (i==L/2 && j==L/2) ? 2 : 0; // Estados 0, 1 o 2
        }
    }
    ofstream archivo(nombreArchivo);
    archivo.close();

    ofstream archivo1(nombreArchivo_1, ios::app);

    // Variables para contar los estados
    int count0, count1, count2;
    int t_inter=0;
    int iteraciones = 1000;
    for (int t = 0; t <= iteraciones; ++t) {
        contarEstados(grid, L, count0, count1, count2);
        // guardarMatrizCada10Iteraciones(grid, L, t, nombreArchivo); // Guardar matriz cada 10 iteraciones
        if(count2<=count0)t_inter=t;
        // cout << count0 << " " << count1 << " " << count2 << endl;
        actualizarGrid(grid, L, probEvaluacion, probRegreso);
    }
    archivo1<< L <<" "<<probEvaluacion<<" "<<probRegreso<<" "<<t_inter<<endl;
    archivo1.close();

    return 0;
}
