// 2048.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "2048.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
Game *game;							            // ��Ϸ������
UINT timerID;							        // ��ʱ��ID
UINT time;							            // ����ʱ

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	// wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY2048);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 450, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		// Init game
		game = new Game();

		// Init time
		time = 5 * 60 * 100;

		// Set timer
		timerID = SetTimer(hWnd, NULL, 1000, TimerProc);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
		case 'H':
		case VK_LEFT:
			if (!game->canMove(MoveCommandLeft)) break;
			game->doMove(MoveCommandLeft);
			InvalidateRect(hWnd, &game->chessboardRect, true);
			InvalidateRect(hWnd, &game->scoreLabelRect, false);
			break;
		case 'D':
		case 'L':
		case VK_RIGHT:
			if (!game->canMove(MoveCommandRight)) break;
			game->doMove(MoveCommandRight);
			InvalidateRect(hWnd, &game->chessboardRect, true);
			InvalidateRect(hWnd, &game->scoreLabelRect, false);;
			break;
		case 'W':
		case 'K':
		case VK_UP:
			if (!game->canMove(MoveCommandUp)) break;
			game->doMove(MoveCommandUp);
			InvalidateRect(hWnd, &game->chessboardRect, true);
			InvalidateRect(hWnd, &game->scoreLabelRect, false);
			break;
		case 'S':
		case 'J':
		case VK_DOWN:
			if (!game->canMove(MoveCommandDown)) break;
			game->doMove(MoveCommandDown);
			InvalidateRect(hWnd, &game->chessboardRect, true);
			InvalidateRect(hWnd, &game->scoreLabelRect, false);
			break;
		case 'R':
			if (game->isTerminated()) game->restart();
			break;
		default:
			break;
		}

		if (game->isTerminated()) {
			KillTimer(hWnd, timerID); // Kill timer
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MY2048_DIALOG), hWnd, Info); // Game over or win
		}
			

		break;
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);

		// TODO:  �ڴ���������ͼ����...
		// Get client rect
		RECT rect;
		GetClientRect(hWnd, &rect);

		game->setRect(rect);
		game->draw(ps.hdc);

		// Draw time
		RECT timeRect = game->scoreLabelRect;
		long width = getRectWidth(timeRect);
		timeRect.left -= width;
		timeRect.right -= width;

		// Draw time
		WCHAR buffer[10];
		getTimeStringByValue(time, buffer);
		drawNumberOnTimeLabel(ps.hdc, timeRect, buffer, RGB(249, 246, 242));

		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// �����ѡ������Ϣ�������
INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		HWND stDisplay = GetDlgItem(hDlg, IDC_STATIC);

		wchar_t buffer[256];
		if (game->isWon()) {
			wsprintfW(buffer, L"You win!!!\nYour score: %d", game->getScore());
		} else {
			if (time == 0) {
				wsprintfW(buffer, L"Time is over! You lose :(\nYour score: %d", game->getScore());
			} else {
				wsprintfW(buffer, L"You lose :(\nYour score: %d", game->getScore());
			}
		}

		SendMessage(stDisplay, WM_SETTEXT, NULL, (WPARAM)buffer);
	}
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDRETRY) {
			HWND hParentWnd = GetParent(hDlg);

			time = 5 * 60 * 100;
			timerID = SetTimer(hParentWnd, NULL, 1000, TimerProc); // Reset timer

			// Restart the game
			game->restart(); // Restart
			EndDialog(hDlg, LOWORD(wParam)); // Close dialog
			InvalidateRect(hParentWnd, &game->chessboardRect, true); // Repaint chessboard
			InvalidateRect(hParentWnd, &game->scoreLabelRect, false); // Repaint score label

			RECT timeRect = game->scoreLabelRect;
			long width = getRectWidth(timeRect);
			timeRect.left -= width;
			timeRect.right -= width;
			InvalidateRect(hParentWnd, &timeRect, false); // Repatin timer

			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDNO) {
			HWND hParentWnd = GetParent(hDlg);
			EndDialog(hDlg, LOWORD(wParam));
			DestroyWindow(hParentWnd); // Close main window, exit
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	if (game->isTerminated()) return;

	if (time == 0) {
		game->stop(); // Stop game
		return;
	}

	time -= 100;
	RECT timeRect = game->scoreLabelRect;
	long width = getRectWidth(timeRect);
	timeRect.left -= width;
	timeRect.right -= width;
	InvalidateRect(hwnd, &timeRect, false);
}
