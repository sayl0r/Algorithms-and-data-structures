#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// БЫСТРАЯ СОРТИРОВКА (Quick Sort)
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ПИРАМИДАЛЬНАЯ СОРТИРОВКА (Heap Sort)
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// РАСПРЕДЕЛЕННАЯ СОРТИРОВКА СЛИЯНИЕМ (Merge Sort / MapReduce)
// Функция для слияния результатов (Фаза Reduce)
vector<int> distributedMerge(const vector<vector<int>>& nodes) {
    if (nodes.empty()) return {};
    vector<int> result = nodes[0];
    for (size_t i = 1; i < nodes.size(); ++i) {
        vector<int> temp;
        temp.reserve(result.size() + nodes[i].size());
        std::merge(result.begin(), result.end(), nodes[i].begin(), nodes[i].end(), back_inserter(temp));
        result = move(temp);
    }
    return result;
}

int main() {
    const int N = 100000; // Размер массива
    vector<int> original(N);
    
    // Заполняем массив случайными числами
    for (int i = 0; i < N; ++i) original[i] = rand() % 100000;

    cout << "Array size: " << N << "\n\n";

    // Тест Quick Sort
    vector<int> arr1 = original;
    auto start = high_resolution_clock::now();
    quickSort(arr1, 0, N - 1);
    auto end = high_resolution_clock::now();
    cout << "1. Quick Sort: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // Тест Heap Sort
    vector<int> arr2 = original;
    start = high_resolution_clock::now();
    heapSort(arr2);
    end = high_resolution_clock::now();
    cout << "2. Heap Sort:  " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // Тест Распределенной сортировки слиянием (Вариант 12)
    cout << "\n3. Distributed Merge Sort (MapReduce Simulation)\n";
    int numNodes = 4; // Эмулируем 4 сервера
    int chunkSize = N / numNodes;
    vector<vector<int>> nodes(numNodes);

    // Фаза Map: разбиваем данные по узлам
    for (int i = 0; i < N; ++i) {
        nodes[i / chunkSize].push_back(original[i]);
    }

    start = high_resolution_clock::now();
    
    // Каждый узел сортирует свои данные
    for (int i = 0; i < numNodes; ++i) {
        sort(nodes[i].begin(), nodes[i].end()); 
    }
    
    // Фаза Reduce: сборка и слияние
    vector<int> finalResult = distributedMerge(nodes);
    end = high_resolution_clock::now();
    cout << "   Time with " << numNodes << " nodes: " 
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}