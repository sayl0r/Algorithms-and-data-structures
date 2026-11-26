#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ДВОИЧНЫЙ ПОИСК (Подсчёт вхождений)
int countOccurrences(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    int first = -1, last = -1;

    // Ищем первое вхождение
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) { first = mid; right = mid - 1; }
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    if (first == -1) return 0; // Не найдено

    // Ищем последнее вхождение
    left = 0; right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) { last = mid; left = mid + 1; }
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return last - first + 1;
}

// ХЕШ-ТАБЛИЦА (Линейное пробирование, Student)
struct Student {
    string name;
    int age;
};

class HashTable {
private:
    struct Entry {
        int key;
        Student value;
        bool occupied = false;
    };
    vector<Entry> table;
    int size;

    int hash(int key) { return key % size; }

public:
    HashTable(int s) : size(s), table(s) {}

    void insert(int key, Student val) {
        int index = hash(key);
        int i = 0;
        while (table[(index + i) % size].occupied) {
            if (table[(index + i) % size].key == key) { // Обновление
                table[(index + i) % size].value = val;
                return;
            }
            i++;
        }
        int pos = (index + i) % size;
        table[pos] = {key, val, true};
    }

    Student* find(int key) {
        int index = hash(key);
        int i = 0;
        while (table[(index + i) % size].occupied) {
            int pos = (index + i) % size;
            if (table[pos].key == key) return &table[pos].value;
            i++;
        }
        return nullptr;
    }
};

int main() {
    cout << "--- PART 1: Binary Search (Count Occurrences) ---\n";
    vector<int> sortedArr = {1, 2, 2, 2, 3, 4, 5, 5, 6};
    int target = 2;
    cout << "Array: 1, 2, 2, 2, 3, 4, 5, 5, 6\n";
    cout << "Count of " << target << ": " << countOccurrences(sortedArr, target) << "\n\n";

    cout << "--- PART 2: Hash Table (Linear Probing, Student) ---\n";
    HashTable ht(5);
    ht.insert(101, {"Ivan", 19});
    ht.insert(102, {"Maria", 20});
    ht.insert(106, {"Petr", 21}); // Коллизия с 101 (106 % 5 == 1, 101 % 5 == 1)
    
    Student* s = ht.find(106);
    if (s) cout << "Found ID 106: " << s->name << ", Age: " << s->age << "\n";
    
    if (ht.find(999) == nullptr) cout << "ID 999 not found (correct)\n\n";

    cout << "--- PART 3: Comparative Analysis ---\n";
    cout << "Binary Search: O(log N)\n";
    cout << "Hash Table:    O(1) average, O(N) worst\n";

    return 0;
}