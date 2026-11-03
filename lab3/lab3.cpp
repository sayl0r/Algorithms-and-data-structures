#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class Vector {
    std::shared_ptr<std::vector<T>> data_;

    // Отделение данных перед изменением
    void detach() {
        if (data_.use_count() > 1) data_ = std::make_shared<std::vector<T>>(*data_);
    }

public:
    // Правило пяти
    Vector() : data_(std::make_shared<std::vector<T>>()) {}
    Vector(const Vector& other) : data_(other.data_) {} // Ленивое копирование
    Vector(Vector&& other) noexcept : data_(std::move(other.data_)) {}
    Vector& operator=(const Vector& other) {
        if (this != &other) data_ = other.data_;
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) data_ = std::move(other.data_);
        return *this;
    }
    ~Vector() = default;

    // Методы
    void push_back(const T& val) { detach(); data_->push_back(val); }
    size_t size() const { return data_->size(); }

    // Итераторы
    typename std::vector<T>::iterator begin() { detach(); return data_->begin(); }
    typename std::vector<T>::iterator end() { detach(); return data_->end(); }
    typename std::vector<T>::const_iterator begin() const { return data_->begin(); }
    typename std::vector<T>::const_iterator end() const { return data_->end(); }
};

int main() {
    Vector<int> v1;
    v1.push_back(1);
    
    Vector<int> v2 = v1; // Копирование (COW)
    v2.push_back(2);     // Модификация (вызовет detach)

    std::cout << "v1: ";
    for (auto it = v1.begin(); it != v1.end(); ++it) std::cout << *it << " ";
    
    std::cout << "\nv2: ";
    for (int x : v2) std::cout << x << " ";
    
    return 0;
}