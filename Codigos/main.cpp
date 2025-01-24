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

#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

// Función para contar vecinos en un estado específico con vecindad de Small-World
int contarVecinos(const vector<vector<int>>& grid, int x, int y, int estado, int L, double probLongRange) {
    int cuenta = 0;

    // Vecindad de Moore de radio 1
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Recorrer los vecinos locales
    for (int k = 0; k < 8; ++k) {
        // Coordenadas locales periódicas
        int nx = (x + dx[k] + L) % L;
        int ny = (y + dy[k] + L) % L;
        if(dx[k]!=0 && dy[k]!=0)continue;// para que sea Moore la neuman
        if ((double)rand() / RAND_MAX < probLongRange) {
            // Elegir una celda aleatoria en el grid como vecino de larga distancia
            int randomX = rand() % L;
            int randomY = rand() % L;
            // Contar si el vecino de larga distancia está en el estado deseado
            if (grid[randomX][randomY] == estado) {
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
            int vecinosActivos = contarVecinos(grid, i, j, ACTIVO_RIESGO, L,0.75);
            int vecinosSinRiesgo = contarVecinos(grid, i, j, SIN_RIESGO, L,0.75);
            int vecinosEvaluando = contarVecinos(grid, i, j, EVALUACION, L,0.75);
            // Aplicar las reglas de transición
            if (grid[i][j] == ACTIVO_RIESGO && vecinosSinRiesgo == 0 && vecinosEvaluando == 0) {
                nuevoGrid[i][j] = ACTIVO_RIESGO; // Regla 1
            } else if (grid[i][j] == ACTIVO_RIESGO && vecinosSinRiesgo >= vecinosEvaluando) {
                nuevoGrid[i][j] = EVALUACION; // Regla 2
            } else if (grid[i][j] == EVALUACION && (float(rand()) / RAND_MAX) <= probEvaluacion) {
                nuevoGrid[i][j] = SIN_RIESGO; // Regla 3
            }  else if (grid[i][j] == SIN_RIESGO && vecinosActivos >= vecinosSinRiesgo && (float(rand()) / RAND_MAX) <= probRegreso) {
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
    string nombreArchivo_1= "datos_t_inter_Moore1_SW075.txt";

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
    int t_inter_duda=0;
    int intersecciones=0;
    int iteraciones = 100;
    for (int t = 0; t <= iteraciones; t++) {
        contarEstados(grid, L, count0, count1, count2);
        guardarMatrizCada10Iteraciones(grid, L, t, nombreArchivo); // Guardar matriz cada 10 iteraciones
        if(count2<=count0){
            if(t_inter==t)intersecciones++;
            t_inter=t;
        }    
        // cout << count0 << " " << count1 << " " << count2 << endl;
        actualizarGrid(grid, L, probEvaluacion, probRegreso);
    }
    archivo1<< L <<" "<<probEvaluacion<<" "<<probRegreso<<" "<<t_inter<<endl;
    archivo1.close();

    return 0;
}
