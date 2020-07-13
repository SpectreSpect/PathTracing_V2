#include "Buffer.h"

Buffer::Buffer()
{
	pBuffer = nullptr;
}

Buffer::~Buffer()
{
	if (pBuffer)
		pBuffer->Release();
}

Buffer::Buffer(ID3D11Device* device, void* pData, UINT ByteWidth, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = ByteWidth;
	bufferDesc.Usage = Usage;
	bufferDesc.BindFlags = BindFlags;
	bufferDesc.CPUAccessFlags = CPUAccessFlags;
	bufferDesc.MiscFlags = MiscFlags;
	bufferDesc.StructureByteStride = StructureByteStride;
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = pData;
	if (FAILED(device->CreateBuffer(&bufferDesc, &subData, &pBuffer)))
		MessageBox(nullptr, L"CreateBuffer() is failed", L"Buffer::Buffer()", MB_ICONERROR);
}

void Buffer::InitBuffer_UAV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride)
{
	CD3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = ByteWidth;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = StructureByteStride;
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = pData;
	if (FAILED(device->CreateBuffer(&bufferDesc, &subData, &pBuffer)))
		MessageBox(nullptr, L"CreateBuffer() is failed", L"UnorderedAccessView::Init()", MB_ICONERROR);
}

