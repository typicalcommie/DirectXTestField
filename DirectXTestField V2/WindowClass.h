#pragma once
#include <Windows.h>
#include "General.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

class Window
{
public:
	void Initialization(LPCWSTR classname, LPCWSTR windowname);
	void RegisterWndClass(LPCWSTR classname);
	bool MessageLoop();

	int screenSizeX, screenSizeY;
	bool working = true;
	MSG msg;
	HWND hwnd;
	
};