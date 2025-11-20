//
// Created by Эвелина on 18.11.2025.
//

#include "LFU.h"

#include <iostream>

LFUCache::LFUCache(int capacity) {
    this->capacity = capacity;
    minFreq = 0;
}

LFUCache::~LFUCache() {
    for (auto& pair : cacheMap) {
        delete pair.second;
    }
    for (auto& pair : freqMap) {
        delete pair.second.first;
        delete pair.second.second;
    }
}

void LFUCache::add(Node *node, int freq) {
    if (freqMap.find(freq) == freqMap.end()) {
        Node *head = new Node(-1, -1);
        Node *tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
        freqMap[freq] = {head, tail};
    }

    Node *head = freqMap[freq].first;
    Node *temp = head->next;
    node->next = temp;
    node->prev = head;
    head->next = node;
    temp->prev = node;
}

void LFUCache::remove(Node *node) {
    Node *delPrev = node->prev;
    Node *delNext = node->next;
    delPrev->next = delNext;
    delNext->prev = delPrev;
}

void LFUCache::updateFreq(Node *node) {
    int oldFreq = node->cnt;
    node->cnt++;

    remove(node);

    if (freqMap[oldFreq].first->next == freqMap[oldFreq].second) {
        delete freqMap[oldFreq].first;
        delete freqMap[oldFreq].second;
        freqMap.erase(oldFreq);
        if (minFreq == oldFreq) {
            minFreq++;
        }
    }
    add(node, node->cnt);
}

int LFUCache::get(int key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return -1;
    }

    Node *node = cacheMap[key];
    int res = node->value;
    updateFreq(node);
    return res;
}

void LFUCache::put(int key, int value) {
    if (capacity == 0) return;

    if (cacheMap.find(key) != cacheMap.end()) {
        Node *node = cacheMap[key];
        node->value = value;
        updateFreq(node);
    } else {
        if (cacheMap.size() == capacity) {
            Node *node = freqMap[minFreq].second->prev;
            cacheMap.erase(node->key);
            remove(node);

            if (freqMap[minFreq].first->next == freqMap[minFreq].second) {
                freqMap.erase(minFreq);
            }
            delete node;
        }

        Node *node = new Node(key, value);
        cacheMap[key] = node;
        minFreq = 1;
        add(node, 1);
    }
}

int LFUCache::operator[](int key) {
    return get(key);
}