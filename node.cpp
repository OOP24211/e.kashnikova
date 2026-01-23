//
// Created by Эвелина on 18.11.2025.
//

#include "node.h"

Node::Node(int k, int v) {
    key = k;
    value = v;
    next = nullptr;
    prev = nullptr;
    cnt = 1;
}