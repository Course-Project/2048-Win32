#include "stdafx.h"
#include <cmath>

long getRectWidth(RECT rect) {
	return rect.right - rect.left;
}

long getRectHeight(RECT rect) {
	return rect.bottom - rect.top;
}

// 画线
void drawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

// 画圆角矩形
void drawRoundRect(HDC hdc, RECT cellRect, COLORREF bgColor) {
	HBRUSH brush = CreateSolidBrush(bgColor);
	HPEN pen = CreatePen(PS_NULL, 0, NULL);
	SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	RoundRect(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom, 10, 10);
	DeleteObject(brush);
	DeleteObject(pen);
}

// 创建固定字体，高度不同
HFONT createFont(int height) {
	return CreateFont(height, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, FF_ROMAN, TEXT("Arial"));
}

// 将时间(单位：百分之一秒)转换成宽字符串(长度为8位)
void getTimeStringByValue(UINT time, LPCWSTR result) {
	// int hSeconds = time % 100;
	time /= 100;
	int seconds = time % 60;
	time /= 60;
	int minutes = time % 60;

	WCHAR buffer[10];
	wsprintfW(buffer, L"0%d:%d", minutes, seconds);

	if (seconds < 10) {
		buffer[5] = buffer[4];
		buffer[4] = buffer[3];
		buffer[3] = L'0';
	}

	memcpy((void *)result, buffer, sizeof(buffer));
}

// 画数字
void drawNumberOnCell(HDC hdc, RECT cellRect, const int &number, COLORREF fontColor) {
	int fontSize = log2(number);

	double factor = fontSize < 7 ? 1 : (fontSize < 10 ? 1.5 : 2);

	HFONT font = createFont(getRectHeight(cellRect) * 0.66 / factor);
	SetTextColor(hdc, fontColor); // Font color
	SetBkMode(hdc, TRANSPARENT); // Background color transparentk
	SelectObject(hdc, font); // Set font

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", number);
	DrawText(hdc, buffer, -1, &cellRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DeleteObject(font);
}

void drawNumberOnRect(HDC hdc, RECT rect, const int &number, COLORREF fontColor) {	
	HFONT font = createFont(getRectHeight(rect) * 0.66);
	SetTextColor(hdc, fontColor); // Font color
	SetBkMode(hdc, TRANSPARENT); // Background color transparentk
	SelectObject(hdc, font); // Set font

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", number);
	DrawText(hdc, buffer, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DeleteObject(font);
}

void drawNumberOnTimeLabel(HDC hdc, RECT rect, LPCWSTR pWStr, COLORREF fontColor) {	
	HFONT font = createFont(getRectHeight(rect) * 0.66 / 2);
	SetTextColor(hdc, fontColor); // Font color
	SetBkMode(hdc, TRANSPARENT); // Background color transparentk
	SelectObject(hdc, font); // Set font

	DrawText(hdc, pWStr, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DeleteObject(font);
}
