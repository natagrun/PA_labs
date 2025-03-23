#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace std;

// Функция для вывода матрицы
void printMatrix(const vector<vector<int> > &matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(3) << val << " ";
        }
        cout << endl;
    }
}

// Рекурсивный обход графа (DFS) для нахождения компонент связности
void dfs(int v, int compId, const vector<vector<int> >& adjacencyList, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component[v] = compId;
    for (int w : adjacencyList[v]) {
        if (!visited[w]) {
            dfs(w, compId, adjacencyList, visited, component);
        }
    }
}

// Функция для сравнения вершин по компонентам связности
bool compareByComponent(int a, int b, const vector<int>& component) {
    return component[a] < component[b];
}

int main() {
    int N;
    cout << "Введите размер матрицы (N x N): ";
    cin >> N;
    
    // Ввод матрицы
    vector<vector<int> > matrix(N, vector<int>(N, 0));
    cout << "Введите матрицу, построчно:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    }

    // Замер времени
    auto start = chrono::high_resolution_clock::now();
    
    // Построение графа: для каждой пары (i, j) (где i < j)
    // если matrix[i][j] != 0 или matrix[j][i] != 0, добавляем ребро между i и j
    vector<vector<int> > adjacencyList(N);
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (matrix[i][j] != 0 || matrix[j][i] != 0) {
                adjacencyList[i].push_back(j);
                adjacencyList[j].push_back(i);
            }
        }
    }
    
    // Поиск компонент связности с помощью DFS
    vector<bool> visited(N, false);
    vector<int> component(N, -1);
    int compId = 0;
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            dfs(i, compId, adjacencyList, visited, component);
            compId++;
        }
    }
    
    // Формирование перестановки вершин по компонентам
    vector<int> permutation(N);
    for (int i = 0; i < N; i++) {
        permutation[i] = i;
    }
    
    // Используем обычную функцию для сравнения
    sort(permutation.begin(), permutation.end(), [&](int a, int b) {
        return compareByComponent(a, b, component);
    });
    
    // Формирование преобразованной матрицы (перестановка строк и столбцов)
    vector<vector<int> > outputMatrix(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            outputMatrix[i][j] = matrix[permutation[i]][permutation[j]];
        }
    }

    // Замер времени окончания
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    // Вывод результатов
    cout << "\nИсходная матрица:" << endl;
    printMatrix(matrix);
    
    cout << "\nКомпоненты связности:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Вершина " << i << " принадлежит компоненте " << component[i] << endl;
    }
    
    cout << "\nПорядок вершин для получения блочной диагональной формы:" << endl;
    for (int idx : permutation) {
        cout << idx << " ";
    }
    cout << "\n" << endl;
    
    cout << "Преобразованная матрица (блочная диагональная форма):" << endl;
    printMatrix(outputMatrix);

    cout << "Время выполнения программы: " << duration.count() << " мкс" << endl;
    
    return 0;
}
