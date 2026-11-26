#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// КЛАСС ГРАФА (Ориентированный, Матрица смежности)
class Graph {
    int V; // Количество вершин
    vector<vector<int>> matrix; // Матрица смежности

public:
    Graph(int vertices) : V(vertices), matrix(vertices, vector<int>(vertices, 0)) {}

    // Добавление ориентированного ребра
    void addEdge(int u, int v) {
        matrix[u][v] = 1;
    }

    // Добавление петли (ребро из вершины в саму себя)
    void addLoop(int u) {
        matrix[u][u] = 1;
    }

    // ОБХОД В ГЛУБИНУ (DFS)
    void DFS(int start) {
        vector<bool> visited(V, false);
        cout << "DFS from " << start << ": ";
        DFS_util(start, visited);
        cout << "\n";
    }

    // Рекурсивная вспомогательная функция для DFS
    void DFS_util(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";
        for (int u = 0; u < V; u++) {
            if (matrix[v][u] == 1 && !visited[u]) {
                DFS_util(u, visited);
            }
        }
    }

    // ОБХОД В ШИРИНУ (BFS)
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << "BFS from " << start << ": ";
        
        while (!q.empty()) {
            int v = q.front(); q.pop();
            cout << v << " ";
            for (int u = 0; u < V; u++) {
                if (matrix[v][u] == 1 && !visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }
        cout << "\n";
    }

    // ПОИСК КОМПОНЕНТ СВЯЗНОСТИ
    void printComponents() {
        vector<bool> visited(V, false);
        int compCount = 0;
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                compCount++;
                cout << "Component " << compCount << ": ";
                DFS_util(i, visited);
                cout << "\n";
            }
        }
    }

    // КРАТЧАЙШИЙ ПУТЬ (BFS)
    vector<int> shortestPath(int start, int end) {
        vector<int> dist(V, -1);
        vector<int> prev(V, -1);
        queue<int> q;

        dist[start] = 0;
        q.push(start);

        // BFS для поиска кратчайшего расстояния
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (v == end) break;

            for (int u = 0; u < V; u++) {
                if (matrix[v][u] == 1 && dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.push(u);
                }
            }
        }

        vector<int> path;
        if (dist[end] == -1) return path; // Пути нет

        // Восстановление пути с конца в начало
        for (int v = end; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void printShortestPath(int start, int end) {
        vector<int> path = shortestPath(start, end);
        if (path.empty()) {
            cout << "No path from " << start << " to " << end << "\n";
            return;
        }
        cout << "Shortest path " << start << " -> " << end << ": ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << " (length: " << path.size() - 1 << ")\n";
    }

    // ДИАМЕТР ГРАФА (BFS дважды)
    int getDiameter() {
        if (V == 0) return 0;
        
        // 1-й BFS: находим самую удаленную вершину (farthestNode) от вершины 0
        vector<int> dist1(V, -1);
        queue<int> q;
        dist1[0] = 0;
        q.push(0);
        int farthestNode = 0;
        
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (dist1[v] > dist1[farthestNode]) farthestNode = v;
            
            for (int u = 0; u < V; u++) {
                if (matrix[v][u] == 1 && dist1[u] == -1) {
                    dist1[u] = dist1[v] + 1;
                    q.push(u);
                }
            }
        }
        
        // 2-й BFS: от farthestNode находим максимальное расстояние до других вершин
        vector<int> dist2(V, -1);
        dist2[farthestNode] = 0;
        q.push(farthestNode);
        int diameter = 0;
        
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (dist2[v] > diameter) diameter = dist2[v];
            
            for (int u = 0; u < V; u++) {
                if (matrix[v][u] == 1 && dist2[u] == -1) {
                    dist2[u] = dist2[v] + 1;
                    q.push(u);
                }
            }
        }
        
        return diameter;
    }
};

int main() {
    cout << "--- Part 1 & 2: Graph, DFS, BFS (Variant 12: Directed with Loops) ---\n";
    Graph g(6);
    
    // Добавляем ребра (ориентированные)
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(1, 2);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);
    
    // Добавляем петли (специфика 12 варианта)
    g.addLoop(0);
    g.addLoop(3);

    g.DFS(0);
    g.BFS(0);

    cout << "\n--- Part 3: Components & Shortest Path ---\n";
    g.printComponents();
    g.printShortestPath(0, 5);

    cout << "\n--- Part 2 (Variant 12): Graph Diameter ---\n";
    cout << "Graph diameter: " << g.getDiameter() << "\n";

    return 0;
}