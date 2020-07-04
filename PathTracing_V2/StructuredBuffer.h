#pragma once
#include <d3d11.h>
class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();
	void Release(IUnknown* unknown);
	HRESULT InitSBuffer(ID3D11Device* device, const UINT StructureByteStride, const UINT ByteWidth);
	HRESULT InitSBShaderResource(ID3D11Device* device, const UINT ElementWidth, const UINT NumElements, const UINT ElementOffset = 0, const UINT FirstElement = 0);
	HRESULT UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth);
	ID3D11Buffer* pSBuffer = nullptr;
	ID3D11ShaderResourceView* pSBShaderResource = nullptr;
};

