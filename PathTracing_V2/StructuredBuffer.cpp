#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer()
{
}

StructuredBuffer::~StructuredBuffer()
{
	Release(pSBuffer);
	Release(pSBShaderResource);
}

HRESULT StructuredBuffer::InitSBuffer(ID3D11Device* device, const UINT StructureByteStride, const UINT ByteWidth, void* pData)
{
	Release(pSBuffer);
	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = StructureByteStride;
	sbDesc.ByteWidth = ByteWidth;
	HRESULT hr;
	if (pData) 
	{
		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = pData;
		hr = device->CreateBuffer(&sbDesc, &subData, &pSBuffer);
	}
	else 
	{
		hr = device->CreateBuffer(&sbDesc, 0, &pSBuffer);
	}
	return hr;
}

HRESULT StructuredBuffer::InitSBShaderResource(ID3D11Device* device, const UINT ElementWidth, const UINT NumElements, const UINT ElementOffset, const UINT FirstElement)
{
	Release(pSBShaderResource);
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.ElementOffset = ElementOffset;
	shaderResourceViewDesc.Buffer.ElementWidth = ElementWidth;
	shaderResourceViewDesc.Buffer.FirstElement = FirstElement;
	shaderResourceViewDesc.Buffer.NumElements = NumElements;
	HRESULT hr = device->CreateShaderResourceView(pSBuffer, &shaderResourceViewDesc, &pSBShaderResource);
	return hr;
}

HRESULT StructuredBuffer::UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub{};
	HRESULT hr = deviceCon->Map(pSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, pData, dataWidth);
	deviceCon->Unmap(pSBuffer, 0);
	return hr;
}

void StructuredBuffer::Release(IUnknown* unknown)
{
	if (unknown)
		unknown->Release();
}
