//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_LFU_H
#define LAB1OOP_LFU_H

#include "icache.h"
#include "node.h"
#include <unordered_map>
#include <list>

class LFUCache : public ICache {
private:
    int capacity;
    int minFreq;
    std::unordered_map<int, Node*> cache;  
    std::unordered_map<int, std::list<Node*>> freqMap; 
    std::unordered_map<int, std::list<Node*>::iterator> iterMap; 

public:
    LFUCache(int capacity);
    ~LFUCache() override;
    int get(int key) override;
    void put(int key, int value) override;
    int operator[](int key) override;
};


#endif //LAB1OOP_LFU_H
