#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT renderTargetWidth, UINT renderTargetHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// описание render target (цели рендера)
	textureDesc.Width = renderTargetWidth;
	textureDesc.Height = renderTargetHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Создание текстуры
	HRESULT result = DX::device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
	if (FAILED(result))
		DX::ApplicationRun = FALSE;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	// Описание render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Создание render target view.
	result = DX::device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
	if (FAILED(result))
		DX::ApplicationRun = FALSE;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Описание shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Создание shader resource view.

	result = DX::device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
	if (FAILED(result))
		DX::ApplicationRun = FALSE;
}

RenderTarget::~RenderTarget()
{
	renderTargetTexture->Release();
	renderTargetView->Release();
	shaderResourceView->Release();
}
