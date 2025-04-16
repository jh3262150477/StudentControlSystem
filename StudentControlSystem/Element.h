#pragma once

#include <graphics.h>
#include <functional>
#include <string>
#include <vector>

using namespace std;

class Element
{
	public:
        virtual void draw() = 0;
        virtual bool checkClick(int mouseX, int mouseY) { return false; }
        virtual void checkMouseOver(int mouseX, int mouseY) {}
        virtual void keyInput(wchar_t ch) {}
        virtual void updateCursor() {}
        virtual void scroll(int mouseX, int mouseY, int wheel){}
};

class Text : public Element {
    public:
        Text(int x, int y, int weight,const wchar_t* ifFaceName, COLORREF textColor, const wchar_t* text);
        void draw() override;
    private:
        int x, y;
        int weight;
        const wchar_t* ifFaceName;
        COLORREF textColor;
        const wchar_t* text;
};

class Image : public Element {
    public:
        Image(int x, int y, IMAGE* image);
        void draw() override;
    private:
        int x, y;
        IMAGE* image;
};

class Button : public Element {
public:
    Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick);
    bool checkClick(int mouseX, int mouseY) override;
    void checkMouseOver(int mouseX, int mouseY) override;
    void draw() override;

private:
    int x, y, width, height;
    float scale;
    std::wstring text;
    std::function<void()> onClick;
    bool isMouseOver;
};

class TextBox : public Element {
public: 
    TextBox(int x, int y, int width, int height, int maxWord);

    const std::wstring& getText() const;
    bool getSelect() const;
    bool isEmpty() const;
    void clear();
    void setText(const std::wstring& newText);
    void draw() override;
    bool checkClick(int mouseX, int mouseY) override;
    void keyInput(wchar_t ch) override;
    void updateCursor() override;



private:
    int x;
    int y;
    int width;
    int height;
    int maxWord;
    std::wstring text;
    bool isSelected;
    bool showCursor;
    int cursorPos;
};

class TableWidget : public Element {
private:
    int x;
    int y;
    int width;
    int height;
    int visibleRowCount;
    int rowHeight;  
    int scrollOffset; 
    vector<vector<wstring>> data;
    vector<int> columnWidths;
    int selectedRow;
    int scrollbarWidth;
    int handleHeight;
    int handleY;

    void calculateColumnWidths();
    std::function<void()> onContinue;//这个Widget管理系统感觉还是不是很好，临时补救一下

public:
    TableWidget(int x, int y, int width, int height, int visibleRowCount, const std::function<void()>& onContinue);

    int getSelectedRow() const;
    wstring getSelectedInfo(int col) const;
    void setData(const vector<vector<wstring>>& newData);
    void scrollUp();
    void scrollDown();
    void scroll(int mouseX, int mouseY, int wheel) override;
    void handleMouseClick(int mouseX, int mouseY);
    void draw();
};