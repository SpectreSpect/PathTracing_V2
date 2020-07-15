#include "SRV.h"

SRV::SRV()
{
	pSRV = nullptr;
}

SRV::~SRV()
{
	if (pSRV)
		pSRV->Release();
}

HRESULT SRV::Init(ID3D11Device* device, Buffer* buffer)
{
	if (pSRV)
		pSRV->Release();
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc{};
	switch (buffer->bufferType)
	{
	case Buffer::BufferType::STRUCTURED_BUFFER:
	{
		StructuredBuffer* structuredBuffer = reinterpret_cast<StructuredBuffer*>(buffer);
		srv_Desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv_Desc.Buffer.ElementWidth = structuredBuffer->ElementWidth;
		srv_Desc.Buffer.NumElements = structuredBuffer->NumElements;
		srv_Desc.Buffer.FirstElement = structuredBuffer->FirstElement;
		srv_Desc.Buffer.ElementOffset = structuredBuffer->ElementOffset;
		return device->CreateShaderResourceView(structuredBuffer->pBuf, &srv_Desc, &pSRV);
	}break;
	case Buffer::BufferType::TEXTURE2D:
	{
		Texture2D* texture2D = reinterpret_cast<Texture2D*>(buffer);
		srv_Desc.Format = texture2D->Format;
		srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_Desc.Texture2D.MostDetailedMip = 0;
		srv_Desc.Texture2D.MipLevels = texture2D->MipLevels;
		return device->CreateShaderResourceView(texture2D->pBuf, &srv_Desc, &pSRV);
	}break;
	}
}
