//
// Created by Jollu Emil on 11/8/23.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

using my_long = long long;

template <typename T>
class CTestArray {
private:
    T *m_ptr;
    my_long m_capacity;  // all reserve memory
    my_long m_length;    //

public:
    explicit CTestArray(long long n = 0) : m_ptr(new T[n]), m_capacity(n), m_length(0) {}

    ~CTestArray() {
        Clear();
    }

public:  // Methods
    void Clear() {
        delete[] m_ptr;
        m_ptr = new T[m_capacity];
        m_length = 0;
    }

    void Sort() {
        std::sort(m_ptr, m_ptr + m_length);
    }

    my_long Push_back(T data) {
        if (m_length == m_capacity) {
            T *old = m_ptr;
            m_ptr = new T[m_capacity = m_capacity * 2];
            std::copy(old, old + m_length, m_ptr);
            delete[] old;
        }
        m_ptr[m_length++] = data;
        return m_length;
    }

    T Pop_back() {
        --m_length;
        return m_ptr[m_length];
    }

    void Delete(long long index) {
        if (index < 0 || index >= m_length) {
            std::cout << "Error: Array index out of bound";
            exit(0);
        }

        for (long long i = index; i < m_length - 1; ++i) {
            m_ptr[i] = m_ptr[i + 1];
        }

        --m_length;
    }

    void Insert(long long index, T value) {
        if (index < 0 || index > m_length) {
            std::cout << "Error: Array index out of bound";
            exit(0);
        }

        if (m_length == m_capacity) {
            T *old = m_ptr;
            m_ptr = new T[m_capacity = m_capacity * 2];
            std::copy(old, old + m_length, m_ptr);
            delete[] old;
        }

        for (long long i = m_length; i > index; --i) {
            m_ptr[i] = m_ptr[i - 1];
        }

        m_ptr[index] = value;
        ++m_length;
    }

    my_long Size() const {
        return m_length;
    }

    [[nodiscard]] bool Empty() const {
        return m_length == 0;
    }

public:  // Operators
    T &operator[](my_long index) {
        if (index >= m_length) {
            std::cout << "Error: Array index out of  bound";
            exit(0);
        }
        return *(m_ptr + index);
    }

public:  // Iterator;
    class Iterator {
        T *ptr_;

    public:
        explicit Iterator() : ptr_(nullptr) {}

        explicit Iterator(T *p) : ptr_(p) {}

        bool operator==(const Iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }

        bool operator!=(const Iterator &rhs) const {
            return !(*this == rhs);
        }

        T operator*() const {
            return *ptr_;
        }

        Iterator &operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            ++*this;
            return temp;
        }
    };

public:  // Decl. Iterators
    [[nodiscard]] Iterator begin() const {
        return Iterator(m_ptr);
    }

    [[nodiscard]] Iterator end() const {
        return Iterator(m_ptr + m_length);
    }
};

int main() {
    using namespace std::string_literals;
    CTestArray<int> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    // Test Push_back and Size
    for (auto i = 0; i < 100; i++)
        vec.Push_back(dis(gen));
    assert(vec.Size() == 100);
    std::cout << "Test Push_back() is Done!\n";

    // Test operator[]

    vec.Push_back(73);
    assert(vec[vec.Size() - 1] == 73);
    vec.Push_back(1);
    assert(vec[vec.Size() - 1] == 1);
    vec.Push_back(73);
    assert(vec[vec.Size() - 1] == 73);
    vec.Push_back(101);
    assert(vec[vec.Size() - 1] == 101);
    vec.Push_back(21);
    assert(vec[vec.Size() - 1] == 21);
    std::cout << "Tes operator[] is Done!\n";

    //     Test Insert
    for (auto i = 0; i < 5; ++i)
        vec.Insert(i, 5);
    assert(vec.Size() == 110);
    std::cout << "Test Insert is Done! \n";

    //    // Test Delete
    for (auto i = 0; i < 5; ++i)
        vec.Delete(i);
    assert(vec.Size() == 105);
    std::cout << "Test Delete is Done!\n";
    //
    //    // Test Pop_back
    assert(vec.Pop_back() == 21);

    assert(vec.Pop_back() == 101);

    assert(vec.Pop_back() == 73);

    assert(vec.Pop_back() == 1);

    assert(vec.Pop_back() == 73);
    assert(vec.Size() == 100);
    std::cout << "Test Pop_back is Done!\n";
    //
    //    // Test Sort
    vec.Sort();
    std::cout << "Array after sort: "s;
    for (auto i = 0; i < 10; i++) {
        std::cout << vec[i] << ' ';
    }
    std::cout << std::endl;

    //    // Test Iterator
    std::cout << "Array using iterator: "s;
    for (int it : vec) {
        std::cout << it << ' ';
    }
    std::cout << std::endl;

    vec.Clear();
    assert(vec.Empty() == true);
    assert(vec.Size() == 0);
}