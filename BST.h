//
// Created by yarem on 03.11.2025.
//

#pragma once

#ifndef ALGOS_HOMEWORK_BST_H
#define ALGOS_HOMEWORK_BST_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "node.h"

class StudentBST {
private:
    Node* root;
    Node* insert(Node* node, Student& student);
    void collectTopHundred(Node* node, std::vector<Student>& vec);
    Node* remove(Node* node, float rating,std::string email);
    Node* findEmail(Node* node, std::string email);
    void findBiggestAverageRating_();
    void getInfo(Node* node, std::unordered_map<std::string, float>& rating, std::unordered_map<std::string, float>& group);
    int countNodes(Node* node);
public:
    StudentBST();

    void insert(Student& student);
    void top_hundred();
    void changeRatingByEmail(std::string email);
    void findBiggestAverageRating();
    int size();
};

#endif //ALGOS_HOMEWORK_BST_H