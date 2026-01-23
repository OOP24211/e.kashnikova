//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_ICACHE_H
#define LAB1OOP_ICACHE_H


class ICache {
public:
    virtual ~ICache() = default;
    virtual int get(int key) = 0;
    virtual void put(int key, int value) = 0;
    virtual int operator[](int key) = 0;
};


#endif //LAB1OOP_ICACHE_H