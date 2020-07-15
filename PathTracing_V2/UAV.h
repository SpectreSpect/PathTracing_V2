#pragma once
#include <d3d11.h>
#include "Buffer.h"
#include "StructuredBuffer.h"
#include "Texture2D.h"
class UAV
{
public:
	UAV();
	HRESULT Init(ID3D11Device* device, Buffer* buffer);
	ID3D11UnorderedAccessView* pUAV;
};