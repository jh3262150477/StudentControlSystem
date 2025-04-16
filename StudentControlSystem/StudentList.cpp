#include "StudentList.h"

StudentList::StudentList() : head(nullptr), length(0) {}

StudentList::~StudentList() {
    freeList();
}

void StudentList::addStudentBack(int code, wstring name, int score) {
    Student* std = new Student();
    std->code = code;
    std->name = name;
    std->score = score;
    std->next = nullptr;

    if (head == nullptr) {
        head = std;
    }
    else {
        Student* now_std = head;
        while (now_std->next != nullptr) {
            now_std = now_std->next;
        }
        now_std->next = std;
    }
    length++;
}

Student* StudentList::getStudentByCode(int code) {
    if (head == nullptr) {
        cout << "当前链表数据为空" << endl;
        return nullptr;
    }
    Student* current = head;
    while (current != nullptr && current->code != code) {
        current = current->next;
    }
    if (current == nullptr) {
        cout << "数据库中无该学生信息" << endl;
        return nullptr;
    }
    return current;
}

int StudentList::getLength() const {
    return length;
}

bool StudentList::isEmpty() const {
    return head == nullptr;
}

vector<vector<wstring>> StudentList::getData() const {
    vector<vector<wstring>> data = { { L"ID", L"Name", L"Score" } };
    Student* current = head;
    while (current != nullptr) {
        data.push_back({ to_wstring(current->code), current->name, to_wstring(current->score) });
        current = current->next;
    }
    return data;
}

void StudentList::deleteStudentByCode(int code) {
    if (head == nullptr) return;
    if (head->code == code) {
        Student* temp = head;
        head = head->next;
        delete temp;
        length--;
        return;
    }
    Student* current = head;
    while (current->next != nullptr && current->next->code != code) {
        current = current->next;
    }
    if (current->next != nullptr) {
        Student* temp = current->next;
        current->next = current->next->next;
        delete temp;
        length--;
    }
}

void StudentList::freeList() {
    Student* current = head;
    while (current != nullptr) {
        Student* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    length = 0;
}