#pragma once

long getRectWidth(RECT rect);
long getRectHeight(RECT rect);

void drawLine(HDC hdc, int x1, int y1, int x2, int y2);
void drawRoundRect(HDC hdc, RECT cellRect, COLORREF bgColor);
void drawNumberOnCell(HDC hdc, RECT cellRect, const int &number, COLORREF fontColor);
void drawNumberOnRect(HDC hdc, RECT rect, const int &number, COLORREF fontColor);
void drawNumberOnTimeLabel(HDC hdc, RECT rect, LPCWSTR pWStr, COLORREF fontColor);

HFONT createFont(int height);
void getTimeStringByValue(UINT time, LPCWSTR result);
