//
// Created by Эвелина on 18.11.2025.
//

#include "fibonacciwrapper.h"
#include "LRU.h"
#include "LFU.h"
#include <stdexcept>

FibonacciWrapper::FibonacciWrapper(int cacheType) {
    if (cacheType == 1) {
        cache = new LRUCache(10);
    } else if (cacheType == 2) {
        cache = new LFUCache(10);
    } else {
        throw std::invalid_argument("Error");
    }
}

FibonacciWrapper::~FibonacciWrapper() {
    delete cache;
}

int FibonacciWrapper::fib(int n) {
    if (n <= 1) return n;

    int result = (*cache)[n];
    if (result != -1) {
        return result;
    }

    int res = fib(n - 1) + fib(n - 2);
    cache->put(n, res);

    return res;
}

int FibonacciWrapper::calculate(int n) {
    if (n < 0) throw std::invalid_argument("Error");
    return fib(n);
}