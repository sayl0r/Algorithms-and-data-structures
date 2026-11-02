#include <iostream>
#include <map>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace std::chrono;

// Класс разреженного массива
template <typename T>
class SparseArray {
private:
    map<int, T> data; // Храним только заполненные индексы (индекс -> значение)

public:
    // Установить значение по индексу
    void set(int index, T value) {
        data[index] = value;
    }

    // Получить значение по индексу (если пусто, вернет 0 или дефолтное значение типа)
    T get(int index) {
        if (data.find(index) != data.end()) {
            return data[index];
        }
        return T(); 
    }

    // Удалить значение по индексу
    void remove(int index) {
        data.erase(index);
    }

    // Вернуть логический размер (максимальный индекс + 1)
    int size() {
        if (data.empty()) return 0;
        return data.rbegin()->first + 1;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    // Для теста используем обычный map из STL в качестве конкурента
    map<int, int> stlMap;
    SparseArray<int> myArray;

    const int N = 100000; // Количество операций

    // Тест 1: Запись (set)
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        myArray.set(i * 10, i); // Записываем с шагом 10 (создаем "дырки")
    }
    auto end = high_resolution_clock::now();
    cout << "Время записи (set): " << duration_cast<milliseconds>(end - start).count() << " мс\n";

    // Тест 2: Чтение (get)
    start = high_resolution_clock::now();
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += myArray.get(i * 10);
    }
    end = high_resolution_clock::now();
    cout << "Время чтения (get): " << duration_cast<milliseconds>(end - start).count() << " мс\n";

    return 0;
}