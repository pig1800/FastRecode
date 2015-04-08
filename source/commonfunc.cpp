#include "stdafx.h"

void Initialize(HWND hWnd, RECT *winRect){
	g_uiMan.Initialize(hWnd, winRect);
}

void Cleanup(){
	g_uiMan.Cleanup();
}

