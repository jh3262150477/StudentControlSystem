#include <cstdlib>
#include <iostream>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include "Widget.h"
#include "StudentList.h"

#define WIDTH 480
#define HEIGHT 320
#define BACKGROUND_COLOR RGB(140,140,140)

using namespace std;


int main() {
    StudentList studentList;
    
    Widget widget(WIDTH, HEIGHT);
    widget.init();
    SetWindowText(GetHWnd(), _T("StudentSystem"));

    //主页面
    widget.createPage(BACKGROUND_COLOR);
    widget.addTextElement(0, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"StudentSystem");
    widget.addTextElement(0, 120, 75, 25, L"Script", YELLOW, L"By 108 all member");
    IMAGE icon;
    loadimage(&icon, _T("./Resources/star.png"), 64, 64, true);
    widget.addImageElement(0, 350, 35, &icon);

    widget.addButton(0, 120, 120, 110, 50, _T("Add"), [&]() {
        widget.setCurrentIndex(1);
        });
    widget.addButton(0, 250, 120, 110, 50, _T("Delete"), [&]() {
        widget.setCurrentIndex(2);
        });
    widget.addButton(0, 190, 180, 100, 50, _T("List"), [&]() {
        widget.setCurrentIndex(3);
        });
    widget.addButton(0, 190, 240, 100, 50, _T("Exit"), [&]() {
        int respone = MessageBox(NULL, _T("Are you sure to exit?"), _T("Exit"), MB_YESNO | MB_ICONQUESTION);
        if (respone == IDYES)
            widget.close();
        });
    

    //添加学生页面

    widget.createPage(BACKGROUND_COLOR);
    widget.addTextElement(1, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"AddStudent");
    widget.addTextElement(1, 20, 80, 20, L"黑体", BLACK, L"Enter Student Infomation");

    widget.addTextElement(1, 40, 120, 20, L"黑体", BLACK, L"ID");
    TextBox* student_id = widget.addTextBox(1, 100, 110, 200, 40, 10);

    widget.addTextElement(1, 35, 170, 20, L"黑体", BLACK, L"Name");
    TextBox* student_name = widget.addTextBox(1, 100, 160, 200, 40, 6);

    widget.addTextElement(1, 30, 220, 20, L"黑体", BLACK, L"Score");
    TextBox* student_score = widget.addTextBox(1, 100, 210, 200, 40, 10);

    widget.addButton(1, 100, 260, 60, 50, L"Back", [&]() {
        widget.setCurrentIndex(0);
        });

    widget.addButton(1, 200, 260, 60, 50, L"Add", [&]() {
        studentList.addStudentBack(stoi(string(student_id->getText().begin(), student_id->getText().end())), student_name->getText(), stoi(string(student_score->getText().begin(), student_score->getText().end())));
        MessageBox(GetHWnd(), L"Add Success!!!", L"Tip", MB_OK | MB_ICONINFORMATION);
        });


    //删除学生界面

    widget.createPage(BACKGROUND_COLOR);
    widget.addTextElement(2, 10, 20, 50, L"Lucida Calligraphy", YELLOW, L"DeleteStudent");
    widget.addTextElement(2, 20, 80, 20, L"黑体", BLACK, L"Enter Student ID");

    widget.addTextElement(2, 35, 170, 20, L"黑体", BLACK, L"ID");
    TextBox* student_id_two = widget.addTextBox(2, 100, 160, 200, 40, 6);

    widget.addButton(2, 100, 260, 60, 50, L"Back", [&]() {
        widget.setCurrentIndex(0);
        });

    widget.addButton(2, 200, 260, 60, 50, L"Delete", [&]() {
        studentList.deleteStudentByCode(stoi(string(student_id_two->getText().begin(), student_id_two->getText().end())));
        cout << "当前链表长度为：" << studentList.getLength() << endl;
        MessageBox(GetHWnd(), L"Delete Success!!!", L"Tip", MB_OK | MB_ICONINFORMATION);
        });


    //学生列表界面

    widget.createPage(BACKGROUND_COLOR);
    widget.addTextElement(3, 10, 10, 20, L"Lucida Calligraphy", YELLOW, L"Student' List");

    TextBox* student_id_three = widget.addTextBox(3, 10, 40, 180, 30, 10);
    student_id_three->setText(L"Find By ID");

    widget.addButton(3, 200, 40, 30, 30, L"->", [&]() {
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

    widget.addButton(3, 300, 35, 100, 40, L"Back", [&]() {
        widget.setCurrentIndex(0);
        });

    TableWidget* table = widget.addTable(3, 40, 100, 400, 200, [&]() {
        if (!studentList.isEmpty()) {
            table->setData(studentList.getData());
        }
        });

    widget.addButton(3, 240, 40, 30, 30, L"R", [&]() {
        if (!studentList.isEmpty()) {
            table->setData(studentList.getData());
        }
        });
    widget.setCurrentIndex(0);
    widget.run();
    widget.close();

    return 0;
}