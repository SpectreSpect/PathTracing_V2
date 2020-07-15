#pragma once
#include <d3d11.h>
class Buffer
{
public:
	Buffer();
	~Buffer();
	Buffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags);
	HRESULT InitBuffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags);
	void InitBuffer_UAVorSRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	void InitBuffer_UAV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	void InitBuffer_SRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	ID3D11Buffer* pBuf;
};

