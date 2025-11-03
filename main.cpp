#include "student.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include "BST.h"

struct Result {
    std::string structure;
    int size;
    long long total_ops;
};

void vector_realisation() {
    std::vector<Student> students;
    std::fstream file;

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field;



    while (std::getline(file, field)) {
        std::stringstream ss(field);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        students.push_back(student);
    }

    std::cout << students.size() << std::endl;

    std::cout << students[0].get_name() << std::endl;

    file.close();
}



void merge(std::vector<Student>& vec, int left, int mid, int right) {
    int size_for_left = mid - left + 1;
    int size_for_right = right - mid;

    std::vector<Student> temp_l(size_for_left);
    std::vector<Student> temp_r(size_for_right);

    for (int i = 0; i < size_for_left; i++) {
        temp_l[i] = vec[left + i];
    }

    for (int i = 0; i < size_for_right; i++) {
        temp_r[i] = vec[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;

    while (i < size_for_left && j < size_for_right) {
        if (temp_l[i].get_rating() >= temp_r[j].get_rating()) {
            vec[k] = temp_l[i];
            i++;
        } else {
            vec[k] = temp_r[j];
            j++;
        }
        k++;
    }

    while (i < size_for_left) {
        vec[k] = temp_l[i];
        i++;
        k++;
    }

    while (j < size_for_right) {
        vec[k] = temp_r[j];
        j++;
        k++;
    }
}

void mergeSortVector(std::vector<Student>& students, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortVector(students, left, mid);
        mergeSortVector(students, mid + 1, right);

        merge(students, left, mid, right);
    }
}

void top_hundred(std::vector<Student>& vec) {
    int size_ = vec.size();

    mergeSortVector(vec, 0, size_ - 1);

    for (int i = 0; i < 100; i++) {
        std::cout << vec[i].get_name() << " " << vec[i].get_surname() << ": "<< vec[i].get_rating() << std::endl;
    }

}

void top_hundred_for_map(std::unordered_map<std::string, Student>& map) {
    int size_ = map.size();
    std::vector<Student> temp(size_);
    for (auto& [key, value] : map) {
        temp.push_back(value);
    }

    top_hundred(temp);
}

void changeRatingByEmail(std::vector<Student>& vec, std::string email) {
    int size_ = vec.size();

    for (int i = 0; i < size_; i++) {
        if (vec[i].get_email() == email) {
            std::cout << vec[i].get_name() << " " << vec[i].get_surname() << ", " << vec[i].get_email() << ": "<< vec[i].get_rating() << std::endl;

            float newRating;
            std::cout << "Enter new value of rating: ";
            std::cin >> newRating;
            vec[i].set_rating(newRating);

            std::cout << vec[i].get_name() << " " << vec[i].get_surname() << ", " << vec[i].get_email() << ": "<< vec[i].get_rating() << std::endl;

            return;
        }
    }

    std::cout << "No student with such email: " << email << std::endl;
}

void changeRatingByEmailMap(std::unordered_map<std::string, Student>& map, std::string email) {
    if (!map.count(email)) {
        std::cout << "No student with such email: " << email << std::endl;
        return;
    }

    float newRating;
    std::cout << email << ": " << map[email].get_rating() << std::endl;

    std::cout << "Enter new rating: ";
    std::cin >> newRating;

    map[email].set_rating(newRating);

    std::cout << email << ": " << map[email].get_rating() << std::endl;
}

void findBiggestAverageRating(std::vector<Student>& vec) {
    int size_ = vec.size();
    std::unordered_map<std::string, float> rating;
    std::unordered_map<std::string, float> group;
    std::unordered_map<std::string, float> average;

    for (int i = 0; i < size_; i++) {
        if (!rating.count(vec[i].get_group())) {
            rating[vec[i].get_group()] = vec[i].get_rating();
            group[vec[i].get_group()] = 1;
        }

        else {
            rating[vec[i].get_group()] += vec[i].get_rating();
            group[vec[i].get_group()] += 1;
        }
    }



    for (auto it = rating.begin(); it != rating.end(); ++it) {
        average[it->first] = rating[it->first] / group[it->first];
    }

    auto max_it_av = std::max_element(average.begin(), average.end(),
                                       [](const auto& p1, const auto& p2) {
                                           return p1.second < p2.second;
                                       });

    std::cout << "Group: " << max_it_av->first << ", " << max_it_av->second << std::endl;
}

void findBiggestAverageRatingMap(std::unordered_map<std::string, Student>& map) {
    int size_ = map.size();
    std::vector<Student> temp(size_);
    for (auto& [key, value] : map) {
        temp.push_back(value);
    }

    findBiggestAverageRating(temp);

}

std::vector<Student> load_students(const std::string &filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return students;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field;
    while (std::getline(file, field)) {
        std::stringstream ss(field);
        std::string line;
        std::vector<std::string> tokens;
        while (std::getline(ss, line, ';'))
            tokens.push_back(line);
        if (tokens.size() < 9) continue;

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        students.push_back(student);
    }

    return students;
}

void run_experiment_deficit(std::vector<Student>& students, int A, int B, int C, int duration_seconds = 10) {
    using namespace std::chrono;
    const int totalW = A + B + C;
    const double p1 = double(A) / totalW;
    const double p2 = double(B) / totalW;
    const double p3 = double(C) / totalW;

    long long cnt1 = 0, cnt2 = 0, cnt3 = 0;
    long long total_cnt = 0;

    auto start = high_resolution_clock::now();

    while (duration_cast<std::chrono::seconds>(high_resolution_clock::now() - start).count() < duration_seconds) {
        double exp1 = (total_cnt + 1) * p1;
        double exp2 = (total_cnt + 1) * p2;
        double exp3 = (total_cnt + 1) * p3;

        double deficit1 = exp1 - cnt1;
        double deficit2 = exp2 - cnt2;
        double deficit3 = exp3 - cnt3;

        if (deficit1 >= deficit2 && deficit1 >= deficit3) {
            top_hundred(students);
            cnt1++;
        }
        else if (deficit2 >= deficit1 && deficit2 >= deficit3) {
            changeRatingByEmail(students, "taras.shevchuk3@student.org");
            cnt2++;
        }
        else {
            findBiggestAverageRating(students);
            cnt3++;
        }

        total_cnt++;
    }

    std::cout << "Op1: " << cnt1 << "\n";
    std::cout << "Op2: " << cnt2 << "\n";
    std::cout << "Op3: " << cnt3 << "\n";
    std::cout << "Total: " << total_cnt << "\n";
}

void run_experiment_deficit_map(std::unordered_map<std::string, Student> students, int A, int B, int C, int duration_seconds = 10) {
    using namespace std::chrono;
    const int totalW = A + B + C;
    const double p1 = double(A) / totalW;
    const double p2 = double(B) / totalW;
    const double p3 = double(C) / totalW;

    long long cnt1 = 0, cnt2 = 0, cnt3 = 0;
    long long total_cnt = 0;

    auto start = high_resolution_clock::now();

    while (duration_cast<std::chrono::seconds>(high_resolution_clock::now() - start).count() < duration_seconds) {
        double exp1 = (total_cnt + 1) * p1;
        double exp2 = (total_cnt + 1) * p2;
        double exp3 = (total_cnt + 1) * p3;

        double deficit1 = exp1 - cnt1;
        double deficit2 = exp2 - cnt2;
        double deficit3 = exp3 - cnt3;

        if (deficit1 >= deficit2 && deficit1 >= deficit3) {
            top_hundred_for_map(students);
            cnt1++;
        }
        else if (deficit2 >= deficit1 && deficit2 >= deficit3) {
            changeRatingByEmailMap(students, "taras.shevchuk3@student.org");
            cnt2++;
        }
        else {
            findBiggestAverageRatingMap(students);
            cnt3++;
        }

        total_cnt++;
    }

    std::cout << "Op1: " << cnt1 << "\n";
    std::cout << "Op2: " << cnt2 << "\n";
    std::cout << "Op3: " << cnt3 << "\n";
    std::cout << "Total: " << total_cnt << "\n";
}

void run_experiment_deficit_bst(StudentBST student_bst, int A, int B, int C, int duration_seconds = 10) {
    using namespace std::chrono;
    const int totalW = A + B + C;
    const double p1 = double(A) / totalW;
    const double p2 = double(B) / totalW;
    const double p3 = double(C) / totalW;

    long long cnt1 = 0, cnt2 = 0, cnt3 = 0;
    long long total_cnt = 0;

    auto start = high_resolution_clock::now();

    while (duration_cast<std::chrono::seconds>(high_resolution_clock::now() - start).count() < duration_seconds) {
        double exp1 = (total_cnt + 1) * p1;
        double exp2 = (total_cnt + 1) * p2;
        double exp3 = (total_cnt + 1) * p3;

        double deficit1 = exp1 - cnt1;
        double deficit2 = exp2 - cnt2;
        double deficit3 = exp3 - cnt3;

        if (deficit1 >= deficit2 && deficit1 >= deficit3) {
            student_bst.top_hundred();
            cnt1++;
        }
        else if (deficit2 >= deficit1 && deficit2 >= deficit3) {
            student_bst.changeRatingByEmail("taras.shevchuk3@student.org");
            cnt2++;
        }
        else {
            student_bst.findBiggestAverageRating();
            cnt3++;
        }

        total_cnt++;
    }

    std::cout << "Op1: " << cnt1 << "\n";
    std::cout << "Op2: " << cnt2 << "\n";
    std::cout << "Op3: " << cnt3 << "\n";
    std::cout << "Total: " << total_cnt << "\n";
}

void sortByPhone_std(std::vector<Student>& students) {
    auto start = std::chrono::high_resolution_clock::now();

    std::sort(students.begin(), students.end(),
              [](Student& a, Student& b) {
                  return a.get_phone_number() < b.get_phone_number();
              });

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::sort time: "
              << std::chrono::duration<double>(end - start).count()
              << " s\n";
}

uint64_t phoneToNumber(const std::string& phone) {
    uint64_t num = 0;
    for (char c : phone) {
        if (std::isdigit(c)) {
            num = num * 10 + (c - '0');
        }
    }
    return num;
}

void radixSortPhones(std::vector<Student>& students) {
    auto start = std::chrono::high_resolution_clock::now();

    const int base = 256;  // працюємо з байтами
    const int passes = sizeof(uint64_t); // 8 проходів
    std::vector<Student> buffer(students.size());
    std::vector<int> count(base);

    std::vector<uint64_t> keys(students.size());
    for (size_t i = 0; i < students.size(); ++i)
        keys[i] = phoneToNumber(students[i].get_phone_number());

    for (int pass = 0; pass < passes; ++pass) {
        std::fill(count.begin(), count.end(), 0);

        int shift = pass * 8;
        for (uint64_t k : keys)
            count[(k >> shift) & 0xFF]++;

        for (int i = 1; i < base; ++i)
            count[i] += count[i - 1];

        for (int i = (int)students.size() - 1; i >= 0; --i) {
            int idx = (keys[i] >> shift) & 0xFF;
            buffer[--count[idx]] = students[i];
        }

        students.swap(buffer);

        for (size_t i = 0; i < students.size(); ++i)
            keys[i] = phoneToNumber(students[i].get_phone_number());
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Optimized Radix Sort time: "
              << std::chrono::duration<double>(end - start).count()
              << " s\n";
}

int main() {

    std::vector<Student> students;
    std::vector<Student> vec_s_std;
    std::vector<Student> vec_s_custom;
    std::unordered_map<std::string, Student> students_map;
    StudentBST student_bst;

    std::fstream file;

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field;



    while (std::getline(file, field)) {
        std::stringstream ss(field);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        students.push_back(student);
    }

    std::cout << students.size() << std::endl;

    file.close();

    run_experiment_deficit(students, 10, 10, 20);

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field_;

    while (std::getline(file, field_)) {
        std::stringstream ss(field_);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        students_map[tokens[2]] = student;
    }

    std::cout << students_map.size() << std::endl;

    file.close();

    run_experiment_deficit_map(students_map, 10, 10, 20);

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field_bst;

    while (std::getline(file, field_bst)) {
        std::stringstream ss(field_bst);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        student_bst.insert(student);
    }

    file.close();

    run_experiment_deficit_bst(student_bst, 10, 10, 20);

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field_s;



    while (std::getline(file, field_s)) {
        std::stringstream ss(field_s);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        vec_s_std.push_back(student);
    }

    std::cout << vec_s_std.size() << std::endl;

    file.close();

    sortByPhone_std(vec_s_std);

    file.open("C:/Users/yarem/CLionProjects/Algos_homework/students.csv", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string field_s_c;

    while (std::getline(file, field_s_c)) {
        std::stringstream ss(field_s_c);
        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        Student student;
        student.set_name(tokens[0]);
        student.set_surname(tokens[1]);
        student.set_email(tokens[2]);
        student.set_birth_year(std::stoi(tokens[3]));
        student.set_birth_month(std::stoi(tokens[4]));
        student.set_birth_day(std::stoi(tokens[5]));
        student.set_group(tokens[6]);
        student.set_rating(std::stof(tokens[7]));
        student.set_phone_number(tokens[8]);

        vec_s_custom.push_back(student);
    }

    std::cout << vec_s_custom.size() << std::endl;

    file.close();

    radixSortPhones(vec_s_custom);

    return 0;
}
