#include "Element.h"
#include <iostream>

Text::Text(int x, int y, int weight, const wchar_t* ifFaceName, COLORREF textColor, const wchar_t* text):
	x(x), y(y), weight(weight),ifFaceName(ifFaceName), textColor(textColor), text(text)
{
}

void Text::draw()
{
	LOGFONT font;
    setbkmode(TRANSPARENT);
	gettextstyle(&font);
    settextcolor(textColor);
	font.lfHeight = weight;
    font.lfQuality = ANTIALIASED_QUALITY;
	_tcscpy_s(font.lfFaceName, ifFaceName);
	settextstyle(&font);
	outtextxy(x, y, text);
}

Image::Image(int x, int y,IMAGE* image):
    x(x), y(y), image(image)
{
}

void Image::draw()
{
	putimage(x, y, image);
}

Button::Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick)
    : x(x), y(y), width(width), height(height), scale(1.0f), text(text), onClick(onClick), isMouseOver(false) {
}

void Button::checkMouseOver(int mouseX, int mouseY) {
    isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

    if (isMouseOver) {
        scale = 1.0f;
    }
    else {
        scale = 1.0f;
    }
}

bool Button::checkClick(int mouseX, int mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        onClick();
        isMouseOver = false;
        scale = 1.0f;
        return true;
    }
    return false;
}

void Button::draw() {
    clearrectangle(x, y, x + width, y + height);

    int scaleWidth = static_cast<int>(width * scale);
    int scaleHeight = static_cast<int>(height * scale);
    int scaleX = x + (width - scaleWidth) / 2;
    int scaleY = y + (height - scaleHeight) / 2;

    if (isMouseOver) {
        setlinecolor(RGB(0, 120, 215));
        setfillcolor(RGB(229, 241, 251));
    }
    else {
        setlinecolor(RGB(173, 173, 173));
        setfillcolor(RGB(225, 225, 225));
    }

    fillrectangle(scaleX, scaleY, scaleX + scaleWidth, scaleY + scaleHeight);
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    LOGFONT font;
    gettextstyle(&font);
    font.lfWeight = FW_BOLD;
    font.lfHeight = static_cast<int>(20 * scale);
    font.lfQuality = ANTIALIASED_QUALITY;
    _tccpy(font.lfFaceName, _T("黑体"));
    settextstyle(&font);

    int textX = scaleX + (scaleWidth - textwidth(text.c_str())) / 2;
    int textY = scaleY + (scaleHeight - textheight(_T("黑体"))) / 2;

    outtextxy(textX, textY, text.c_str());
}

TextBox::TextBox(int x, int y, int width, int height, int maxWord)
    : x(x), y(y), width(width), height(height), maxWord(maxWord), isSelected(false), showCursor(false), cursorPos(0) {
}


const std::wstring& TextBox::getText() const
{
    return text;
}

bool TextBox::getSelect() const {
    return isSelected;
}

bool TextBox::isEmpty() const {
    return text.empty();
}

void TextBox::clear() {
    text = L"";
}

void TextBox::setText(const std::wstring& newText) {
    text = newText;
    cursorPos = text.length();
}


void TextBox::draw() {
    setfillcolor(WHITE);
    if (isSelected)
    {
        setlinecolor(RGB(0, 120, 215));
        fillrectangle(x, y, x + width, y + height);
    }
    else
    {
        setlinecolor(RGB(122, 122, 122));
        fillrectangle(x, y, x + width, y + height);
    }

    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(height * 3 / 4, 0, _T("黑体"));

    outtextxy(x + 5, y + (height - textheight(_T("黑体"))) / 2, text.c_str());

    setlinecolor(BLACK);
    if (isSelected && showCursor)
    {
        int cursorX = x + 5 + textwidth(text.substr(0, cursorPos).c_str());
        line(cursorX, y + 2 + height / 8, cursorX, y + height * 7 / 8 - 2);
    }
}

bool TextBox::checkClick(int mouseX, int mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        isSelected = true;
        return true;
    }
    else {
        isSelected = false;
        cursorPos = text.length();
        return false;
    }
}

void TextBox::keyInput(wchar_t ch) {
    if (isSelected)
    {
        switch (ch)
        {
        case '\b':	
            if (!text.empty() && cursorPos > 0)
            {
                text.erase(cursorPos - 1, 1);
                cursorPos--;
            }
            break;
        case '\r':
        case '\n':
            cursorPos = text.length();
            isSelected = false;
            break;
        default:
            if (text.length() < maxWord)
            {
                text.insert(cursorPos, 1, ch);
                cursorPos++;
            }
        }
    }
}

void TextBox::updateCursor() {
    static DWORD lastTick = GetTickCount();
    DWORD currentTick = GetTickCount();
    if (currentTick - lastTick >= 500) { 
        showCursor = !showCursor;
        lastTick = currentTick;
    }
}

TableWidget::TableWidget(int x, int y, int width, int height, int visibleRowCount, const std::function<void()>& onContinue)
    : x(x), y(y), width(width), height(height), visibleRowCount(visibleRowCount), onContinue(onContinue) {
    data = { {} };
    rowHeight = height / visibleRowCount;
    selectedRow = -1;
    scrollOffset = 0;
    scrollbarWidth = 20;
    handleHeight = 30;
    handleY = 0;
}

int TableWidget::getSelectedRow() const {
    return selectedRow;
}

wstring TableWidget::getSelectedInfo(int col) const {
    return data[selectedRow][col];
}

void TableWidget::setData(const vector<vector<wstring>>& newData) {
    data = newData;
    calculateColumnWidths();
}

void TableWidget::calculateColumnWidths() {
    columnWidths.clear();
    if (!data.empty()) {
        columnWidths.resize(data[0].size(), 0);
        for (const auto& row : data) {
            for (size_t j = 0; j < row.size(); ++j) {
                int width = textwidth(row[j].c_str());
                if (width > columnWidths[j]) {
                    columnWidths[j] = width;
                }
            }
        }
    }

    int sumWidth = 0;
    for (auto it : columnWidths)
        sumWidth += it;
    for (auto& it : columnWidths)
        it = static_cast<float>(it) / sumWidth * width;
}

void TableWidget::scrollUp() {
    if (scrollOffset > 0) {
        scrollOffset--;
    }
    if (scrollOffset < 0) {
        scrollOffset = 0;
    }
}

void TableWidget::scrollDown() {
    int maxScrollOffset = data.size() - visibleRowCount;
    if (scrollOffset < maxScrollOffset) {
        scrollOffset++;
    }
}

void TableWidget::scroll(int mouseX, int mouseY, int wheel) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        if (wheel > 0) {
            scrollUp();
        }
        else if (wheel < 0) {
            scrollDown();
        }
    }
}

void TableWidget::handleMouseClick(int mouseX, int mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        int clickedRow = (mouseY - y) / rowHeight + scrollOffset;

        if (clickedRow >= 0 && clickedRow < static_cast<int>(data.size())) {
            selectedRow = clickedRow;
        }
        else {
            selectedRow = -1;
        }
    }
}

void TableWidget::draw() {
    onContinue();

    clearrectangle(x, y, x + width, y + height);
    setbkmode(1);
    setfillcolor(WHITE);
    solidrectangle(x, y, x + width, y + height);
    setlinecolor(BLACK);
    settextstyle(12, 0, _T("黑体"));

    int rowCount = min(visibleRowCount, static_cast<int>(data.size()));

    // 绘制表头
    int headerY = y;
    int columnX = x;
    for (int j = 0; j < data[0].size(); ++j) {
        int columnWidth = columnWidths[j];
        rectangle(columnX, headerY, columnX + columnWidth, headerY + rowHeight);
        int textX = columnX + (columnWidth - textwidth(data[0][j].c_str())) / 2;
        int textY = headerY + (rowHeight - textheight(_T("黑体"))) / 2;
        outtextxy(textX, textY, data[0][j].c_str());
        columnX += columnWidth;
    }

    // 绘制表格内容
    for (int i = 1; i < rowCount; ++i) {
        int rowY = y + i * rowHeight;
        int dataIndex = i + scrollOffset;
        columnX = x;
        for (int j = 0; j < data[dataIndex].size(); ++j) {
            int columnWidth = columnWidths[j];
            bool isSelectedRow = (dataIndex == selectedRow);
            if (isSelectedRow) {
                setfillcolor(LIGHTBLUE);
                settextcolor(RED);
            }
            else {
                setfillcolor(WHITE);
                settextcolor(BLACK);
            }
            fillrectangle(columnX, rowY, columnX + columnWidth, rowY + rowHeight);
            int textX = columnX + (columnWidth - textwidth(data[dataIndex][j].c_str())) / 2;
            int textY = rowY + (rowHeight - textheight(_T("黑体"))) / 2;
            outtextxy(textX, textY, data[dataIndex][j].c_str());
            columnX += columnWidth;
        }
    }

    // 绘制滚动条背景
    int scrollbarX = x + width;
    setfillcolor(LIGHTGRAY);
    solidrectangle(scrollbarX, y, scrollbarX + scrollbarWidth, y + height);

    // 计算滑块位置和大小
    int handleX = scrollbarX;
    int handleWidth = scrollbarWidth;
    int maxHandleY = height - handleHeight;
    handleY = maxHandleY * static_cast<double>(scrollOffset) / (data.size() - visibleRowCount);

    // 绘制滑块
    setfillcolor(DARKGRAY);
    solidrectangle(handleX, y + handleY, handleX + handleWidth, y + handleY + handleHeight);
}