#pragma once
#include <d3d11.h>
#include "Buffer.h"
class StructuredBuffer : public Buffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();
	HRESULT Init(ID3D11Device* device, void* pData, UINT StructureByteStride, UINT ByteWidth, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage);
	void Release(IUnknown* unknown);
	HRESULT UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth);
	UINT ElementWidth;
	UINT NumElements;
	UINT FirstElement;
	UINT ElementOffset;
	ID3D11Buffer* pBuf;
};

