#pragma once
#include <d3d11.h>
class Buffer
{
public:
	enum class BufferType
	{
		STRUCTURED_BUFFER = 0,
		TEXTURE2D = 1,
		CONSTANT_BUFFER = 2
	};
	Buffer();
	~Buffer();
	Buffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags);
	HRESULT InitBuffer(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags);
	void InitBuffer_UAVorSRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	void InitBuffer_UAV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	void InitBuffer_SRV(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride, UINT CPUAccessFlags, D3D11_USAGE Usage);
	BufferType bufferType;
	ID3D11Buffer* pBuf_Temp;
};

