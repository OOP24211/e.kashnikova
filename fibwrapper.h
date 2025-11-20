//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_FIBONACCIWRAPPER_H
#define LAB1OOP_FIBONACCIWRAPPER_H


#include "icache.h"

class FibonacciWrapper {
private:
    ICache* cache;
    int fib(int n);

public:
    FibonacciWrapper(int cacheType);
    ~FibonacciWrapper();
    int calculate(int n);
};


#endif //LAB1OOP_FIBONACCIWRAPPER_H