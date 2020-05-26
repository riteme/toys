#include <iostream>

struct A {
    int operator+(const A &) const {
        return 1;
    }

    constexpr bool valid() const {
        return true;
    }
};

struct B {
    int operator*(const B &) const {
        return 2;
    }

    constexpr bool valid() const {
        return false;
    }
};

template <typename T>
concept Aha = requires (T a, T b) {
    a.valid() ? a + b : a * b;
};

template <Aha T>
void foo(T a, T b) {
    if (a.valid())
        std::cout << a + b << std::endl;
    else
        std::cout << a * b << std::endl;
}

int main() {
    foo(A(), A());
    foo(B(), B());
    return 0;
}