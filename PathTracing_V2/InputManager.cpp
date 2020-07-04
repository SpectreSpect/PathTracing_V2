#include "InputManager.h"

std::vector<char> InputManager::rawBuffer;
UINT InputManager::VKey;
UINT InputManager::MakeCode;
UINT InputManager::KeyState;
UINT InputManager::Message;
FLOAT InputManager::MouseX;
FLOAT InputManager::MouseY;
void InputManager::HandleRawInputMessage(MSG* msg)
{
	std::wostringstream ost;
	std::wstring str;
	if (msg->message == WM_INPUT)
	{
		UINT size;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(msg->lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
		rawBuffer.resize(size);
		GetRawInputData(reinterpret_cast<HRAWINPUT>(msg->lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER));

		RAWINPUT& ri = reinterpret_cast<RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE)
		{
			if (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)
			{
				MouseX = ri.data.mouse.lLastX;
				MouseY = ri.data.mouse.lLastY;
			}

		}

		//OutputDebugString(L"ALL \n");
		if (ri.header.dwType == RIM_TYPEKEYBOARD)
		{

			VKey = ri.data.keyboard.VKey;
			MakeCode = ri.data.keyboard.MakeCode;
			KeyState = ri.data.keyboard.Flags;
			Message = ri.data.keyboard.Message;
			ri.data.keyboard.ExtraInformation;
		}
	}

}

void InputManager::RegisterRawInput()
{
	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x06;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = NULL;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x02;
	rid[1].dwFlags = 0;
	rid[1].hwndTarget = NULL;
	RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
}

void InputManager::ClearInputData()
{
	InputManager::MouseX = 0;
	InputManager::MouseY = 0;
}
