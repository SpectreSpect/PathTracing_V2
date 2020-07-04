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


BOOL CheckMessage(MSG* msg)
{

	if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
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
	MainWindow* win = new MainWindow();
	win->Create(L"Window", WS_OVERLAPPEDWINDOW, NULL, NULL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	DX* directX = new DX();
	directX->Init(win->hwnd, DX::screenResolutionWidth, DX::screenResolutionHeight);
	ShowWindow(win->hwnd, nCmdShow);
	DX::currentWindow = win->hwnd;
	MSG msg{};
	FLOAT colorSky[4] = { 0.4f, 0.6f, 0.96f, 1.0f };
	FLOAT colorLightBlack[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	FLOAT color[4] = { 0, 0, 0, 0 };

	struct VERTEX
	{
		FLOAT x, y, z;
		FLOAT color[4];
	};
	VERTEX vertices[] =
	{
		{-1, 1, 0, {0, 0, 0, 1.0f}},
		{1, 1, 0, {0, 0, 0, 1.0f}},
		{1, -1, 0.0f, {0, 0, 0, 1.0f}},
		{-1, -1, 0.0f, {0, 0, 0, 1.0f}}
	};

	//Shape* triangle = new Shape();
	//triangle->shader = new ShaderRayTracing();
	InputManager::RegisterRawInput();
	WINDOWINFO winInfo;
	RayTracing rayTracing;
	//HRESULT hr;
	//UINT kek = D3D11_FORMAT_SUPPORT_VIDEO_ENCODER;
	//hr = DX::device->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT, &kek);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr, L"Format isn't supported", L"TestFormat", MB_ICONHAND);
	//	DX::ApplicationRun = FALSE;
	//}
	while (DX::ApplicationRun == TRUE)
	{
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