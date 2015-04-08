#include "stdafx.h"

void UIMan::Initialize(HWND hWnd, RECT *winRect){
	BUTTON buttons[] =
	{
		{ 432, 10, 492, 28, TEXT("Open..."), 0 },
		{ 432, 30, 492, 48, TEXT("Open..."), 0 },
		{ 432, 50, 492, 68, TEXT("Open..."), 0 },
		{ 432, 70, 492, 88, TEXT("Open..."), 0 },
		{ 140, 330, 200, 348, TEXT("Start"), 0 },
		{ 300, 330, 360, 348, TEXT("Exit"), 0 },
		{ 220, 330, 280, 348, TEXT("Stop"), 0 }
	};

	m_buttons = (BUTTON *)malloc(sizeof(buttons));
	m_numbuttons = sizeof(buttons) / sizeof(BUTTON);
	memcpy(m_buttons, &buttons, sizeof(buttons));

	BOX boxes[] = 
	{
		{ 90, 10 - 1, 420, 28 },
		{ 90, 30 - 1, 420, 48 },
		{ 90, 50 - 1, 420, 68 },
		{ 90, 70 - 1, 420, 88 },
		{ 90, 89, 158, 170 },
		{ 100, 192, 400, 207 },
		{ 100, 212, 400, 227 },
		{ 100, 232, 400, 247 },
		{ 100, 252, 400, 267 },
		{ 100, 272, 400, 287 },
		{ 100, 290, 400, 307 }
	};
	m_boxes = (BOX *)malloc(sizeof(boxes));
	m_numboxes = sizeof(boxes) / sizeof(BOX);
	memcpy(m_boxes, &boxes, sizeof(boxes));

	UITEXT texts[] = 
	{
		{ 10, 10, TEXT("Source") },
		{ 10, 30, TEXT("Destination") },
		{ 10, 50, TEXT("Subtitle") },
		{ 10, 70, TEXT("Cut List") },
		{ 10, 90, TEXT("Resolution") },
		{ 10, 170, TEXT("Progress") },
		{ 10, 190, TEXT("Read") },
		{ 10, 210, TEXT("Demux") },
		{ 10, 230, TEXT("Video Encode") },
		{ 10, 250, TEXT("Audio Encode") },
		{ 10, 270, TEXT("Mux") },
		{ 10, 290, TEXT("Write") }
	};
	m_texts = (UITEXT *)malloc(sizeof(texts));
	m_numtexts = sizeof(texts) / sizeof(UITEXT);
	memcpy(m_texts, &texts, sizeof(texts));

	TCHAR restexts[4][10] = {
		TEXT("640x360"),
		TEXT("848x480"),
		TEXT("1280x720"),
		TEXT("1920x1080")
	};
	m_numrestexts = 4;
	memcpy(m_restexts, &restexts, sizeof(restexts));

	ZeroMemory(m_inputfile, sizeof(TCHAR) * 256);
	ZeroMemory(m_outputfile, sizeof(TCHAR) * 256);
	ZeroMemory(m_subfile, sizeof(TCHAR) * 256);
	ZeroMemory(m_cutfile, sizeof(TCHAR) * 256);

	m_inprocess = false;
	m_selectedres = 0;
	
	HDC hDC;

	m_hWnd = hWnd;
	memcpy(&m_winRect, winRect, sizeof(RECT));
	m_blackpen = CreatePen(PS_SOLID, 1, 0);
	m_graypen = CreatePen(PS_SOLID, 1, 0xa0a0a0);
	m_invisiblepen = CreatePen(PS_NULL, 0, 0);
	m_bluebrush = CreateSolidBrush(RGB(40, 150, 255));
	m_whitebrush = CreateSolidBrush(0xffffff);
	m_blackbrush = CreateSolidBrush(0);
	m_invisiblebrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	m_bgbrush = CreateSolidBrush(0xf0f0f0);
	m_font = CreateFont(16, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		TEXT("Arial"));

	hDC = GetDC(m_hWnd);
	m_hDCBack = CreateCompatibleDC(hDC);
	m_hBitmap = CreateCompatibleBitmap(hDC, m_winRect.right - m_winRect.left, m_winRect.bottom - m_winRect.top);
	SelectObject(m_hDCBack, m_hBitmap);
	ReleaseDC(m_hWnd, hDC);

	SelectObject(m_hDCBack, m_font);
	SetBkMode(m_hDCBack, TRANSPARENT);

	SetTimer(m_hWnd, 0, 10, NULL);
}

void UIMan::Cleanup(){
	KillTimer(m_hWnd, 0);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hDCBack);
	DeleteObject(m_font);
	DeleteObject(m_blackbrush);
	DeleteObject(m_whitebrush);
	DeleteObject(m_bluebrush);
	DeleteObject(m_invisiblepen);
	DeleteObject(m_blackpen);
	DeleteObject(m_bgbrush);

	free(m_boxes);
	free(m_texts);
	free(m_buttons);
}

void UIMan::Render(){
	int i;

	SelectObject(m_hDCBack, m_invisiblepen);
	SelectObject(m_hDCBack, m_bgbrush);
	Rectangle(m_hDCBack, 0, 0, getWidth(), getHeight());


	SetTextAlign(m_hDCBack, TA_LEFT);
	SetTextColor(m_hDCBack, 0);
	for (i = 0; i < m_numtexts; i++)
		TextOut(m_hDCBack, m_texts[i].x, m_texts[i].y, m_texts[i].text, lstrlen(m_texts[i].text));

	SelectObject(m_hDCBack, m_blackpen);
	SetTextAlign(m_hDCBack, TA_CENTER | TA_TOP);
	for (i = 0; i < m_numbuttons; i++){
		if (!m_buttons[i].selected) SelectObject(m_hDCBack, m_bgbrush);
		else						SelectObject(m_hDCBack, m_bluebrush);

		if (i < m_numbuttons - 1){
			if (m_inprocess){
				SelectObject(m_hDCBack, m_graypen);
				SetTextColor(m_hDCBack, 0xa0a0a0);
			} else {
				SelectObject(m_hDCBack, m_blackpen);
				SetTextColor(m_hDCBack, 0);
			}
		} else {
			if (!m_inprocess){
				SelectObject(m_hDCBack, m_graypen);
				SetTextColor(m_hDCBack, 0xa0a0a0);
			}
			else {
				SelectObject(m_hDCBack, m_blackpen);
				SetTextColor(m_hDCBack, 0);
			}
		}
		
		Rectangle(m_hDCBack, m_buttons[i].x1, m_buttons[i].y1, m_buttons[i].x2, m_buttons[i].y2);
		TextOut(m_hDCBack, m_buttons[i].x1 + (m_buttons[i].x2 - m_buttons[i].x1) / 2, m_buttons[i].y1, m_buttons[i].text, lstrlen(m_buttons[i].text));
	}
		

	SelectObject(m_hDCBack, m_blackpen);
	SelectObject(m_hDCBack, m_whitebrush);
	for (i = 0; i < m_numboxes; i++)
		Rectangle(m_hDCBack, m_boxes[i].x1, m_boxes[i].y1, m_boxes[i].x2, m_boxes[i].y2);

	SetTextColor(m_hDCBack, 0);
	SetTextAlign(m_hDCBack, TA_LEFT);
	RECT textboxes[4];
	memcpy(textboxes, m_boxes, sizeof(BOX) * 4);
	textboxes[0].left += 2;
	textboxes[1].left += 2;
	textboxes[2].left += 2;
	textboxes[3].left += 2;
	textboxes[0].top++;
	textboxes[1].top++;
	textboxes[2].top++;
	textboxes[3].top++;
	DrawText(m_hDCBack, m_inputfile, lstrlen(m_inputfile), (LPRECT)&textboxes[0], NULL);
	DrawText(m_hDCBack, m_outputfile, lstrlen(m_outputfile), (LPRECT)&textboxes[1], NULL);
	DrawText(m_hDCBack, m_subfile, lstrlen(m_subfile), (LPRECT)&textboxes[2], NULL);
	DrawText(m_hDCBack, m_cutfile, lstrlen(m_cutfile), (LPRECT)&textboxes[3], NULL);

	for (i = 0; i < 4; i++){
		if (i == m_selectedres){
			SelectObject(m_hDCBack, m_bluebrush);
			SelectObject(m_hDCBack, m_invisiblepen);
			Rectangle(m_hDCBack, 91, 90 + i * 20, 158, 90 + (i + 1) * 20);
		}
		TextOut(m_hDCBack, 92, 90 + i * 20, m_restexts[i], 10);
	}
	
	InvalidateRect(m_hWnd, NULL, FALSE);
}

void UIMan::ProcessInput(Point &pt, int mouseevent){
	int i;
	OPENFILENAME ofn;

	switch (mouseevent){
	case MOUSEMOVE:
		if (!m_inprocess){
			for (i = 0; i < m_numbuttons - 1; i++){
				if (pt.In((BOX *)&m_buttons[i]))
					m_buttons[i].selected = true;
				else
					m_buttons[i].selected = false;
			}
		}
		
		if (pt.In((BOX *)&m_buttons[m_numbuttons - 1]) && m_inprocess)
			m_buttons[m_numbuttons - 1].selected = true;
		else
			m_buttons[m_numbuttons - 1].selected = false;

		break;

	case MOUSEDOWN:
		for (i = 0; i < m_numbuttons; i++){
			if (m_buttons[i].selected){
				switch (i){
				case 0:				// Open Source
					ZeroMemory(&ofn, sizeof(OPENFILENAME));
					
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = m_hWnd;
					ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
					ofn.lpstrFilter = TEXT("Transport Streams (*.ts, *.m2ts)\0*.ts;*.m2ts\0All Files\0*.*\0\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFile = m_inputfile;
					ofn.nMaxFile = 256;
					ofn.lpstrDefExt = TEXT(".ts");
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					
					GetOpenFileName(&ofn);
					break;

				case 1:				// Open Destination
					ZeroMemory(&ofn, sizeof(OPENFILENAME));

					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = m_hWnd;
					ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
					ofn.lpstrFilter = TEXT("MP4 (*.mp4)\0*.mp4\0All Files\0*.*\0\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFile = m_outputfile;
					ofn.nMaxFile = 256;
					ofn.lpstrDefExt = TEXT(".mp4");
					
					GetOpenFileName(&ofn);
					break;

				case 2:				// Open Sub
					ZeroMemory(&ofn, sizeof(OPENFILENAME));

					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = m_hWnd;
					ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
					ofn.lpstrFilter = TEXT("Subtitles (*.srt, *.ass)\0*.srt;*.ass\0All Files\0*.*\0\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFile = m_subfile;
					ofn.nMaxFile = 256;
					ofn.lpstrDefExt = TEXT(".ass");
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

					GetOpenFileName(&ofn);
					break;

				case 3:				// Open Cut
					ZeroMemory(&ofn, sizeof(OPENFILENAME));

					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = m_hWnd;
					ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
					ofn.lpstrFilter = TEXT("Cut list (*.cut)\0*.cut\0All Files\0*.*\0\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFile = m_cutfile;
					ofn.nMaxFile = 256;
					ofn.lpstrDefExt = TEXT(".cut");
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

					GetOpenFileName(&ofn);
					break;

				case 4:				// Start
					m_inprocess = true;
					break;

				case 5:				// Exit
					SendMessage(m_hWnd, WM_DESTROY, 0, 0);
					break;

				case 6:				// Stop
					m_inprocess = false;
					
					break;
				}
				m_buttons[i].selected = false;
			}
		}

		if (!m_inprocess && pt.In(&m_boxes[4]))
			m_selectedres = (pt.y - (m_boxes[4].y1 + 1)) / 20;
		break;
	}
}