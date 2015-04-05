#include "stdafx.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wc;
	MSG msg;
	RECT winRect;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;

	if (!IsWindows7SP1OrGreater()){
		MessageBox(NULL, TEXT("Need Windows 7 SP1 or Greater!"), TEXT("Incorrect system version"), MB_OK | MB_ICONERROR);
		return -1;
	}

	cudaGetDeviceCount(&g_cudaDeviceCount);

	SetRect(&winRect, 0, 0, 500, 300);;
	AdjustWindowRect(&winRect, windowStyle, FALSE);

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
	wc.lpszClassName = TEXT("ModelApp");
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	g_hWnd = CreateWindow(wc.lpszClassName,
		TEXT("batchproc"),
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		winRect.right - winRect.left - 1,
		winRect.bottom - winRect.top - 1,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (g_hWnd == NULL)
		return FALSE;

	//Initialize();
	//
	//if (programend)
	//	return -1;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

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
		//hwnd = hWnd;
		break;

	case WM_KEYDOWN:
		break;

	case WM_TIMER:
		/*
		maxfiledisp = (maxfile == 0) ? 1 : maxfile;
		maxfilenextdisp = (maxfilenext == 0) ? 1 : maxfilenext - 7;

		SelectObject(hDCBack, font);

		SelectObject(hDCBack, invisiblepen);
		SelectObject(hDCBack, whitebrush);
		Rectangle(hDCBack, 0, 0, sPos.right - sPos.left, sPos.bottom - sPos.top);

		if (gpuallbusy){
			SetTextAlign(hDCBack, TA_RIGHT);
			wsprintf(str, TEXT("GPU BUSY"));
			TextOut(hDCBack, 390, 5, str, lstrlen(str));
			SetTextAlign(hDCBack, TA_LEFT);
		}

		wsprintf(str, TEXT("GPU STATUS: "));
		TextOut(hDCBack, 120, 5, str, lstrlen(str));
		GetTextExtentPoint32(hDCBack, str, lstrlen(str), &gputextsize);
		SelectObject(hDCBack, invisiblepen);
		for (i = 0; i < cudacount; i++){
			if (gpubusy[i])
				SelectObject(hDCBack, gpubrush);
			else
				SelectObject(hDCBack, blackbrush);

			Rectangle(hDCBack, 120 + gputextsize.cx + i * 12, 5 + 3, 120 + gputextsize.cx + i * 12 + 10, 5 + gputextsize.cy - 1);
		}

		wsprintf(str, TEXT("Threads : %d"), threadrunning);
		TextOut(hDCBack, 10, 5, str, lstrlen(str));

		// Overall Progress
		wsprintf(str, TEXT("Books %4d/%4d"), bookprocessed, maxbook);
		TextOut(hDCBack, 10, 25, str, lstrlen(str));
		wsprintf(str, TEXT("%d%%"), bookprocessed * 100 / maxbook);
		TextOut(hDCBack, 200, 25, str, lstrlen(str));

		SelectObject(hDCBack, blackpen);
		SelectObject(hDCBack, whitebrush);
		Rectangle(hDCBack, 10, 45, 390, 60);

		SelectObject(hDCBack, invisiblepen);
		SelectObject(hDCBack, bluebrush);
		Rectangle(hDCBack, 11, 47, 11 + (((bookprocessed << 16) / maxbook * 379) >> 16), 59);

		// Book Process Progress
		wsprintf(str, TEXT("Current Book : %s"), curname);
		TextOut(hDCBack, 10, 75, str, lstrlen(str));
		wsprintf(str, TEXT("File Processed %4d/%4d"), curfile, maxfiledisp);
		TextOut(hDCBack, 10, 95, str, lstrlen(str));
		wsprintf(str, TEXT("%d%%"), curfile * 100 / maxfiledisp);
		TextOut(hDCBack, 200, 95, str, lstrlen(str));

		SelectObject(hDCBack, blackpen);
		SelectObject(hDCBack, whitebrush);
		Rectangle(hDCBack, 10, 115, 390, 130);

		SelectObject(hDCBack, invisiblepen);
		SelectObject(hDCBack, bluebrush);
		Rectangle(hDCBack, 11, 117, 11 + (((curfile << 16) / maxfiledisp * 379) >> 16), 129);

		// File Written Progress
		wsprintf(str, TEXT("File Written %4d/%4d"), writefile, maxfiledisp);
		TextOut(hDCBack, 10, 135, str, lstrlen(str));
		wsprintf(str, TEXT("%d%%"), writefile * 100 / maxfiledisp);
		TextOut(hDCBack, 200, 135, str, lstrlen(str));

		SelectObject(hDCBack, blackpen);
		SelectObject(hDCBack, whitebrush);
		Rectangle(hDCBack, 10, 155, 390, 170);

		SelectObject(hDCBack, invisiblepen);
		SelectObject(hDCBack, bluebrush);
		Rectangle(hDCBack, 11, 157, 11 + (((writefile << 16) / maxfiledisp * 379) >> 16), 169);

		// File Read Progress
		wsprintf(str, TEXT("Next Book : %s"), nextname);
		TextOut(hDCBack, 10, 185, str, lstrlen(str));
		wsprintf(str, TEXT("Next Book Read %4d/%4d"), curfilenext, maxfilenextdisp);
		TextOut(hDCBack, 10, 205, str, lstrlen(str));
		wsprintf(str, TEXT("%d%%"), curfilenext * 100 / (maxfilenextdisp));
		TextOut(hDCBack, 200, 205, str, lstrlen(str));

		SelectObject(hDCBack, blackpen);
		SelectObject(hDCBack, whitebrush);
		Rectangle(hDCBack, 10, 225, 390, 240);

		SelectObject(hDCBack, invisiblepen);
		SelectObject(hDCBack, bluebrush);
		Rectangle(hDCBack, 11, 227, 11 + (((curfilenext << 16) / maxfilenextdisp * 379) >> 16), 239);

		// GPU Proc
		wsprintf(str, TEXT("GPU Summary :"));
		TextOut(hDCBack, 10, 250, str, lstrlen(str));
		for (i = 0; i < cudacount; i++){
			cudaSetDevice(i);
			cudaMemGetInfo(&gpumemfree[i], &gpumemtotal[i]);
			wsprintf(str, TEXT("GPU%d : %hs"), i + 1, prop[i].name);
			TextOut(hDCBack, 10, 270 + i * 65, str, lstrlen(str));
			wsprintf(str, TEXT("RAM Clock : %dMHz"), prop[i].memoryClockRate / 1000);
			TextOut(hDCBack, 10, 270 + i * 65 + 20, str, lstrlen(str));
			wsprintf(str, TEXT("Core Clock : %dMHz"), prop[i].clockRate / 1000);
			TextOut(hDCBack, 193, 270 + i * 65 + 20, str, lstrlen(str));
			percent[i] = ((gpumemtotal[i] >> 20) - (gpumemfree[i] >> 20)) * 100 / (gpumemtotal[i] >> 20);
			wsprintf(str, TEXT("Memory : %d%% %d"), percent[i], gpumemtotal[i] - gpumemfree[i]);
			TextOut(hDCBack, 10, 270 + i * 65 + 40, str, lstrlen(str));
			wsprintf(str, TEXT("Files : %d"), gpuproc[i]);
			TextOut(hDCBack, 263, 270 + i * 65 + 40, str, lstrlen(str));
			//if(percent[i] > 96)
			//programend = 1;
		}

		InvalidateRect(hWnd, NULL, FALSE);

		if (programend)
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			*/
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		//BitBlt(hDC, 0, 0, sPos.right - sPos.left, sPos.bottom - sPos.top, hDCBack, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		//CleanUp();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}