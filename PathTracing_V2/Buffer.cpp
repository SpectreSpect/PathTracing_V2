#include "Buffer.h"

Buffer::Buffer()
{
	pBuf = nullptr;
}

Buffer::~Buffer()
{
	if (pBuf)
		pBuf->Release();
}

Buffer::Buffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags)
{

	if (pData != nullptr) 
	{
		if(FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage, BindFlags, CPUAccessFlags, MiscFlags)))
		MessageBox(nullptr, L"CreateBuffer() with pSysMem is failed", L"Buffer::Buffer()", MB_ICONERROR);
	}
	else
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage, BindFlags, CPUAccessFlags, MiscFlags)))
			MessageBox(nullptr, L"CreateBuffer() is failed", L"Buffer::Buffer()", MB_ICONERROR);
	}
}

HRESULT Buffer::InitBuffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags)
{
	if (pBuf)
		pBuf->Release();
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = ByteWidth;
	bufferDesc.Usage = Usage;
	bufferDesc.BindFlags = BindFlags;
	bufferDesc.CPUAccessFlags = CPUAccessFlags;
	bufferDesc.MiscFlags = MiscFlags;
	bufferDesc.StructureByteStride = StructureByteStride;
	if (pData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = pData;
		return device->CreateBuffer(&bufferDesc, &subData, &pBuf);
	}
	else
	{
		return device->CreateBuffer(&bufferDesc, 0, &pBuf);
	}
}

void Buffer::InitBuffer_UAVorSRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage)
{

	if (pData) 
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage, 
				              D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 
			                  CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer() with pSysData is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}
	else 
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage,
			D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
			CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer()  is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}

}

void Buffer::InitBuffer_UAV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage)
{
	if (pData)
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage,
			D3D11_BIND_UNORDERED_ACCESS,
			CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer() with pSysData is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}
	else
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage,
			D3D11_BIND_UNORDERED_ACCESS,
			CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer()  is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}
}

void Buffer::InitBuffer_SRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage)
{
	if (pData)
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage,
			D3D11_BIND_SHADER_RESOURCE,
			CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer() with pSysData is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}
	else
	{
		if (FAILED(InitBuffer(device, pData, ByteWidth, StructureByteStride, Usage,
			D3D11_BIND_SHADER_RESOURCE,
			CPUAccessFlags, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)))
			MessageBox(nullptr, L"CreateBuffer()  is failed", L"UnorderedAccessView::InitBuffer_UAV()", MB_ICONERROR);
	}
}
