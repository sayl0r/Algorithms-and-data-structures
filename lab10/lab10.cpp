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