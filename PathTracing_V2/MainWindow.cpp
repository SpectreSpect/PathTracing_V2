#include "MainWindow.h"
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	return 0;
	case WM_ACTIVATE:
	{
		if (wParam == WA_ACTIVE)
		{
			windowState = 1;
			OutputDebugString(L"WM_ACTIVATE\n");
		}
		else if (wParam == WA_CLICKACTIVE)
		{
			windowState = 1;
			OutputDebugString(L"WA_CLICKACTIVE\n");
		}
		else if(wParam == WA_INACTIVE)
		{
			windowState = 0;
			OutputDebugString(L"WA_INACTIVE\n");
		}
	}
	return 0;
	case WM_LBUTTONDOWN:
	{
		leftButtomState = 1;
	}
	return 0;
	case WM_LBUTTONUP:
	{		
		leftButtomState = 0;
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

