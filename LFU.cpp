//
// Created by Эвелина on 18.11.2025.
//

#include "LFU.h"
#include <list>
#include <unordered_map>

LFUCache::LFUCache(int capacity) : capacity(capacity), minFreq(0) {}

LFUCache::~LFUCache() {
    for (auto& pair : cache) {
        delete pair.second;
    }
}

int LFUCache::get(int key) {
    if (capacity == 0 || cache.find(key) == cache.end()) {
        return -1;
    }

    Node* node = cache[key];

    freqMap[node->cnt].erase(iterMap[key]);

    if (freqMap[node->cnt].empty()) {
        freqMap.erase(node->cnt);
        if (minFreq == node->cnt) {
            minFreq++;
        }
    }

    node->cnt++;

    freqMap[node->cnt].push_front(node);
    iterMap[key] = freqMap[node->cnt].begin();

    return node->value;
}

void LFUCache::put(int key, int value) {
    if (capacity == 0) return;

    if (cache.find(key) != cache.end()) {
        cache[key]->value = value;
        get(key);  
        return;
    }

    if (cache.size() >= capacity) {
        Node* toDelete = freqMap[minFreq].back();
        freqMap[minFreq].pop_back();

        cache.erase(toDelete->key);
        iterMap.erase(toDelete->key);
        delete toDelete;

        if (freqMap[minFreq].empty()) {
            freqMap.erase(minFreq);
        }
    }

    Node* newNode = new Node(key, value);
    newNode->cnt = 1;

    cache[key] = newNode;
    freqMap[1].push_front(newNode);
    iterMap[key] = freqMap[1].begin();
    minFreq = 1;
}

int LFUCache::operator[](int key) {
    return get(key);
}
