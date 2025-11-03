//
// Created by yarem on 03.11.2025.
//

#include "node.h"

Node::Node(const Student &student) {
    this->student = student;
    this->left = nullptr;
    this->right = nullptr;
}
