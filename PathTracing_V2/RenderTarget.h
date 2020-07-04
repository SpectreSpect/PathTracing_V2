#pragma once
#include "DX.h"
class RenderTarget
{
public:
	RenderTarget(UINT renderTargetWidth, UINT renderTargetHeight);
	~RenderTarget();
	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
};

