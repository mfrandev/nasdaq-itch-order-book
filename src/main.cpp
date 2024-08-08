#include <iostream>

#include <fmt/format.h>

#include <messages/common.h>

int main() {
    for(int i = 0; i < 10; ++i) {
        fmt::println("This is a test: {}", i);
    }
    return 0;
}