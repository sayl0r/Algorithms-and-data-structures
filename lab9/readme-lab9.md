# Лабораторная работа №9: Бинарное дерево. Обходы. BST

**Студент:** Слукин Юрий Алексеевич
**Группа:** ПИЖ-б-о-25-2
**Технология:** C++
**Вариант:** 12

## Содержание
- [Лабораторная работа №9: Бинарное дерево. Обходы. BST](#лабораторная-работа-9-бинарное-дерево-обходы-bst)
  - [Содержание](#содержание)
  - [Цель работы](#цель-работы)
  - [Теоретическое обоснование](#теоретическое-обоснование)
  - [Постановка задачи (Вариант 12)](#постановка-задачи-вариант-12)
  - [Листинг программы](#листинг-программы)
  - [Анализ сложности операций](#анализ-сложности-операций)
  - [Контрольные вопросы](#контрольные-вопросы)
  - [Вывод](#вывод)

## Цель работы

Освоить структуру данных «бинарное дерево», изучить способы его обхода (прямой, симметричный, обратный, уровневый), реализовать бинарное дерево поиска (BST) с операциями вставки, удаления и поиска, а также применить рекурсию для решения практических задач.

## Теоретическое обоснование

**Бинарное дерево** — иерархическая структура, где каждый узел имеет не более двух потомков (левый и правый).

**Обходы дерева:**
*   *Pre-order (Прямой)*: Корень → Левый → Правый.
*   *In-order (Симметричный)*: Левый → Корень → Правый. Даёт отсортированную последовательность для BST.
*   *Post-order (Обратный)*: Левый → Правый → Корень.
*   *Level-order (Уровневый)*: По уровням слева направо (используется очередь).

**BST (Бинарное дерево поиска)** — дерево, где для любого узла все элементы левого поддерева меньше узла, а правого — больше. В среднем операции выполняются за O(log n).

## Постановка задачи (Вариант 12)

1.  **Часть 1 (Бинарное дерево):** Реализовать 4 обхода (preOrder, inOrder, postOrder, levelOrder) + **поиск пути от корня до заданного узла**.
2.  **Часть 2 (BST):** Реализовать вставку, поиск, удаление, обход + **подсчёт узлов в заданном диапазоне [low, high]**.
3.  **Часть 3 (Применение):** Создать BST из 20 случайных чисел, вывести отсортированную последовательность, найти высоту и удалить элемент.

## Листинг программы

```cpp
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
```

## Анализ сложности операций

| Операция | Средний случай (сбалансированное дерево) | Худший случай (вырожденное дерево) | Память |
| :--- | :--- | :--- | :--- |
| **Поиск (search)** | O(log n) | O(n) | O(log n) (стек при рекурсии) |
| **Вставка (insert)** | O(log n) | O(n) | O(log n) |
| **Удаление (remove)** | O(log n) | O(n) | O(log n) |
| **Обходы (traversals)** | O(n) | O(n) | O(n) для level-order (очередь) |
| **Поиск пути (findPath)** | O(n) | O(n) | O(n) |

## Контрольные вопросы

1. **Что такое бинарное дерево? Из каких элементов оно состоит?**
   Иерархическая структура, где каждый узел имеет не более двух потомков. Состоит из корня, узлов, листьев и связей между ними.

2. **В чём отличие бинарного дерева от бинарного дерева поиска (BST)?**
   В BST есть строгое правило: все элементы слева меньше узла, справа — больше. В обычном бинарном дереве такого правила нет.

3. **Какие существуют виды обхода бинарного дерева? В чём их особенности?**
   Pre-order (К-Л-П), In-order (Л-К-П), Post-order (Л-П-К), Level-order (по уровням). Они определяют порядок посещения корня относительно детей.

4. **Какой обход даёт отсортированную последовательность для BST?**
   Симметричный (in-order).

5. **Какова сложность операций поиска, вставки и удаления в BST? От чего она зависит?**
   В среднем O(log n). Зависит от высоты дерева. В худшем случае (вырожденное дерево) — O(n).

6. **Что происходит с BST, если вставлять элементы в отсортированном порядке?**
   Дерево вырождается в связный список, и сложность операций падает до O(n).

7. **Какие три случая нужно рассмотреть при удалении узла из BST?**
   Удаление листа (просто удалить), удаление узла с одним потомком (заменить потомком), удаление узла с двумя потомками (заменить минимальным из правого поддерева).

8. **Как найти наименьший элемент в BST? Наибольший?**
   Наименьший — идти по левым потомкам до конца. Наибольший — идти по правым.

9. **Как вычислить высоту бинарного дерева рекурсивно?**
   1 + максимум из высот левого и правого поддеревьев (базовый случай — 0 для пустого узла).

10. **Что такое наименьший общий предок (LCA) двух узлов в дереве?**
    Самый нижний узел, который является предком одновременно для обоих заданных узлов.

11. **Как проверить, является ли бинарное дерево BST?**
    Выполнить in-order обход и проверить, что последовательность возрастает.

12. **Что такое сбалансированное дерево? Почему это важно для производительности?**
    Дерево, где высота левого и правого поддеревьев отличается не более чем на 1. Это гарантирует сложность O(log n).

13. **Как преобразовать несбалансированное BST в сбалансированное?**
    Получить отсортированный массив через in-order обход и построить из него новое дерево, выбирая середину как корень.

14. **Как восстановить дерево по массиву обхода (in-order + pre-order)?**
    Первый элемент pre-order — корень. Находим его в in-order (все слева — левое поддерево, справа — правое). Рекурсивно повторяем.

15. **Как реализовать итеративный (не рекурсивный) обход бинарного дерева?**
    Использовать стек (для pre/in/post-order) или очередь (для level-order).

## Вывод

В ходе лабораторной работы были изучены структуры данных «бинарное дерево» и «бинарное дерево поиска». Реализованы все виды обходов, а также специфичные задачи варианта 12: поиск пути от корня до узла и подсчёт узлов в диапазоне. Третья часть работы наглядно продемонстрировала, как BST из 20 случайных чисел позволяет вывести отсортированную последовательность с помощью in-order обхода.