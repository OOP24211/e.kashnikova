//
// Created by Эвелина on 18.11.2025.
//

#include "LRU.h"
#include <iostream>

LRUCache::LRUCache(int capacity) {
    this->capacity = capacity;
    head = new Node(-1, -1);
    tail = new Node(-1, -1);
    head->next = tail;
    tail->prev = head;
}

LRUCache::~LRUCache() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LRUCache::add(Node *node) {
    Node *nextNode = head->next;
    head->next = node;
    node->prev = head;
    node->next = nextNode;
    nextNode->prev = node;
}

void LRUCache::remove(Node *node) {
    Node *prevNode = node->prev;
    Node *nextNode = node->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
}

int LRUCache::get(int key) {
    if (cacheMap.find(key) == cacheMap.end())
        return -1;

    Node *node = cacheMap[key];
    remove(node);
    add(node);
    return node->value;
}

void LRUCache::put(int key, int value) {
    if (cacheMap.find(key) != cacheMap.end()) {
        Node *oldNode = cacheMap[key];
        remove(oldNode);
        delete oldNode;
        cacheMap.erase(key);
    }

    Node *node = new Node(key, value);
    cacheMap[key] = node;
    add(node);

    if (cacheMap.size() > capacity) {
        Node *delNode = tail->prev;
        remove(delNode);
        cacheMap.erase(delNode->key);
        delete delNode;
    }
}

int LRUCache::operator[](int key) {
    return get(key);
}