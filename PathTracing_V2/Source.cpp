#include "MainWindow.h"
#include "DX.h"
#include "Shader.h"
#include "Shape.h"
#include "ColoredVerticesShader.h"
#include "ShaderRayTracing.h"
#include <vector>
#include "RayTracing.h"

#include <iostream>
#include <sstream>
#include "Timer.h"
#include <algorithm>

BOOL CheckMessage(MSG* msg)
{

	while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(msg);
		DispatchMessage(msg);

		if (msg->message == WM_QUIT)
		{
			return TRUE;
		}
		InputManager::HandleRawInputMessage(msg);
	}
}
#define SCREEN_WIDTH 1920 
#define SCREEN_HEIGHT 1080 
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Timer::Init();
	MainWindow* win = new MainWindow();
	win->Create(L"Window", WS_OVERLAPPEDWINDOW, NULL, NULL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	DX* directX = new DX();
	directX->Init(win->hwnd, DX::screenResolutionWidth, DX::screenResolutionHeight);
	ShowWindow(win->hwnd, nCmdShow);
	DX::currentWindow = win->hwnd;
	MSG msg{};
	FLOAT color[4] = { 0, 0, 0, 0 };

	struct VERTEX
	{
		FLOAT x, y, z;
		FLOAT color[4];
	};
	InputManager::RegisterRawInput();
	WINDOWINFO winInfo;
	RayTracing rayTracing;
	while (DX::ApplicationRun == TRUE)
	{
		Timer::deltaTime = Timer::GetMilisecondsElapsed();
		Timer::Restart();
		if (CheckMessage(&msg) == TRUE)
			break;
		GetWindowInfo(DX::currentWindow, &winInfo);
		DX::screenResolutionWidth = winInfo.rcClient.right - winInfo.rcClient.left;
		DX::screenResolutionHeight = winInfo.rcClient.bottom - winInfo.rcClient.top;
		DX::deviceCon->ClearRenderTargetView(DX::backRenderTargetView, color);
		rayTracing.Draw();
		InputManager::ClearInputData();
		DX::swapChain->Present(0, 0);
	}
	delete directX;
	return 0;
}