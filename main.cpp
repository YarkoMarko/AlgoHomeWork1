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
#include <random>
#include <windows.h>
#include <psapi.h>

size_t getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize; // у байтах
}

struct Result {
    std::string structure;
    int size;
    long long total_ops;
};

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
        std::cout << vec[i].get_email() << ": "<< vec[i].get_rating() << std::endl;
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
            std::cout << vec[i].get_email() << ": "<< vec[i].get_rating() << std::endl;

            float newRating;
            std::cout << "Enter new value of rating: ";
            std::cin >> newRating;
            vec[i].set_rating(newRating);

            std::cout << vec[i].get_email() << ": "<< vec[i].get_rating() << std::endl;

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
        while (std::getline(ss, line, ','))
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

Result run_experiment_deficit(std::vector<Student>& students, int A, int B, int C, int duration_seconds = 10) {
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

    std::cout << "[Vector] Op1=" << cnt1 << " Op2=" << cnt2 << " Op3=" << cnt3 << " Total=" << total_cnt << "\n";
    return {"Vector", (int)students.size(), total_cnt};
}

Result run_experiment_deficit_map(std::unordered_map<std::string, Student> students, int A, int B, int C, int duration_seconds = 10) {
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

    std::cout << "[Map] Op1=" << cnt1 << " Op2=" << cnt2 << " Op3=" << cnt3 << " Total=" << total_cnt << "\n";
    return {"Hash table", (int)students.size(), total_cnt};
}

Result run_experiment_deficit_bst(StudentBST student_bst, int A, int B, int C, int duration_seconds = 10) {
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

    std::cout << "[BST] Op1=" << cnt1 << " Op2=" << cnt2 << " Op3=" << cnt3 << " Total=" << total_cnt << "\n";
    return {"TreeMap", student_bst.size(), total_cnt};
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

void measure_memory_usage(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        return;
    }

    std::string header;
    std::getline(file, header);

    std::vector<Student> students_all;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ';')) tokens.push_back(token);

        Student s;
        s.set_name(tokens[0]);
        s.set_surname(tokens[1]);
        s.set_email(tokens[2]);
        s.set_birth_year(std::stoi(tokens[3]));
        s.set_birth_month(std::stoi(tokens[4]));
        s.set_birth_day(std::stoi(tokens[5]));
        s.set_group(tokens[6]);
        s.set_rating(std::stof(tokens[7]));
        s.set_phone_number(tokens[8]);

        students_all.push_back(s);
    }

    file.close();

    std::ofstream memcsv("C:/Users/yarem/CLionProjects/AlgoHomeWork/memory_usage.csv");
    memcsv << "structure,size,memory_bytes\n";

    std::vector<int> sizes = {100, 1000, 10000, 100000};

    for (int size : sizes) {
        if (students_all.size() < size) break;

        std::vector<Student> subset(students_all.begin(), students_all.begin() + size);

        size_t mem_before = getCurrentMemoryUsage();
        std::vector<Student> vec = subset;
        size_t mem_after = getCurrentMemoryUsage();
        memcsv << "Vector," << size << "," << (mem_after - mem_before) << "\n";

        mem_before = getCurrentMemoryUsage();
        std::unordered_map<std::string, Student> map;
        for (auto& s : subset)
            map[s.get_email()] = s;
        mem_after = getCurrentMemoryUsage();
        memcsv << "Hash table," << size << "," << (mem_after - mem_before) << "\n";

        mem_before = getCurrentMemoryUsage();
        StudentBST bst;
        for (auto& s : subset)
            bst.insert(s);
        mem_after = getCurrentMemoryUsage();
        memcsv << "TreeMap," << size << "," << (mem_after - mem_before) << "\n";
    }

    memcsv.close();
}

void save_students_to_csv(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }


    file << "m_name;m_surname;m_email;m_birth_year;m_birth_month;m_birth_day;"
            "m_group;m_rating;m_phone_number\n";

    for (const Student& s : students) {
        file << s.get_name() << ";"
             << s.get_surname() << ";"
             << s.get_email() << ";"
             << s.get_birth_year() << ";"
             << s.get_birth_month() << ";"
             << s.get_birth_day() << ";"
             << s.get_group() << ";"
             << s.get_rating() << ";"
             << s.get_phone_number() << "\n";
    }

    file.close();
    std::cout << "Saved " << students.size() << " students to " << filename << std::endl;
}

int main() {
    std::vector<int> sizes = {100, 1000, 10000, 100000};
    std::vector<Result> results;

    std::vector<Student> all_students = load_students("C:/Users/yarem/CLionProjects/AlgoHomeWork/students.csv");
    std::vector<Student> vec_s7_std = load_students("C:/Users/yarem/CLionProjects/AlgoHomeWork/students.csv");
    std::vector<Student> vec_s7_custom = load_students("C:/Users/yarem/CLionProjects/AlgoHomeWork/students.csv");

    std::cout << "Total loaded: " << all_students.size() << std::endl;

    for (int N : sizes) {
        std::cout << "\n===== Running experiment for N = " << N << " =====\n";
        if (N > (int)all_students.size()) {
            std::cerr << "Dataset too small for " << N << " students!\n";
            break;
        }

        std::vector<Student> part(all_students.begin(), all_students.begin() + N);

        auto res_vec = run_experiment_deficit(part, 10, 10, 20);
        results.push_back(res_vec);

        std::unordered_map<std::string, Student> map_part;
        for (int i = 0; i < N; i++)
            map_part[part[i].get_email()] = part[i];
        auto res_map = run_experiment_deficit_map(map_part, 10, 10, 20);
        results.push_back(res_map);

        StudentBST bst_part;
        std::shuffle(part.begin(), part.end(), std::mt19937(std::random_device{}()));
        for (auto& s : part) {
            bst_part.insert(s);
        }
        auto res_bst = run_experiment_deficit_bst(bst_part, 10, 10, 20);
        results.push_back(res_bst);
    }

    measure_memory_usage("C:/Users/yarem/CLionProjects/Algos_homework/students.csv");

    std::ofstream out("C:/Users/yarem/CLionProjects/AlgoHomeWork/experiment_results.csv");
    out << "structure,size,total_ops\n";
    for (auto& r : results)
        out << r.structure << "," << r.size << "," << r.total_ops << "\n";
    out.close();

    std::cout << "\nРезультати збережено у 'experiment_results.csv'\n";

    sortByPhone_std(vec_s7_std);
    radixSortPhones(vec_s7_custom);

    save_students_to_csv(vec_s7_std, "C:/Users/yarem/CLionProjects/AlgoHomeWork/sorted_std.csv");
    save_students_to_csv(vec_s7_custom, "C:/Users/yarem/CLionProjects/AlgoHomeWork/sorted_custom.csv");
    return 0;
}
