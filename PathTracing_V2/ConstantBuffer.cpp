#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
	bufferType = Buffer::BufferType::CONSTANT_BUFFER;
	pBuf = nullptr;
}

ConstantBuffer::ConstantBuffer(ID3D11Device* device, const void* pSysMem, const UINT ByteWidth, const D3D11_USAGE Usage, const UINT CPUAccessFlags)
{
	bufferType = Buffer::BufferType::CONSTANT_BUFFER;
	pBuf = nullptr;
	Init(device, pSysMem, ByteWidth, Usage, CPUAccessFlags);
}

ConstantBuffer::~ConstantBuffer()
{
	if (pBuf != nullptr)
		pBuf->Release();
}


void ConstantBuffer::Map(D3D11_MAP MapType)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub = {};
	DX::deviceCon->Map(pBuf, 0, MapType, 0, &mappedSub);
	mappedAddress = mappedSub.pData;
}

size_t ConstantBuffer::CopyMem(const void* data, size_t dataSize, size_t offset)
{
	memcpy((void*)((size_t)mappedAddress + offset), data, dataSize);
	currentOffset = offset + dataSize;
	return currentOffset;
}

void ConstantBuffer::CopyMemOnce(const void* data, const size_t dataSize)
{
	Map();
	memcpy(mappedAddress, data, dataSize);
	UnMap();
}

void ConstantBuffer::UnMap()
{
	DX::deviceCon->Unmap(pBuf, 0);
}

HRESULT ConstantBuffer::Init(ID3D11Device* device, const void* pData, const UINT ByteWidth, const D3D11_USAGE Usage, const UINT CPUAccessFlags)
{
	if (pBuf)
		pBuf->Release();
	D3D11_BUFFER_DESC constantBuffer_Desc{};
	constantBuffer_Desc.ByteWidth = ByteWidth;
	constantBuffer_Desc.Usage = Usage;
	constantBuffer_Desc.CPUAccessFlags = CPUAccessFlags;
	constantBuffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (pData)
	{
		D3D11_SUBRESOURCE_DATA constantSubData{};
		constantSubData.pSysMem = pData;
		return device->CreateBuffer(&constantBuffer_Desc, &constantSubData, &pBuf);
	}
	else 
	{
		return device->CreateBuffer(&constantBuffer_Desc, NULL, &pBuf);
	}
}
