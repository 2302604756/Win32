#include <windows.h>
#include <windowsx.h>
#include "Graphics.h"
#include <iostream>
#include "Window.h"
#include "App.h"

HWND ghMainWnd = 0;

bool InitWindowsApp(HINSTANCE instanceHandle, int show);

int Run(Graphics &gfx);

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



int WINAPI main(
	HINSTANCE hInst,
	HINSTANCE,
	LPSTR,
	int nCmdShow)
{
	App app;

	while (app.IsRunning())
	{
		app.ProcessMessages();
		app.Update();
		app.Render();
	}

	return 0;
}

//注册窗口实例
bool InitWindowsApp(HINSTANCE instanceHandle, int show) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"BasicWindowClass";

	if (!RegisterClass(&wc)) {
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}
	//创建窗口
	ghMainWnd = CreateWindow(L"BasicWindowClass", L"Basic Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, 0, 0, instanceHandle, 0);
	if (ghMainWnd == 0) {
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}
	//显示窗口
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);
	return true;
}


int Run(Graphics &gfx) {
	MSG msg = { 0 };

	BOOL bRet = 1;
	//消息循环
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_LBUTTONDOWN: 
	{
		// 获取鼠标在窗口的坐标
		int xPos = ((int)(short)((WORD)(((DWORD_PTR)(lParam)) & 0xffff)));
		int yPos = GET_Y_LPARAM(lParam);
		//传给全局变量


	
		return 0;
	}
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			PostQuitMessage(0);
		if(wParam == VK_SPACE)
			MessageBox(0, L"Space key pressed", L"Info", MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_QUIT:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:

		return 0;
	//获取鼠标位置
	case WM_MOUSEACTIVATE:
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}