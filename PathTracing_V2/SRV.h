#pragma once
#include <d3d11.h>
#include "StructuredBuffer.h"
class SRV
{
public:
	SRV();
	~SRV();
	HRESULT Init(ID3D11Device* device, Buffer* buffer);
	ID3D11ShaderResourceView* pSRV;
};