#pragma once
#include <wincodec.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dCompiler.lib")
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <ScreenGrab.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "InputManager.h"
#include "SamplerState.h"
#include "ShaderResourceView.h"

#define HCheck(HR, TEXT, CAPTION) if(FAILED(HR)){MessageBox(nullptr, TEXT, CAPTION, MB_ICONERROR);}
#define PI 3.14159265358979
class DX
{
public:

	~DX();
	bool Init(HWND OutputWindow, UINT backBufferWidth, UINT backBufferHeight);
	static BOOL ApplicationRun;
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceCon;
	static IDXGISwapChain* swapChain;
	static ID3D11RenderTargetView* backRenderTargetView;
	static ID3D11Texture2D* backBufferTexture;
	static FLOAT screenResolutionWidth;
	static FLOAT screenResolutionHeight;
	static HWND currentWindow;
	static int multipleTo(float Value, float multNum);
	static void ConfineCursor(BOOL state);
};

