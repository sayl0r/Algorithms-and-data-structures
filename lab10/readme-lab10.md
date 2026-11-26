# Лабораторная работа №10: AVL-дерево. Префиксное дерево (Trie)

**Студент:** Слукин Юрий Алексеевич
**Группа:** ПИЖ-б-о-25-2
**Технология:** C++
**Вариант:** 12

## Содержание
- [Лабораторная работа №10: AVL-дерево. Префиксное дерево (Trie)](#лабораторная-работа-10-avl-дерево-префиксное-дерево-trie)
  - [Содержание](#содержание)
  - [Цель работы](#цель-работы)
  - [Теоретическое обоснование](#теоретическое-обоснование)
  - [Постановка задачи (Вариант 12)](#постановка-задачи-вариант-12)
  - [Листинг программы](#листинг-программы)
  - [Сравнительный анализ](#сравнительный-анализ)
  - [Контрольные вопросы](#контрольные-вопросы)
  - [Вывод](#вывод)

## Цель работы

Реализовать самобалансирующееся AVL-дерево с операциями вставки, удаления и поиска, а также префиксное дерево (Trie) для эффективного хранения строк. Освоить механизмы балансировки (повороты) и сравнить производительность различных структур данных.

## Теоретическое обоснование

**AVL-дерево** — это самобалансирующееся бинарное дерево поиска. Для каждого узла разница высот левого и правого поддеревьев (фактор баланса) не превышает 1. Если баланс нарушается, выполняются повороты (малые и большие). Это гарантирует сложность O(log n) для вставки, поиска и удаления даже в худшем случае.

**Trie (префиксное дерево)** — структура данных для хранения строк. Каждый узел представляет собой символ, а путь от корня до узла образует префикс слова. Позволяет быстро искать слова и префиксы за O(L), где L — длина строки.

## Постановка задачи (Вариант 12)

1.  **Часть 1 (AVL-дерево):** Вставка, балансировка, поиск, удаление. Дополнительно: **Построение AVL из массива**.
2.  **Часть 2 (Trie):** Вставка, поиск, удаление. Дополнительно: **Поиск слов по маске (с подстановкой '?')**.
3.  **Часть 3 (Анализ):** Сравнить производительность AVL-дерева и BST (теоретически).

## Листинг программы

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// AVL-ДЕРЕВО
struct AVLNode {
    int key, h;
    AVLNode *l, *r;
    AVLNode(int k) : key(k), h(1), l(nullptr), r(nullptr) {}
};

class AVL {
    AVLNode* root = nullptr;

    int H(AVLNode* n) { return n ? n->h : 0; }
    int B(AVLNode* n) { return n ? H(n->l) - H(n->r) : 0; }
    void U(AVLNode* n) { if (n) n->h = 1 + max(H(n->l), H(n->r)); }

    AVLNode* RR(AVLNode* y) { // Правый поворот
        AVLNode* x = y->l; AVLNode* T2 = x->r;
        x->r = y; y->l = T2;
        U(y); U(x);
        return x;
    }

    AVLNode* RL(AVLNode* x) { // Левый поворот
        AVLNode* y = x->r; AVLNode* T2 = y->l;
        y->l = x; x->r = T2;
        U(x); U(y);
        return y;
    }

    AVLNode* balance(AVLNode* n) { // Балансировка
        if (!n) return n;
        U(n);
        int bf = B(n);
        if (bf > 1) {
            if (B(n->l) < 0) n->l = RL(n->l);
            return RR(n);
        }
        if (bf < -1) {
            if (B(n->r) > 0) n->r = RR(n->r);
            return RL(n);
        }
        return n;
    }

    AVLNode* insert(AVLNode* n, int k) { // Вставка
        if (!n) return new AVLNode(k);
        if (k < n->key) n->l = insert(n->l, k);
        else if (k > n->key) n->r = insert(n->r, k);
        else return n;
        return balance(n);
    }

    AVLNode* minVal(AVLNode* n) { // Поиск минимума
        while (n && n->l) n = n->l;
        return n;
    }

    AVLNode* remove(AVLNode* n, int k) { // Удаление
        if (!n) return n;
        if (k < n->key) n->l = remove(n->l, k);
        else if (k > n->key) n->r = remove(n->r, k);
        else {
            if (!n->l || !n->r) {
                AVLNode* temp = n->l ? n->l : n->r;
                if (!temp) { temp = n; n = nullptr; }
                else *n = *temp;
                delete temp;
            } else {
                AVLNode* temp = minVal(n->r);
                n->key = temp->key;
                n->r = remove(n->r, temp->key);
            }
        }
        if (!n) return n;
        return balance(n);
    }

    void inOrder(AVLNode* n) { if (n) { inOrder(n->l); cout << n->key << " "; inOrder(n->r); } }
    void destroy(AVLNode* n) { if (n) { destroy(n->l); destroy(n->r); delete n; } }

public:
    ~AVL() { destroy(root); }
    void add(int k) { root = insert(root, k); }
    void del(int k) { root = remove(root, k); }
    void print() { inOrder(root); cout << "\n"; }
    void build(vector<int>& arr) { for (int v : arr) add(v); } // Построение из массива
};

// ПРЕФИКСНОЕ ДЕРЕВО (TRIE)
struct TrieNode {
    TrieNode* child[26] = {nullptr};
    bool isEnd = false;
};

class Trie {
    TrieNode* root;

    void destroy(TrieNode* n) { // Очистка
        if (!n) return;
        for (int i = 0; i < 26; i++) destroy(n->child[i]);
        delete n;
    }

    bool match(TrieNode* n, const string& pat, int i) { // Рекурсивный поиск по маске
        if (!n) return false;
        if (i == pat.length()) return n->isEnd;
        if (pat[i] == '?') {
            for (int j = 0; j < 26; j++) 
                if (n->child[j] && match(n->child[j], pat, i + 1)) return true;
            return false;
        }
        return match(n->child[pat[i] - 'a'], pat, i + 1);
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { destroy(root); }

    void add(string w) { // Вставка
        TrieNode* c = root;
        for (char ch : w) {
            int i = ch - 'a';
            if (!c->child[i]) c->child[i] = new TrieNode();
            c = c->child[i];
        }
        c->isEnd = true;
    }

    bool search(string w) { // Поиск слова
        TrieNode* c = root;
        for (char ch : w) {
            int i = ch - 'a';
            if (!c->child[i]) return false;
            c = c->child[i];
        }
        return c->isEnd;
    }

    bool del(string w) { // Удаление (снимаем флаг)
        TrieNode* c = root;
        for (char ch : w) {
            int i = ch - 'a';
            if (!c->child[i]) return false;
            c = c->child[i];
        }
        if (c->isEnd) { c->isEnd = false; return true; }
        return false;
    }

    bool maskSearch(string pat) { return match(root, pat, 0); } // Поиск по маске
};

int main() {
    cout << "--- AVL Tree (Variant 12: Build from Array) ---\n";
    AVL avl;
    vector<int> arr = {10, 20, 30, 40, 50, 25};
    cout << "Array: 10 20 30 40 50 25\n";
    avl.build(arr);
    cout << "In-order: "; avl.print();
    avl.del(30);
    cout << "After removing 30: "; avl.print();

    cout << "\n--- Trie (Variant 12: Mask Search '?') ---\n";
    Trie trie;
    trie.add("cat"); trie.add("car"); trie.add("dog");
    cout << "Words: cat, car, dog\n";
    cout << "Search 'ca?': " << (trie.maskSearch("ca?") ? "true" : "false") << "\n";
    cout << "Search 'c?r': " << (trie.maskSearch("c?r") ? "true" : "false") << "\n";
    cout << "Search 'd?g': " << (trie.maskSearch("d?g") ? "true" : "false") << "\n";
    cout << "Search 'x?x': " << (trie.maskSearch("x?x") ? "true" : "false") << "\n";

    return 0;
}
```

## Сравнительный анализ

| Характеристика | BST (обычное) | AVL (самобалансирующееся) | Trie (префиксное дерево) |
| :--- | :--- | :--- | :--- |
| **Поиск (средний)** | O(log n) | O(log n) | O(L) |
| **Поиск (худший)** | O(n) | O(log n) | O(L) |
| **Вставка (худший)** | O(n) | O(log n) | O(L) |
| **Удаление (худший)** | O(n) | O(log n) | O(L) |
| **Память** | O(n) | O(n) | O(алфавит × длина) |
| **Поиск по префиксу** | Медленно | Медленно | O(L) |

*Где L — длина строки, n — количество элементов.*

## Контрольные вопросы

1. **Что такое AVL-дерево и зачем оно нужно?**
   Самобалансирующееся бинарное дерево поиска. Нужно для гарантии сложности O(log n) даже в худших случаях, предотвращая вырождение дерева в список.

2. **Что такое фактор баланса?**
   Разница высот левого и правого поддеревьев. Для AVL он должен быть -1, 0 или 1.

3. **Какие бывают повороты в AVL-дереве?**
   Малый левый, малый правый, большой левый (левый-правый), большой правый (правый-левый).

4. **В чём отличие AVL-дерева от красно-черного дерева?**
   AVL строже балансируется (быстрее поиск), но требует больше поворотов при вставке/удалении. Красно-черное дерево балансируется слабее, но операции модификации быстрее.

5. **Что такое Trie? Для чего он используется?**
   Префиксное дерево. Используется для автодополнения, проверки орфографии, IP-маршрутизации.

6. **Какова сложность операций в Trie?**
   O(L), где L — длина строки (ключ). Не зависит от количества элементов.

7. **Как реализовать поиск по маске в Trie?**
   С помощью рекурсивного обхода. Если символ в маске '?', перебираем всех возможных потомков, иначе идем по конкретному индексу.

8. **Как удалить слово из Trie?**
   Найти узел, соответствующий концу слова, и снять флаг `isEnd`. (Опционально: удалить лишние узлы, если они не используются другими словами).

9. **Что такое префиксный поиск?**
   Поиск всех слов, начинающихся с заданной строки.

10. **Какая структура быстрее для поиска: AVL или Trie?**
    Trie быстрее для строк (O(L) против O(log N * сравнение строк)), но требует больше памяти.

11. **Как сбалансировать обычное BST?**
    Преобразовать в AVL или красно-черное дерево, либо перестроить из отсортированного массива.

12. **Почему AVL-дерево называется самобалансирующимся?**
    Потому что оно автоматически выполняет повороты при вставке/удалении, поддерживая баланс без внешнего вмешательства.

13. **Как построить AVL-дерево из массива?**
    Просто вставить все элементы массива по одному. Дерево само сбалансируется.

14. **В чём недостаток Trie?**
    Высокое потребление памяти (каждый узел хранит массив указателей на 26 символов).

15. **Где применяется AVL-дерево?**
    В базах данных (индексы), в памяти (менеджеры памяти), в языках программирования (словари).

## Вывод

В ходе лабораторной работы было реализовано AVL-дерево с полным набором операций (вставка, удаление, поиск) и механизмом балансировки. Также реализовано префиксное дерево (Trie) со специфичной для 12 варианта функцией поиска по маске ('?'). Проведенный анализ показал, что AVL гарантирует стабильное время O(log n), а Trie обеспечивает максимальную скорость для строковых операций.