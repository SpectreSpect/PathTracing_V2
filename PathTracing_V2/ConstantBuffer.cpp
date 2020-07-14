#include "Shader.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const void* pSysMem, UINT ByteWidth, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	LoadData(pSysMem, ByteWidth, Usage, CPUAccessFlags);
}

ConstantBuffer::~ConstantBuffer()
{
	Release();
}

void ConstantBuffer::Release()
{
	if (pConstantBuffer != nullptr)
		pConstantBuffer->Release();
}
void ConstantBuffer::LoadData(const void* pSysMem, UINT ByteWidth, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	Release();
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.Usage = Usage;
	//constantBufferDesc.ByteWidth = (((int)ByteWidth / 16) + 1) * 16;
	constantBufferDesc.ByteWidth = ByteWidth;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = CPUAccessFlags;
	D3D11_SUBRESOURCE_DATA constantSubData{};
	constantSubData.pSysMem = pSysMem;
	HRESULT hr = DX::device->CreateBuffer(&constantBufferDesc, &constantSubData, &pConstantBuffer);
	if (FAILED(hr)) MessageBox(nullptr, L"device->CreateBuffer() is failed", L"ConstantBuffer::LoadData()", MB_ICONERROR);
}

void ConstantBuffer::Set(UINT StartSlot, UINT NumBuffers)
{
	DX::deviceCon->PSSetConstantBuffers(StartSlot, NumBuffers, &pConstantBuffer);
}

void ConstantBuffer::Map(D3D11_MAP MapType)
{
	mapped = true;
	mappedSub = {};
	DX::deviceCon->Map(pConstantBuffer, 0, MapType, 0, &mappedSub);
}

size_t ConstantBuffer::CopyMem(const void* data, size_t dataSize, size_t offset)
{
	memcpy((void*)((size_t)mappedSub.pData + offset), data, dataSize);
	currentOffset = offset + dataSize;
	return currentOffset;
}

void ConstantBuffer::UnMap()
{
	DX::deviceCon->Unmap(pConstantBuffer, 0);
	mapped = false;

}
