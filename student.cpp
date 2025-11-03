//
// Created by yarem on 02.11.2025.
//

#include "student.h"

std::string Student::get_name() {
    return this->m_name;
}

std::string Student::get_surname() {
    return this->m_surname;
}

std::string Student::get_email() {
    return this->m_email;
}

int Student::get_birth_year() {
    return this->m_birth_year;
}

int Student::get_birth_month() {
    return this->m_birth_month;
}

int Student::get_birth_day() {
    return this->m_birth_day;
}

std::string Student::get_group() {
    return this->m_group;
}

float Student::get_rating() {
    return this->m_rating;
}

std::string Student::get_phone_number() {
    return this->m_phone_number;
}

void Student::set_name(std::string name) {
    this->m_name = name;
}

void Student::set_surname(std::string surname) {
    this->m_surname = surname;
}

void Student::set_email(std::string email) {
    this->m_email = email;
}

void Student::set_birth_year(int year) {
    this->m_birth_year = year;
}

void Student::set_birth_month(int month) {
    this->m_birth_month = month;
}

void Student::set_birth_day(int day) {
    this->m_birth_day = day;
}

void Student::set_group(std::string group) {
    this->m_group = group;
}

void Student::set_rating(float rating) {
    this->m_rating = rating;
}

void Student::set_phone_number(std::string phone_number) {
    this->m_phone_number = phone_number;
}