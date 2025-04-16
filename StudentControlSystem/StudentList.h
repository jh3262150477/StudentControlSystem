#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Student {
    int code;
    wstring name;
    int score;
    Student* next;
};

class StudentList {
private:
    Student* head;
    int length;

public:
    StudentList();
    ~StudentList();

    void addStudentBack(int code, wstring name, int score);
    Student* getStudentByCode(int code);
    int getLength() const;
    bool isEmpty() const;
    vector<vector<wstring>> getData() const;
    void deleteStudentByCode(int code);
    void freeList();
};