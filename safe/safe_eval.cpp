//
// Created by zaqwes on 07.09.2022.
//

//#include <typeinfo>

#include <stdlib.h>

[[nodiscard]] void* rtti() noexcept {
    //    auto a =typeid(int);
    // throw int();
    int* ptr = new int();
    (void)ptr;

    void* p = malloc(10);
    (void)p;
    return nullptr;
}

void g() { (void)rtti(); }