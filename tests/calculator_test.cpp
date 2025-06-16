#include <cassert>
#include <iostream>
#include "../include/calculator.h"

void test_add() {
    Calculator calc;
    assert(calc.add(2, 3) == 5);
    assert(calc.add(-1, 1) == 0);
    assert(calc.add(0, 0) == 0);
}

void test_subtract() {
    Calculator calc;
    assert(calc.subtract(5, 3) == 2);
    assert(calc.subtract(1, 1) == 0);
    assert(calc.subtract(0, 0) == 0);
}

void test_multiply() {
    Calculator calc;
    assert(calc.multiply(2, 3) == 6);
    assert(calc.multiply(-2, 3) == -6);
    assert(calc.multiply(0, 5) == 0);
}

void test_divide() {
    Calculator calc;
    assert(calc.divide(6, 2) == 3.0f);
    assert(calc.divide(5, 2) == 2.5f);
    assert(calc.divide(0, 5) == 0.0f);
}

int main() {
    test_add();
    test_subtract();
    test_multiply();
    test_divide();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
} 