#include "MainWindow.h"
#include "DX.h"
#include "Shader.h"
#include "Shape.h"
#include "ColoredVerticesShader.h"
#include "ShaderRayTracing.h"
#include <vector>
#include "RayTracing.h"
#include "NewRayTracing.h"
#include "Object_PT.h"
#include "TestingComputeShader.h"
#include <iostream>
#include <sstream>
#include "Timer.h"
#include <algorithm>
#include <Winuser.h>
#include "Sphere_PT.h"
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
	DX::windowState = &win->windowState;

	MSG msg{};
	FLOAT color[4] = { 0, 0, 0, 0 };

	struct VERTEX
	{
		FLOAT x, y, z;
		FLOAT color[4];
	};
	InputManager::RegisterRawInput();
	WINDOWINFO winInfo;
	//RayTracing rayTracing;
	//NewRayTracing newRayTracing(DX::device, objects, DX::screenResolutionWidth, DX::screenResolutionHeight);
	std::vector<Object_PT*> objects;
	Sphere_PT someSphere(objects, 0.5f, float3{ -0.7f, 0, 1 }, float4{0.4f, 0.7f, 1, 1});
	Sphere_PT someSphere1(objects, 0.5f, float3{ 0, 2, 3 }, float4{1, 0, 0.5f, 1});
	TestingComputeShader test(DX::device, objects);
	while (DX::ApplicationRun == TRUE)
	{
		Timer::deltaTime = Timer::GetMilisecondsElapsed();
		Timer::Restart();
		if (CheckMessage(&msg) == TRUE)
			break;
		GetWindowInfo(DX::currentWindow, &winInfo);
		//if (SetActiveWindow(DX::currentWindow) == NULL)
		//	break;
		DX::screenResolutionWidth = winInfo.rcClient.right - winInfo.rcClient.left;
		DX::screenResolutionHeight = winInfo.rcClient.bottom - winInfo.rcClient.top;
		DX::deviceCon->ClearRenderTargetView(DX::backRenderTargetView, color);
		test.Draw(DX::deviceCon);
		//rayTracing.Draw();
		//newRayTracing.Draw(DX::deviceCon, DX::screenResolutionWidth, DX::screenResolutionHeight,  win->leftButtomState, win->windowState);
		InputManager::ClearInputData();
		DX::swapChain->Present(0, 0);
	}
	delete directX;
	return 0;
}