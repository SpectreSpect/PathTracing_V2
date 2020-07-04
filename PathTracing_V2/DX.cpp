#include "DX.h"

BOOL DX::ApplicationRun = TRUE;
ID3D11Device* DX::device;
ID3D11DeviceContext* DX::deviceCon;
IDXGISwapChain* DX::swapChain;
ID3D11RenderTargetView* DX::backRenderTargetView;
ID3D11Texture2D* DX::backBufferTexture;
FLOAT DX::screenResolutionWidth = 1920;
FLOAT DX::screenResolutionHeight = 1080;
HWND DX::currentWindow;
DX::~DX()
{
	backBufferTexture->Release();
}
bool DX::Init(HWND OutputWindow, UINT backBufferWidth, UINT backBufferHeight)
{
	DXGI_SWAP_CHAIN_DESC saidChainDesc{};
	saidChainDesc.BufferCount = 1;
	saidChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//saidChainDesc.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	saidChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	saidChainDesc.BufferDesc.Width = backBufferWidth;
	saidChainDesc.BufferDesc.Height = backBufferHeight;
	saidChainDesc.OutputWindow = OutputWindow;
	saidChainDesc.SampleDesc.Count = 1;
	saidChainDesc.Windowed = TRUE;
	saidChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	saidChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&saidChainDesc,
		&DX::swapChain,
		&DX::device,
		NULL,
		&DX::deviceCon
	);
	DX::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	DX::device->CreateRenderTargetView(backBufferTexture, NULL, &DX::backRenderTargetView);
	DX::deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = backBufferWidth;
	viewport.Height = backBufferHeight;
	DX::deviceCon->RSSetViewports(1, &viewport);
	return false;
}

int DX::multipleTo(float Value, float multNum)
{
	float div = Value / multNum;
	int result = (int(div) + 1) * multNum;
	return result;
}

void DX::ConfineCursor(BOOL state)
{
	if (state == TRUE)
	{
		RECT clientRect;
		GetClientRect(DX::currentWindow, &clientRect);
		MapWindowPoints(DX::currentWindow, nullptr, reinterpret_cast<POINT*>(&clientRect), 2);
		ClipCursor(&clientRect);
	}
	else
	{
		ClipCursor(nullptr);
	}

}
