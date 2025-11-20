//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_LFU_H
#define LAB1OOP_LFU_H


#include "icache.h"
#include "node.h"
#include <unordered_map>

class LFUCache : public ICache {
private:
    int capacity;
    std::unordered_map<int, Node *> cacheMap;
    std::unordered_map<int, std::pair<Node *, Node *>> freqMap;
    int minFreq;

    void add(Node *node, int freq);
    void remove(Node *node);
    void updateFreq(Node *node);

public:
    LFUCache(int capacity);
    ~LFUCache() override;
    int get(int key) override;
    void put(int key, int value) override;
    int operator[](int key) override;
};


#endif //LAB1OOP_LFU_H