#pragma once
#include <d3d11.h>
#include "Buffer.h"
class Texture2D : public Buffer
{
public:
	Texture2D();
	HRESULT Init(ID3D11Device* device, void* pData, UINT Width, UINT Height, DXGI_FORMAT Format, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage,
		UINT MiscFlags, UINT MipLevels, UINT ArraySize, UINT SampleDesc_Count);
	ID3D11Texture2D* pBuf;
	DXGI_FORMAT Format;
	UINT MipLevels;
};