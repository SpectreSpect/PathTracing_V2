#pragma once
#include <d3d11.h>
#include "Buffer.h"
class StructuredBuffer : public Buffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();
	void Release(IUnknown* unknown);
	HRESULT InitSBuffer(ID3D11Device* device, void* pData, const UINT StructureByteStride, const UINT ByteWidth);
	HRESULT InitSBShaderResource(ID3D11Device* device, const UINT ElementWidth, const UINT NumElements, const UINT ElementOffset = 0, const UINT FirstElement = 0);
	HRESULT UploadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth);
	//ID3D11Buffer* pSBuffer = nullptr;
	ID3D11ShaderResourceView* pSBShaderResource = nullptr;


	HRESULT Init_Test(ID3D11Device* device, void* pData, UINT StructureByteStride, UINT ByteWidth, UINT BindFlags, UINT CPUAccessFlags, D3D11_USAGE Usage);
	ID3D11Buffer* pBuf_test = nullptr;
};

