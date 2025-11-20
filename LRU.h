//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_LRU_H
#define LAB1OOP_LRU_H


#include "icache.h"
#include "node.h"
#include <unordered_map>

class LRUCache : public ICache {
private:
    int capacity;
    std::unordered_map<int, Node *> cacheMap;
    Node *head;
    Node *tail;

    void add(Node *node);
    void remove(Node *node);

public:
    LRUCache(int capacity);
    ~LRUCache() override;
    int get(int key) override;
    void put(int key, int value) override;
    int operator[](int key) override;
};

#endif //LAB1OOP_LRU_H