#include <iostream>
#include <vector>
#include <queue>
#include <deque>

// СТЕК
class CustomStack {
    std::vector<int> data;
public:
    void push(int val) { data.push_back(val); }
    void pop() { if (!data.empty()) data.pop_back(); }
    int top() { return data.empty() ? -1 : data.back(); }
    int getMiddle() { return data.empty() ? -1 : data[data.size() / 2]; }
};

// КУЧА
bool isMaxHeap(const std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i <= (n - 2) / 2; ++i) {
        int left = 2 * i + 1, right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
    }
    return true;
}

int main() {
    // Стек
    std::cout << "--- 1. Stack with getMiddle() ---\n";
    CustomStack s;
    s.push(10); s.push(20); s.push(30); s.push(40); s.push(50);
    std::cout << "Top: " << s.top() << ", Middle: " << s.getMiddle() << "\n\n";

    // Очередь
    std::cout << "--- 2. Split Queue (Even/Odd) ---\n";
    std::queue<int> q;
    for (int i = 1; i <= 6; ++i) q.push(i * 10);
    std::queue<int> evenPos, oddPos;
    int pos = 1;
    while (!q.empty()) {
        int val = q.front(); q.pop();
        (pos % 2 == 0) ? evenPos.push(val) : oddPos.push(val);
        pos++;
    }
    std::cout << "Odd: "; while (!oddPos.empty()) { std::cout << oddPos.front() << " "; oddPos.pop(); }
    std::cout << "\nEven: "; while (!evenPos.empty()) { std::cout << evenPos.front() << " "; evenPos.pop(); }
    std::cout << "\n\n";

    // Дек
    std::cout << "--- 3. Deque with insertAt(index) ---\n";
    std::deque<int> dq = {10, 20, 40, 50};
    dq.insert(dq.begin() + 2, 30); // Вставка 30 на индекс 2
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n\n";

    // Приоритетная очередь
    std::cout << "--- 4. Check Binary Heap ---\n";
    std::vector<int> heap = {90, 15, 10, 7, 12, 2};
    std::vector<int> not_heap = {10, 20, 30, 40, 50};
    std::cout << "Array {90, 15, 10, 7, 12, 2} is heap? " << (isMaxHeap(heap) ? "Yes" : "No") << "\n";
    std::cout << "Array {10, 20, 30, 40, 50} is heap? " << (isMaxHeap(not_heap) ? "Yes" : "No") << "\n";

    return 0;
}