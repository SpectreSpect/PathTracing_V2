#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer()
{
}

StructuredBuffer::~StructuredBuffer()
{
	Release(pBuf_test);
	Release(pSBShaderResource);
}

HRESULT StructuredBuffer::InitSBuffer(ID3D11Device* device, void* pData, const UINT StructureByteStride, const UINT ByteWidth)
{
	return Init_Test(device, pData, StructureByteStride, ByteWidth, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

HRESULT StructuredBuffer::InitSBShaderResource(ID3D11Device* device, const UINT ElementWidth, const UINT NumElements, const UINT ElementOffset, const UINT FirstElement)
{
	Release(pSBShaderResource);
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.ElementOffset = this->ElementOffset = ElementOffset;
	shaderResourceViewDesc.Buffer.ElementWidth = this->ElementWidth = ElementWidth;
	shaderResourceViewDesc.Buffer.FirstElement = this->FirstElement = FirstElement;
	shaderResourceViewDesc.Buffer.NumElements = this->NumElements = NumElements;
	HRESULT hr = device->CreateShaderResourceView(pBuf_test, &shaderResourceViewDesc, &pSBShaderResource);
	return hr;
}

HRESULT StructuredBuffer::UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub{};
	HRESULT hr = deviceCon->Map(pBuf_test, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, pData, dataWidth);
	deviceCon->Unmap(pBuf_test, 0);
	return hr;
}
void StructuredBuffer::Release(IUnknown* unknown)
{
	if (unknown)
		unknown->Release();
}

HRESULT StructuredBuffer::Init_Test(ID3D11Device* device, void* pData, UINT StructureByteStride, UINT ByteWidth, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage)
{
	bufferType = Buffer::BufferType::STRUCTURED_BUFFER;
	Release(pBuf_test);
	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = BindFlags;
	sbDesc.CPUAccessFlags = CPUAccessFlags;
	sbDesc.Usage = Usage;
	sbDesc.StructureByteStride = StructureByteStride;
	sbDesc.ByteWidth = ByteWidth;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	if (pData)
	{
		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = pData;
		return device->CreateBuffer(&sbDesc, &subData, &pBuf_test);
	}
	else
	{
		return device->CreateBuffer(&sbDesc, NULL, &pBuf_test);
	}
}


