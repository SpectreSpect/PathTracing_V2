#pragma once
#include <d3d11.h>
class Buffer
{
public:
	Buffer();
	~Buffer();
	Buffer(ID3D11Device* device, void* pData, UINT ByteWidth, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride);
	void InitBuffer_UAV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride);
	void InitBuffer_SRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride);
	ID3D11Buffer* pBuffer;
};

