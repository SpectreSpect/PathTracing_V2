#pragma once
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <vector>
#define KEY_DOWN 0
#define KEY_UP 1

class InputManager
{
public:
	static void HandleRawInputMessage(MSG* msg);
	static void RegisterRawInput();
	static void ClearInputData();
	static std::vector<char> rawBuffer;
	static FLOAT MouseX;
	static FLOAT MouseY;
	static UINT VKey;
	static UINT MakeCode;
	static UINT KeyState;
	static UINT Message;
};

