#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono>
#include <iomanip>

using namespace std;

struct CompareComponents {
    const vector<int>& component_global;
    CompareComponents(const vector<int>& components) : component_global(components) {}

    bool operator()(int a, int b) const {
        return component_global[a] < component_global[b];
    }
};

void printMatrix(const vector<vector<int> > &matrix) {
    for (const auto &row : matrix) {
        for (int val : row) {
            cout << setw(3) << val << " ";
        }
        cout << endl;
    }
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N;
    vector<vector<int> > matrix;
    
    if (rank == 0) {
        cout << "Введите размер матрицы (N x N): ";
        cin >> N;
        matrix.resize(N, vector<int>(N));
        cout << "Введите матрицу, построчно:" << endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cin >> matrix[i][j];
            }
        }
    }
    
    auto start_total = std::chrono::high_resolution_clock::now();

    // Рассылка
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        matrix.resize(N, vector<int>(N));
    }
    
    for (int i = 0; i < N; ++i) {
        MPI_Bcast(matrix[i].data(), N, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    int chunk_size = N / size; //строки для потока
    int remainder = N % size; //остаток 
    int start = rank * chunk_size + min(rank, remainder);
    int end = start + chunk_size + (rank < remainder ? 1 : 0) - 1;
    int local_num = end - start + 1;
    
    //Построение списка смежности
    vector<vector<int> > adjacencyList(N);
    for (int i = start; i <= end; ++i) {
        for (int j = 0; j < N; ++j) {
            if (matrix[i][j] != 0 || matrix[j][i] != 0) {
                adjacencyList[i].push_back(j);
            }
        }
    }
    
    vector<int> component_global(N);
    for (int i = 0; i < N; ++i) component_global[i] = i;
    
    vector<int> recvcounts(size); // массив элементов, которые каждый процесс отправляет/получает.
    vector<int> displs(size); // массив, содержащий смещения
    for (int r = 0; r < size; ++r) {
        int r_chunk = N / size;
        int r_remainder = N % size;
        displs[r] = r * r_chunk + min(r, r_remainder);
        recvcounts[r] = r_chunk + (r < r_remainder ? 1 : 0);
    }
    //Поиск компонент связности итеративный 
    bool changed;
    do {
        changed = false;
        vector<int> component_new = component_global;
        
        for (int i = start; i <= end; ++i) {
            int min_comp = component_global[i];
            for (int neighbor : adjacencyList[i]) {
                if (component_global[neighbor] < min_comp) {
                    min_comp = component_global[neighbor];
                }
            }
            if (min_comp < component_new[i]) {
                component_new[i] = min_comp;
                changed = true;
            }
        }
        
        vector<int> buffer(N);
        MPI_Allgatherv(component_new.data() + start, local_num, MPI_INT,
                      buffer.data(), recvcounts.data(), displs.data(), MPI_INT,
                      MPI_COMM_WORLD);
        
        bool global_changed = false;
        for (int i = 0; i < N; ++i) {
            if (buffer[i] < component_global[i]) {
                component_global[i] = buffer[i];
                global_changed = true;
            }
        }
        
        MPI_Allreduce(MPI_IN_PLACE, &global_changed, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);
        changed = global_changed;
    } while (changed);
    
    //Определение порядка перестановки вершин
    vector<int> permutation(N); // порядок вершин после перестановки
    if (rank == 0) {
        for (int i = 0; i < N; ++i) permutation[i] = i;
        sort(permutation.begin(), permutation.end(), CompareComponents(component_global));
    } 
    
    MPI_Bcast(permutation.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    //Формирование новой переставленной матрицы
    vector<vector<int> > sorted_matrix(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            sorted_matrix[i][j] = matrix[permutation[i]][permutation[j]];
        }
    }
    
    auto end_total = std::chrono::high_resolution_clock::now();
    auto duration_total = std::chrono::duration_cast<std::chrono::microseconds>(end_total - start_total);
  

    if (rank == 0) {
        cout << "\nПорядок вершин для получения блочной диагональной формы:";
        for (int p : permutation) cout << p << " ";
        cout << "\n\nПреобразованная матрица (блочная диагональная форма):\n";
        printMatrix(sorted_matrix);
    }
    
    if (rank == 0) {
        std::cout << "Общее время выполнения программы: " << duration_total.count() << " мкс" << std::endl;
    }

    MPI_Finalize();
    return 0;
}