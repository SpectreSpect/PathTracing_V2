#pragma once
#include "DX.h"
class ShaderResourceView
{
public:
	ShaderResourceView(
		ID3D11Resource* pResource,
		//DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		D3D11_SRV_DIMENSION ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		UINT MostDetailedMip = 0,
		UINT MipLevels = 1);
	ShaderResourceView(const wchar_t* path);

	ShaderResourceView(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(
		DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		D3D11_SRV_DIMENSION ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		UINT MostDetailedMip = 0,
		UINT MipLevels = 1);


	~ShaderResourceView();
	void Release();
	void LoadData(
		ID3D11Resource* pResource,
		//DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_SRV_DIMENSION ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		UINT MostDetailedMip = 0,
		UINT MipLevels = 1);
	void LoadData(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc);
	void LoadData(const wchar_t* path);
	void LoadData(const wchar_t* path, ID3D11Resource* resource);
	void Set(UINT startSlot = 0, UINT NumViews = 1);
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11Resource* resource;
	static void LoadToShaderResource(ID3D11ShaderResourceView** pResource, const wchar_t* path);
};

