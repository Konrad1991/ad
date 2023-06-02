#include <iostream>
#include <vector>
#include <functional>

template<typename T>
class Bla {

public:
    T i;
    T* i_ref;

    Bla(T& i_) : i(i_), i_ref(&i) {}

};

int main() {
    int a = 5;
    int b = 10;

    Bla<int> one(a);
    Bla<int> two(b);

    std::vector<Bla<int>> vec_bla{one, two};
    for(auto i: vec_bla) {
        std::cout << i.i << " " << i.i_ref << std::endl;
        std::cout << &a << " " << &b << std::endl;
    }

    std::vector<Bla<int>*> ptrs{&one, &two};
    std::vector<std::reference_wrapper<Bla<int>>> vec_bla_refs;
    for (auto* ptr : ptrs) {
        vec_bla_refs.emplace_back(std::ref(*ptr));
    }
    for(auto& ref : vec_bla_refs) {
        std::cout << ref.get().i << " " << ref.get().i_ref << std::endl;
    }


    std::vector<std::reference_wrapper<int>> vec_refs{a, b};
    for(auto& ref : vec_refs) {

        std::cout << ref.get() << std::endl;
    }


    return 0;
}
