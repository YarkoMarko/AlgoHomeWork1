//
// Created by yarem on 03.11.2025.
//

#pragma once

#ifndef ALGOS_HOMEWORK_NODE_H
#define ALGOS_HOMEWORK_NODE_H

#include "student.h"

class Node {
public:
    Student student;
    Node* left;
    Node* right;

    Node(const Student& student);
};


#endif //ALGOS_HOMEWORK_NODE_H