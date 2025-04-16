#pragma once

#include <graphics.h>
#include <vector>
#include "Element.h"

class Widget
{
private:
	int width;
	int height;
	int currentIndex;
	vector<IMAGE*> pages;
	vector<vector<Element*>> elements;

	void addPage(IMAGE* n);
	void addElement(int pageIndex, Element* e);
	void mouseMove(int mouseX, int mouseY);
	void mouseClick(int mouseX, int mouseY);
	void draw();
	void keyInput(wchar_t ch);
	void updateCursors();
	void mouseWheel(int mouseX, int mouseY, int wheel);

public:
	Widget(int width, int height);

	void init(); //初始化图形界面
	void run(); //进入消息循环队列
	void close(); // 关闭图形绘制界面
	int getCurrentIndex();
	void setCurrentIndex(int index);
	vector<Element*> getElements(int pageIndex);
	int createPage(COLORREF color);
	Text* addTextElement(int pageIndex, int x, int y, int weight, const wchar_t* ifFaceName, COLORREF textColor, const wchar_t* text);
	void addImageElement(int pageIndex, int x, int y, IMAGE* image);
	Button* addButton(int pageIndex, int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick);
	TextBox* addTextBox(int pageIndex, int x, int y, int width, int height, int maxWord);
	TableWidget* addTable(int pageIndex, int x, int y, int width, int height, const std::function<void()>& onContinue);
};

