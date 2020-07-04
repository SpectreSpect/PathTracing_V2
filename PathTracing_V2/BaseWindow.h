#pragma once
#include <Windows.h>
template<class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

	}
	BOOL Create
	(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		HINSTANCE hInstance = NULL,
		DWORD dwExStyle = NULL,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL,
		HMENU hMenu = NULL
	)
	{
		winName = lpWindowName;
		WNDCLASS wc = {};
		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.lpszClassName = ClassName();
		wc.lpszMenuName = lpWindowName;
		RegisterClass(&wc);
		hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, this);
		return (hwnd ? TRUE : FALSE);
	}
	HWND hwnd = NULL;
	LPCWSTR winName = NULL;
	virtual LPCWSTR ClassName() = 0;
protected:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};