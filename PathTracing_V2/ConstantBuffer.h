#pragma once
#include <d3d11.h>
#include "DX.h"
#include "Buffer.h"
class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer();
	ConstantBuffer(ID3D11Device* device, const void* pSysMem, const UINT ByteWidth, const D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, const UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	~ConstantBuffer();
	void Map(D3D11_MAP MapType = D3D11_MAP_WRITE_DISCARD);
	size_t CopyMem(const void* data, size_t dataSize, size_t offset = 0);
	void CopyMemOnce(const void* data, const size_t dataSize);
	void UnMap();
	HRESULT Init(ID3D11Device* device, const void* pData, const UINT ByteWidth, const D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, const UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	ID3D11Buffer* pBuf;

private:
	size_t currentOffset;
	void* mappedAddress;
};

