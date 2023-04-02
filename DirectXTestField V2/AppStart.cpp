#include "General.h"
#include "WindowClass.h"
#include "Graphics.h"

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR command, int count)
{
	Window window;
	GraphicsClass Graphics;
	window.Initialization(L"DirectXTF", L"DirectXTestField");
	Graphics.SetScreenSize(window.screenSizeX, window.screenSizeY);
	Graphics.SetHWND(&window.hwnd);
	Graphics.Initialization();

	while (window.working)
	{
		if (!window.MessageLoop())
		{
			window.working = false;
		}

		else if (!Graphics.FrameProc())
		{
			window.working = false;
		}
	}

	MessageBox(0, L"App is stop working. |=_=|", 0, 0);

	return 0;
}
