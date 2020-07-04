#pragma once
#include <d3d11.h>
class Shader;
class ConstantBuffer
{
public:
	ConstantBuffer(const void* pSysMem, UINT ByteWidth, D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	~ConstantBuffer();
	void Release();
	void LoadData(const void* pSysMem, UINT ByteWidth, D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	void Set(UINT StartSlot = 0, UINT NumBuffers = 1);
	void Map(D3D11_MAP MapType = D3D11_MAP_WRITE_DISCARD);
	size_t CopyMem(const void* data, size_t dataSize, size_t offset = 0);
	void UnMap();
	ID3D11Buffer* pConstantBuffer;
	D3D11_BUFFER_DESC constantBufferDesc{};
	D3D11_SUBRESOURCE_DATA constantSubData{};
	size_t currentOffset;
private:
	BOOL mapped;
	D3D11_MAPPED_SUBRESOURCE mappedSub;
};

