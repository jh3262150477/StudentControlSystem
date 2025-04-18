#include "Page.h"
#include <iostream>

using namespace std;

void Page::addPage(IMAGE* n)
{
	pages.push_back(n);
	elements.push_back({});
}

void Page::addElement(int pageIndex, Element* e)
{
	if (pageIndex >= 0 && pageIndex < pages.size())
	{
		elements[pageIndex].push_back(e);
	}
}

void Page::setCurrentIndex(int index)
{
    if (index >= 0 && index < pages.size())
    {
        currentIndex = index;
    }
}

vector<Element*> Page::getElements(int pageIndex)
{
	return elements[pageIndex];
}

void Page::mouseMove(int mouseX, int mouseY)
{
	if (currentIndex >= 0 && currentIndex < pages.size())
	{
		for (Element* e : elements[currentIndex]) {
            e->checkMouseOver(mouseX, mouseY);
		}
	}
}

void Page::mouseClick(int mouseX, int mouseY)
{
	if (currentIndex >= 0 && currentIndex < pages.size())
	{
		for (Element* e : elements[currentIndex]) {
			if (e->checkClick(mouseX, mouseY))
            {
				break;
            }
		}
	}
}

void Page::draw()
{
	if (currentIndex >= 0 && currentIndex < pages.size())
	{
		putimage(0, 0, pages[currentIndex]);
		for (Element* element : elements[currentIndex]) {
			element->draw();
		}
	}
}

void Page::keyInput(wchar_t ch)
{
	if (currentIndex >= 0 && static_cast<size_t>(currentIndex) < elements.size()) {
		for (Element* element : elements[currentIndex]) {
			element->keyInput(ch);
		}
		cout << "Key input: " << ch << std::endl;
	}
}

void Page::updateCursors() {
	if (currentIndex >= 0 && static_cast<size_t>(currentIndex) < elements.size()) {
		for (Element* element : elements[currentIndex]) {
			element->updateCursor(); // 更新所有元素的光标状态
		}
	}
}

void Page::mouseWheel(int mouseX, int mouseY, int wheel)
{
	if (currentIndex >= 0 && currentIndex < pages.size())
	{
		for (Element* element : elements[currentIndex])
		{
			element->scroll(mouseX, mouseY, wheel);
		}
	}
}

Page::Page(int width, int height) :
	width(width), height(height), currentIndex(-1){}

void Page::init()
{
	initgraph(width, height, NULL);
}

void Page::run()
{
	ExMessage msg;

	BeginBatchDraw();

	while (true) {
		if (peekmessage(&msg))
		{
			int mouseX = msg.x;
			int mouseY = msg.y;

			switch (msg.message)
			{
				case WM_LBUTTONDOWN:
					mouseClick(mouseX, mouseY);
					break;
				case WM_MOUSEMOVE:
					mouseMove(mouseX, mouseY);
					break;
				case WM_CHAR:
					keyInput(msg.ch);
					break;
				case WM_MOUSEWHEEL:
					mouseWheel(mouseX, mouseY, msg.wheel);
					break;
			}
		}
		draw();
		updateCursors();
		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
}

void Page::close()
{
	closegraph();
	exit(0);
}

int Page::getCurrentIndex()
{
	return currentIndex;
}

int Page::createPage(COLORREF color)
{
	IMAGE* img = new IMAGE(width, height);
	setfillcolor(color);
	solidrectangle(0, 0, width, height);
	getimage(img, 0, 0, width, height);
	addPage(img);
	return 0;
}

Text* Page::addTextElement(int pageIndex, int x, int y, int weight, const wchar_t* ifFaceName, COLORREF textColor, const wchar_t* text)
{
	Text* t = new Text(x, y, weight, ifFaceName, textColor, text);
	addElement(pageIndex, t);
	return t;
}

void Page::addImageElement(int pageIndex, int x, int y, IMAGE* image)
{
	Image* i = new Image(x, y, image);
	addElement(pageIndex, i);
}

Button* Page::addButton(int pageIndex, int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick)
{
	Button* b = new Button(x, y, width, height, text, onClick);
	addElement(pageIndex, b);
	return b;
}

TextBox* Page::addTextBox(int pageIndex, int x, int y, int width, int height, int maxWord)
{
	TextBox* t = new TextBox(x, y, width, height, maxWord);
	addElement(pageIndex, t);
    return t;
}

TableWidget* Page::addTable(int pageIndex, int x, int y, int width, int height, const std::function<void()>& onContinue) {
	TableWidget* table = new TableWidget(x, y, width, height, 10, onContinue);
	addElement(pageIndex, table);
	return table;
}