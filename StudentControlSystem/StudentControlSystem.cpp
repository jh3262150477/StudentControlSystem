#include <cstdlib>
#include <iostream>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include "Page.h"
#include "StudentList.h"

#define WIDTH 480
#define HEIGHT 320
#define BACKGROUND_COLOR RGB(140,140,140)

using namespace std;


int main() {
    StudentList studentList;
    
    Page page(WIDTH, HEIGHT);
    page.init();
    SetWindowText(GetHWnd(), _T("StudentSystem"));

    //主页面
    page.createPage(BACKGROUND_COLOR);
    page.addTextElement(0, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"StudentSystem");
    page.addTextElement(0, 120, 75, 25, L"Script", YELLOW, L"By 108 all member");
    IMAGE icon;
    loadimage(&icon, _T("./Resources/star.png"), 64, 64, true);
    page.addImageElement(0, 350, 35, &icon);

    page.addButton(0, 120, 120, 110, 50, _T("Add"), [&]() {
        page.setCurrentIndex(1);
        });
    page.addButton(0, 250, 120, 110, 50, _T("Delete"), [&]() {
        page.setCurrentIndex(2);
        });
    page.addButton(0, 190, 180, 100, 50, _T("List"), [&]() {
        page.setCurrentIndex(3);
        });
    page.addButton(0, 190, 240, 100, 50, _T("Exit"), [&]() {
        int respone = MessageBox(NULL, _T("Are you sure to exit?"), _T("Exit"), MB_YESNO | MB_ICONQUESTION);
        if (respone == IDYES)
            page.close();
        });
    

    //添加学生页面

    page.createPage(BACKGROUND_COLOR);
    page.addTextElement(1, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"AddStudent");
    page.addTextElement(1, 20, 80, 20, L"黑体", BLACK, L"Enter Student Infomation");

    page.addTextElement(1, 40, 120, 20, L"黑体", BLACK, L"ID");
    TextBox* student_id = page.addTextBox(1, 100, 110, 200, 40, 10);

    page.addTextElement(1, 35, 170, 20, L"黑体", BLACK, L"Name");
    TextBox* student_name = page.addTextBox(1, 100, 160, 200, 40, 6);

    page.addTextElement(1, 30, 220, 20, L"黑体", BLACK, L"Score");
    TextBox* student_score = page.addTextBox(1, 100, 210, 200, 40, 10);

    page.addButton(1, 100, 260, 60, 50, L"Back", [&]() {
        page.setCurrentIndex(0);
        });

    page.addButton(1, 200, 260, 60, 50, L"Add", [&]() {
        studentList.addStudentBack(stoi(string(student_id->getText().begin(), student_id->getText().end())), student_name->getText(), stoi(string(student_score->getText().begin(), student_score->getText().end())));
        MessageBox(GetHWnd(), L"Add Success!!!", L"Tip", MB_OK | MB_ICONINFORMATION);
        });


    //删除学生界面

    page.createPage(BACKGROUND_COLOR);
    page.addTextElement(2, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"DeleteStudent");
    page.addTextElement(2, 20, 80, 20, L"黑体", BLACK, L"Enter Student ID");

    page.addTextElement(2, 35, 170, 20, L"黑体", BLACK, L"ID");
    TextBox* student_id_two = page.addTextBox(2, 100, 160, 200, 40, 6);

    page.addButton(2, 100, 260, 60, 50, L"Back", [&]() {
        page.setCurrentIndex(0);
        });

    page.addButton(2, 200, 260, 60, 50, L"Delete", [&]() {
        studentList.deleteStudentByCode(stoi(string(student_id_two->getText().begin(), student_id_two->getText().end())));
        cout << "当前链表长度为：" << studentList.getLength() << endl;
        MessageBox(GetHWnd(), L"Delete Success!!!", L"Tip", MB_OK | MB_ICONINFORMATION);
        });


    //学生列表界面

    page.createPage(BACKGROUND_COLOR);
    page.addTextElement(3, 10, 10, 20, L"Lucida Calligraphy", YELLOW, L"Student' List");

    TextBox* student_id_three = page.addTextBox(3, 10, 40, 180, 30, 10);
    student_id_three->setText(L"Find By ID");

    page.addButton(3, 200, 40, 30, 30, L"->", [&]() {
        Student* student = studentList.getStudentByCode(stoi(string(student_id_three->getText().begin(), student_id_three->getText().end())));
        if (student != nullptr) {
            wstringstream wss;
            wss << L"ID：" << student->code
                << L"\t\tName：" << student->name
                << L"\t\tScore：" << student->score;

            MessageBox(GetHWnd(), wss.str().c_str(), L"Search Success", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBox(GetHWnd(), L"Can find the Student", L"Error", MB_OK | MB_ICONERROR);
        }
        });

    page.addButton(3, 300, 35, 100, 40, L"Back", [&]() {
        page.setCurrentIndex(0);
        });

    TableWidget* table = page.addTable(3, 40, 100, 400, 200, [&]() {
        if (!studentList.isEmpty()) {
            table->setData(studentList.getData());
        }
        });

    page.addButton(3, 240, 40, 30, 30, L"R", [&]() {
        if (!studentList.isEmpty()) {
            table->setData(studentList.getData());
        }
        });
    page.setCurrentIndex(0);
    page.run();
    page.close();

    return 0;
}