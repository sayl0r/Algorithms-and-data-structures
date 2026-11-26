#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Node { 
    int data; 
    Node *left = nullptr, *right = nullptr; 
    Node(int val) : data(val) {} 
};

// Единый класс для бинарного дерева и BST
class Tree {
    Node* root = nullptr;
    
    // УНИВЕРСАЛЬНЫЙ ОБХОД (type: 1 - pre, 2 - in, 3 - post)
    void traverse(Node* node, int type) {
        if (!node) return;
        if (type == 1) cout << node->data << " "; // Корень
        traverse(node->left, type);               // Левый
        if (type == 2) cout << node->data << " "; // Корень
        traverse(node->right, type);              // Правый
        if (type == 3) cout << node->data << " "; // Корень
    }
    
    // ПОИСК ПУТИ ОТ КОРНЯ ДО УЗЛА
    bool findPath(Node* node, int target, vector<int>& path) {
        if (!node) return false; 
        path.push_back(node->data); // Добавляем текущий узел в путь
        
        // Если нашли или нашли в поддеревьях — возвращаем true
        if (node->data == target || findPath(node->left, target, path) || findPath(node->right, target, path)) 
            return true;
            
        path.pop_back(); // Если тупик, убираем узел из пути
        return false;
    }
    
    // ВСТАВКА В BST
    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->data) node->left = insert(node->left, key); 
        else if (key > node->data) node->right = insert(node->right, key);
        return node;
    }
    
    // УДАЛЕНИЕ ИЗ BST (3 случая)
    Node* remove(Node* node, int key) {
        if (!node) return node;
        
        // Ищем узел
        if (key < node->data) node->left = remove(node->left, key);
        else if (key > node->data) node->right = remove(node->right, key);
        else {
            // Случай 1 и 2: Нет левого или нет правого потомка
            if (!node->left) { Node* temp = node->right; delete node; return temp; }
            if (!node->right) { Node* temp = node->left; delete node; return temp; }
            
            // Случай 3: Два потомка. Ищем минимум в правом поддереве
            Node* temp = node->right; 
            while (temp->left) temp = temp->left; 
            node->data = temp->data; // Заменяем значение
            node->right = remove(node->right, temp->data); // Удаляем дубликат
        }
        return node;
    }
    
    // ПОДСЧЁТ УЗЛОВ В ДИАПАЗОНЕ [low, high]
    int countRange(Node* node, int low, int high) {
        if (!node) return 0;
        // Если узел меньше нижней границы, идем только вправо
        if (node->data < low) return countRange(node->right, low, high);
        // Если узел больше верхней границы, идем только влево
        if (node->data > high) return countRange(node->left, low, high);
        // Если узел в диапазоне, считаем его + оба поддерева
        return 1 + countRange(node->left, low, high) + countRange(node->right, low, high);
    }
    
    // Вычисление высоты дерева
    int getHeight(Node* node) { 
        return node ? 1 + max(getHeight(node->left), getHeight(node->right)) : 0; 
    }
    
    // Очистка памяти
    void destroy(Node* node) { 
        if (node) { destroy(node->left); destroy(node->right); delete node; } 
    }

public:
    ~Tree() { destroy(root); }
    
    void insert(int key) { root = insert(root, key); }
    void remove(int key) { root = remove(root, key); }
    
    // Вывод всех обходов
    void printTraversals() {
        cout << "Pre-order:   "; traverse(root, 1); cout << "\n";
        cout << "In-order:    "; traverse(root, 2); cout << "\n";
        cout << "Post-order:  "; traverse(root, 3); cout << "\n";
        cout << "Level-order: ";
        if (root) {
            queue<Node*> q; 
            q.push(root);
            while (!q.empty()) {
                Node* current = q.front(); 
                q.pop(); 
                cout << current->data << " ";
                if (current->left) q.push(current->left); 
                if (current->right) q.push(current->right);
            }
        }
        cout << "\n";
    }
    
    // Вывод пути до узла
    void printPath(int target) {
        vector<int> path;
        if (findPath(root, target, path)) { 
            cout << "Path to " << target << ": "; 
            for (int val : path) cout << val << " -> "; 
            cout << "End\n"; 
        } else {
            cout << "Node " << target << " not found\n";
        }
    }
    
    int countInRange(int low, int high) { return countRange(root, low, high); }
    int getHeight() { return getHeight(root); }
};

int main() {
    cout << "=== PART 1 & 2: Tree & BST Operations ===\n";
    
    // Создаем дерево
    Tree tree;
    for (int val : {10, 5, 15, 3, 7, 20, 6, 18}) tree.insert(val);
    
    // Выводим обходы, путь и статистику
    tree.printTraversals(); 
    tree.printPath(6);
    cout << "Nodes in range [5, 15]: " << tree.countInRange(5, 15) << "\n";
    cout << "Tree height: " << tree.getHeight() << "\n\n";

    cout << "=== PART 3: Random BST ===\n";
    srand(time(0)); 
    Tree randomTree;
    
    // Заполняем случайными числами
    cout << "Random values: ";
    for (int i = 0; i < 20; i++) { 
        int val = rand() % 100; 
        cout << val << " "; 
        randomTree.insert(val); 
    }
    cout << "\n";
    
    // Выводим обходы (In-order будет отсортирован)
    randomTree.printTraversals(); 
    
    // Удаляем элемент
    cout << "\nRemoving 50 (if exists)...\n"; 
    randomTree.remove(50); 
    randomTree.printTraversals();
    
    return 0;
}