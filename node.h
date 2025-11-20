//
// Created by Эвелина on 18.11.2025.
//

#ifndef LAB1OOP_NODE_H
#define LAB1OOP_NODE_H


struct Node {
    int key;
    int value;
    int cnt;
    Node *next;
    Node *prev;

    Node(int k, int v);
};



#endif //LAB1OOP_NODE_H