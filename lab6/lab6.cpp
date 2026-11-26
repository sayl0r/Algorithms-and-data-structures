#include <iostream>
#include <string>

using namespace std;

// РЕКУРСИВНЫЕ АЛГОРИТМЫ (Вариант 12)

// Задача 1: Факториал числа
long long fact(int n) { return (n <= 1) ? 1 : n * fact(n - 1); }

// Задача 2: Числа Фибоначчи
int fib(int n) { return (n <= 1) ? n : fib(n - 1) + fib(n - 2); }

// Задача 3: Проверка сбалансированности скобок
bool checkBrackets(string s, int i = 0, int bal = 0) {
    if (bal < 0) return false;
    if (i == s.length()) return bal == 0;
    if (s[i] == '(' || s[i] == '[' || s[i] == '{') return checkBrackets(s, i + 1, bal + 1);
    if (s[i] == ')' || s[i] == ']' || s[i] == '}') return checkBrackets(s, i + 1, bal - 1);
    return checkBrackets(s, i + 1, bal);
}

// Задача 4: Генерация всех комбинаций
void genComb(int n, string cur = "") {
    if (cur.length() == n) { cout << cur << " "; return; }
    genComb(n, cur + "0"); genComb(n, cur + "1");
}

// ПРОСТЫЕ СОРТИРОВКИ

// 1. Пузырьковая сортировка (Bubble Sort)
void bubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool sw = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) { swap(a[j], a[j + 1]); sw = true; }
        }
        if (!sw) break; // Оптимизация
    }
}

// 2. Сортировка выбором (Selection Sort)
void selectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIdx]) minIdx = j;
        }
        if (minIdx != i) swap(a[i], a[minIdx]);
    }
}

// 3. Рекурсивная сортировка вставками (Доп. задание 12 варианта)
void insertionSortRec(int a[], int n) {
    if (n <= 1) return;
    insertionSortRec(a, n - 1);
    int last = a[n - 1], j = n - 2;
    while (j >= 0 && a[j] > last) { 
        a[j + 1] = a[j]; 
        j--; 
    }
    a[j + 1] = last;
}

// Вспомогательная функция для вывода массива
void print(int a[], int n) {
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "\n";
}

int main() {
    // Тест рекурсивных алгоритмов
    cout << "RECURSION\n";
    cout << "Fact(5): " << fact(5) << "\n";
    cout << "Fib(10): " << fib(10) << "\n";
    cout << "Brackets '{[()]}': " << (checkBrackets("{[()]}") ? "true" : "false") << "\n";
    cout << "Combinations(2): "; genComb(2); cout << "\n\n";

    // Тест сортировок
    cout << "SORTING\n";
    int a1[] = {5, 2, 9, 1, 4}, a2[] = {5, 2, 9, 1, 4}, a3[] = {5, 2, 9, 1, 4};
    int n = 5;

    // 1. Пузырьковая
    bubbleSort(a1, n);        
    cout << "1. Bubble Sort:    "; print(a1, n);

    // 2. Выбором
    selectionSort(a2, n);     
    cout << "2. Selection Sort: "; print(a2, n);

    // 3. Рекурсивная вставками (Доп. задание)
    insertionSortRec(a3, n);  
    cout << "3. Insertion Sort (Recursive): "; print(a3, n);

    return 0;
}