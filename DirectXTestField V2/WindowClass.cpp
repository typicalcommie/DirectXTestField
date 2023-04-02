#include "WindowClass.h"

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

void Window::RegisterWndClass(LPCWSTR classname)
{
	WNDCLASS wnd = { 0 };
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	GetModuleFileName(wnd.hInstance, 0, 0);
	wnd.lpfnWndProc = WinProc;
	wnd.lpszClassName = classname;

	RegisterClass(&wnd);
}

void Window::Initialization(LPCWSTR classname, LPCWSTR windowname)
{
	RegisterWndClass(classname);

	screenSizeX = GetSystemMetrics(SM_CXFULLSCREEN);	screenSizeY = GetSystemMetrics(SM_CYFULLSCREEN);
	screenSizeX /= 1.5; screenSizeY /= 1.5;
	hwnd = CreateWindow(classname, windowname, WS_VISIBLE | WS_OVERLAPPEDWINDOW, screenSizeX / 2, screenSizeY / 2, screenSizeX, screenSizeY, 0, 0, 0, 0);
}

bool Window::MessageLoop()
{

	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}
	return true;
}