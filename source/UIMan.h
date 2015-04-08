// UI Manager

#pragma once

typedef struct _BUTTON{
	int x1;
	int y1;
	int x2;
	int y2;
	TCHAR text[20];
	bool selected;
} BUTTON;

typedef struct _BOX{
	int x1;
	int y1;
	int x2;
	int y2;
} BOX;

typedef struct _UITEXT{
	int x;
	int y;
	TCHAR text[20];
} UITEXT;

class Point{
public:
	int x;
	int y;

	Point() { x = 0; y = 0; }
	Point(int x, int y){ this->x = x; this->y = y; }

	bool In(BOX *rect){ return (x >= rect->x1 && x < rect->x2 && y >= rect->y1 && y < rect->y2); }
};


class UIMan
{
	BUTTON	*m_buttons;
	int		m_numbuttons;
	BOX		*m_boxes;
	int		m_numboxes; 
	UITEXT	*m_texts;
	int		m_numtexts;
	TCHAR	m_restexts[4][10];
	int		m_numrestexts;

	RECT	m_winRect;
	HWND	m_hWnd;
	HDC		m_hDCBack;
	HBITMAP m_hBitmap;
	HBRUSH	m_bluebrush;
	HBRUSH	m_bgbrush;
	HBRUSH	m_whitebrush;
	HBRUSH	m_invisiblebrush;
	HBRUSH	m_blackbrush;
	HPEN	m_blackpen;
	HPEN	m_graypen;
	HPEN	m_invisiblepen;
	HFONT	m_font;

	bool	m_inprocess;
	int		m_selectedres;

	TCHAR	m_inputfile[256];
	TCHAR	m_outputfile[256];
	TCHAR	m_subfile[256];
	TCHAR	m_cutfile[256];

public:
	enum{
		MOUSEDOWN,
		MOUSEMOVE,
		MOUSEUP
	};

	UIMan() {}
	~UIMan() {}

	void Initialize(HWND hWnd, RECT *winRect);
	void Cleanup();

	void Render();
	void ProcessInput(Point &pt, int mouseevent);

	HDC getDC() { return m_hDCBack; }
	int getWidth() { return m_winRect.right - m_winRect.left; }
	int getHeight() { return m_winRect.bottom - m_winRect.top; }
};