//
// Created by yarem on 02.11.2025.
//

#pragma once

#ifndef ALGOS_HOMEWORK_STUDENT_H
#define ALGOS_HOMEWORK_STUDENT_H
#include <iostream>

class Student {
private:
    std::string m_name;
    std::string m_surname;
    std::string m_email;
    int m_birth_year;
    int m_birth_month;
    int m_birth_day;
    std::string m_group;
    float m_rating;
    std::string m_phone_number;
public:
    std::string get_name() const;
    std::string get_surname() const;
    std::string get_email() const;
    int get_birth_year() const;
    int get_birth_month() const;
    int get_birth_day() const;
    std::string get_group() const;
    float get_rating() const;
    std::string get_phone_number() const;

    void set_name(std::string name);
    void set_surname(std::string surname);
    void set_email(std::string email);
    void set_birth_year(int year);
    void set_birth_month(int month);
    void set_birth_day(int day);
    void set_group(std::string group);
    void set_rating(float rating);
    void set_phone_number(std::string phone_number);
};
#endif //ALGOS_HOMEWORK_STUDENT_H