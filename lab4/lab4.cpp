#include <iostream>


// ОДНОСВЯЗНЫЙ СПИСОК (SinglyLinkedList)
class SinglyLinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };
    Node* head;
    Node* tail;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr) {}
    
    ~SinglyLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void push_back(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void print() {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "nullptr\n";
    }
};

// ДВУСВЯЗНЫЙ ЦИКЛИЧЕСКИЙ СПИСОК (DoublyLinkedList)
class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Node* prev;
        Node(int val) : data(val), next(nullptr), prev(nullptr) {}
    };
    Node* head;

public:
    DoublyLinkedList() : head(nullptr) {}
    
    ~DoublyLinkedList() {
        if (!head) return;
        Node* current = head;
        do {
            Node* next = current->next;
            delete current;
            current = next;
        } while (current != head);
    }

    // В кольцевом списке push_back добавляет элемент перед головой
    void push_back(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            Node* tail = head->prev; // Последний элемент
            newNode->next = head;
            newNode->prev = tail;
            tail->next = newNode;
            head->prev = newNode;
        }
    }

    void print() {
        if (!head) {
            std::cout << "List is empty\n";
            return;
        }
        Node* current = head;
        do {
            std::cout << current->data << " <-> ";
            current = current->next;
        } while (current != head);
        std::cout << "(return to head: " << head->data << ")\n";
    }
};

int main() {
    std::cout << "Singly Linked List" << std::endl;
    SinglyLinkedList singleList;
    singleList.push_back(10);
    singleList.push_back(20);
    singleList.push_back(30);
    singleList.print();

    std::cout << "\nDoubly Circular Linked List" << std::endl;
    DoublyLinkedList doubleList;
    doubleList.push_back(1);
    doubleList.push_back(2);
    doubleList.push_back(3);
    doubleList.print();

    return 0;
}