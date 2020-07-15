#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView(ID3D11Resource* pResource, DXGI_FORMAT Format, D3D11_SRV_DIMENSION ViewDimension, UINT MostDetailedMip, UINT MipLevels)
{
	LoadData(pResource, Format, ViewDimension, MostDetailedMip, MipLevels);
}

ShaderResourceView::ShaderResourceView(const wchar_t* path)
{
	LoadData(path);
}
ShaderResourceView::ShaderResourceView(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc)
{
	LoadData(pResource, shaderResourceViewDesc);
}

D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView::GetShaderResourceViewDesc(DXGI_FORMAT Format, D3D11_SRV_DIMENSION ViewDimension, UINT MostDetailedMip, UINT MipLevels)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = Format;
	shaderResourceViewDesc.ViewDimension = ViewDimension;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = MostDetailedMip;
	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;
	return shaderResourceViewDesc;
}

ShaderResourceView::~ShaderResourceView()
{
	Release();
}

void ShaderResourceView::Release()
{
	if (shaderResourceView != nullptr)
		shaderResourceView->Release();
}

void ShaderResourceView::LoadData(ID3D11Resource* pResource, DXGI_FORMAT Format, D3D11_SRV_DIMENSION ViewDimension, UINT MostDetailedMip, UINT MipLevels)
{
	Release();
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = Format;
	shaderResourceViewDesc.ViewDimension = ViewDimension;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = MostDetailedMip;
	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;
	DX::device->CreateShaderResourceView(pResource, &shaderResourceViewDesc, &shaderResourceView);
}

void ShaderResourceView::LoadData(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc)
{
	Release();
	DX::device->CreateShaderResourceView(pResource, &shaderResourceViewDesc, &shaderResourceView);
}

void ShaderResourceView::LoadData(const wchar_t* path)
{
	Release();
	HRESULT hr;
	hr = DirectX::CreateWICTextureFromFile(DX::device, path, nullptr, &shaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView::LoadData() is failed!", L"RayTracing.cpp", MB_ICONHAND);
		DX::ApplicationRun = FALSE;
	}
}

void ShaderResourceView::LoadData(const wchar_t* path, ID3D11Resource* resource)
{
	Release();
	HRESULT hr;
	hr = DirectX::CreateWICTextureFromFile(DX::device, path, &resource, &shaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView::LoadData() is failed!", L"RayTracing.cpp", MB_ICONHAND);
		DX::ApplicationRun = FALSE;
	}
}


void ShaderResourceView::LoadToShaderResource(ID3D11ShaderResourceView** pShaderResource, const wchar_t* path)
{
	HRESULT hr;
	hr = DirectX::CreateWICTextureFromFile(DX::device, path, nullptr, pShaderResource);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView::LoadToShaderResource() is failed!", L"ShaderResourceView.cpp", MB_ICONHAND);
		DX::ApplicationRun = FALSE;
	}
}
