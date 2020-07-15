#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer()
{
	bufferType = Buffer::BufferType::STRUCTURED_BUFFER;
	ElementWidth = 0;
	NumElements = 0;
	FirstElement = 0;
	ElementOffset = 0;
}

StructuredBuffer::~StructuredBuffer()
{
	Release(pBuf);
}
HRESULT StructuredBuffer::Init(ID3D11Device* device, void* pData, UINT StructureByteStride, UINT ByteWidth, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage)
{
	Release(pBuf);
	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = BindFlags;
	sbDesc.CPUAccessFlags = CPUAccessFlags;
	sbDesc.Usage = Usage;
	sbDesc.StructureByteStride = this->ElementWidth = StructureByteStride;
	sbDesc.ByteWidth = ByteWidth;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	this->NumElements = ByteWidth / StructureByteStride;
	this->ElementOffset = 0;
	this->FirstElement = 0;

	if (pData)
	{
		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = pData;
		return device->CreateBuffer(&sbDesc, &subData, &pBuf);
	}
	else
	{
		return device->CreateBuffer(&sbDesc, NULL, &pBuf);
	}
}

HRESULT StructuredBuffer::UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub{};
	HRESULT hr = deviceCon->Map(pBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, pData, dataWidth);
	deviceCon->Unmap(pBuf, 0);
	return hr;
}
void StructuredBuffer::Release(IUnknown* unknown)
{
	if (unknown)
		unknown->Release();
}




