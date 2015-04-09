#include "stdafx.h"

//#include <vld.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	RECT winRect = { 0, 0, 500, 360 };
	RECT newRect = { 0, 0, 500, 360 };
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;

	if (!IsWindows7SP1OrGreater()){
		MessageBox(NULL, TEXT("Need Windows 7 SP1 or Greater!"), TEXT("Incorrect system version"), MB_OK | MB_ICONERROR);
		return -1;
	}

	//cudaGetDeviceCount(&g_cudaDeviceCount);

	AdjustWindowRect(&newRect, windowStyle, FALSE);

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("FastRecode");
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hWnd = CreateWindow(wc.lpszClassName,
		TEXT("FastRecode"),
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		newRect.right - newRect.left - 1,
		newRect.bottom - newRect.top - 1,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
		return FALSE;

	Initialize(hWnd, &winRect);
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//cThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ControlThread, NULL, 0, &cid);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// 窗口处理器
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC	hDC;
	PAINTSTRUCT ps;
	int i;
	int maxfiledisp;
	int maxfilenextdisp;
	int gpustat = 1;
	SIZE gputextsize;
	int percent[8];

	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		g_uiMan.ProcessInput(Point(LOWORD(lParam), HIWORD(lParam)), UIMan::MOUSEDOWN);
		break;

	case WM_MOUSEMOVE:
		g_uiMan.ProcessInput(Point(LOWORD(lParam), HIWORD(lParam)), UIMan::MOUSEMOVE);
		break;
	
	case WM_TIMER:
		g_uiMan.Render();
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		BitBlt(hDC, 0, 0, g_uiMan.getWidth(), g_uiMan.getHeight(), g_uiMan.getDC(), 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}