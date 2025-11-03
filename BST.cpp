//
// Created by yarem on 03.11.2025.
//

#include <iostream>
#include "BST.h"
#include <algorithm>

StudentBST::StudentBST() {
    this->root = nullptr;
}

Node *StudentBST::insert(Node *node, Student &student) {
    if (node == nullptr) {
        return new Node(student);
    }

    if (student.get_rating() >= node->student.get_rating()) {
        node->left = insert(node->left, student);
    }

    else {
        node->right = insert(node->right, student);
    }

    return node;
}

void StudentBST::insert(Student &student) {
    this->root = insert(this->root, student);
}

void StudentBST::collectTopHundred(Node *node, std::vector<Student> &vec) {
    if (node == nullptr || vec.size() >= 100) {
        return;
    }

    collectTopHundred(node->left, vec);

    if (vec.size() < 100) {
        vec.push_back(node->student);
    }

    collectTopHundred(node->right, vec);
}

void StudentBST::top_hundred() {
    std::vector<Student> vec;

    collectTopHundred(this->root, vec);

    int size_ = vec.size();
    for (int i = 0; i < size_; i++) {
        std::cout << vec[i].get_email() << ": " << vec[i].get_rating() << std::endl;
    }
}

Node *StudentBST::remove(Node *node, float rating, std::string email) {
    if (node == nullptr) {
        return nullptr;
    }

    if (rating > node->student.get_rating()) {
        node->left = remove(node->left, rating, email);
    }

    else if (rating < node->student.get_rating()) {
        node->right = remove(node->right, rating, email);
    }

    else {
        if (email != node->student.get_email()) {
            node->left = remove(node->left, rating, email);
        }

        else {
            if (node->left == nullptr) {
                Node *temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == nullptr) {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            else {
                Node* temp = node->left;
                while (temp->right != nullptr) {
                    temp = temp->right;
                }

                node->student = temp->student;
                node->left = remove(node->left, temp->student.get_rating(), temp->student.get_email());
            }
        }
    }

    return node;
}

Node* StudentBST::findEmail(Node *node, std::string email) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->student.get_email() == email) {
        return node;
    }

    Node* left = findEmail(node->left, email);

    if (left != nullptr) {
        return left;
    }

    return findEmail(node->right, email);
}

void StudentBST::changeRatingByEmail(std::string email) {
    Node* node = findEmail(this->root, email);

    if (node == nullptr) {
        std::cout << "No student with such email: " << email << std::endl;
        return;
    }

    root = remove(root, node->student.get_rating(), email);

    float newRating;
    std::cout << "Enter new value of rating: ";
    std::cin >> newRating;

    node->student.set_rating(newRating);

    insert(node->student);
}

void StudentBST::getInfo(Node *node, std::unordered_map<std::string, float> &rating, std::unordered_map<std::string, float> &group) {
    if (node == nullptr) {
        return;
    }

    getInfo(node->left, rating, group);

    if (!rating.count(node->student.get_group())) {
        rating[node->student.get_group()] = node->student.get_rating();
        group[node->student.get_group()] = 1;
    }

    else {
        rating[node->student.get_group()] += node->student.get_rating();
        group[node->student.get_group()] += 1;
    }

    getInfo(node->right, rating, group);
}

void StudentBST::findBiggestAverageRating_() {
    std::unordered_map<std::string, float> rating;
    std::unordered_map<std::string, float> group;
    std::unordered_map<std::string, float> average;

    getInfo(this->root, rating, group);

    for (auto it = rating.begin(); it != rating.end(); ++it) {
        average[it->first] = rating[it->first] / group[it->first];
    }

    auto max_it_av = std::max_element(average.begin(), average.end(),
                                       [](const auto& p1, const auto& p2) {
                                           return p1.second < p2.second;
                                       });

    std::cout << "Group: " << max_it_av->first << ", " << max_it_av->second << std::endl;
}

void StudentBST::findBiggestAverageRating() {
    findBiggestAverageRating_();
}

int StudentBST::countNodes(Node *node) {
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int StudentBST::size() {
    return countNodes(this->root);
}

