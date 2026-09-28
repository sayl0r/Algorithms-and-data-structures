#include <iostream>
#include <string>

// ==================== STORE — SINGLETON ====================
class Store {
    std::string name;
    static Store* instance;          // статическое поле-одиночка
    Store(const std::string& n) : name(n) {}   // закрытый конструктор

public:
    static Store* getInstance() {
        if (!instance) instance = new Store("TechnoMart");
        return instance;
    }
    std::string getName() const { return name; }
};
Store* Store::instance = nullptr;

// ==================== PURCHASE — КОМПОЗИЦИЯ ====================
class Purchase {
    std::string product;
    Store& store;                    // ссылка — без магазина не создать
    static int count;                // счётчик объектов

public:
    Purchase(const std::string& p, Store& s) : product(p), store(s) {
        ++count;
        std::cout << "Purchase #" << count << ": " << product
                  << " в " << store.getName() << "\n";
    }
    ~Purchase() { --count; }
    static int getCount() { return count; }
};
int Purchase::count = 0;

// ==================== CUSTOMER — АГРЕГАЦИЯ ====================
class Customer {
    std::string name;
    Store* store = nullptr;          // указатель — может быть nullptr

public:
    Customer(const std::string& n) : name(n) {}
    void setStore(Store* s) { store = s; }

    Purchase* buy(const std::string& p) {
        if (!store) {
            std::cout << name << " не может купить: нет магазина\n";
            return nullptr;
        }
        return new Purchase(p, *store);
    }
};

// ==================== ДЕМОНСТРАЦИЯ ====================
int main() {
    // Singleton: два вызова — один объект
    Store* s1 = Store::getInstance();
    Store* s2 = Store::getInstance();
    std::cout << "Singleton: " << (s1 == s2 ? "один объект" : "разные") << "\n";

    // Агрегация: покупатель без магазина
    Customer c("Иван");
    c.buy("Телефон");                // не сработает

    // Привязка магазина и покупки
    c.setStore(s1);
    Purchase* p1 = c.buy("Ноутбук");
    Purchase* p2 = c.buy("Мышь");

    // Счётчик
    std::cout << "Всего покупок: " << Purchase::getCount() << "\n";

    delete p1;
    delete p2;
    delete s1;
    return 0;
}