#include "UAV.h"

UAV::UAV()
{
	pUAV = nullptr;
}

HRESULT UAV::Init(ID3D11Device* device, Buffer* buffer)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_Desc{};
	switch (buffer->bufferType)
	{
	case Buffer::BufferType::STRUCTURED_BUFFER: 
	{
		StructuredBuffer* structuredBuffer = reinterpret_cast<StructuredBuffer*>(buffer);
		uav_Desc.Format = DXGI_FORMAT_UNKNOWN;
		uav_Desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_Desc.Buffer.NumElements = structuredBuffer->NumElements;
		uav_Desc.Buffer.FirstElement = structuredBuffer->FirstElement;
		return device->CreateUnorderedAccessView(structuredBuffer->pBuf, &uav_Desc, &pUAV);
	}break;
	case Buffer::BufferType::TEXTURE2D:
	{
		Texture2D* texture2D = reinterpret_cast<Texture2D*>(buffer);
		uav_Desc.Format = texture2D->Format;
		uav_Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		return device->CreateUnorderedAccessView(texture2D->pBuf, &uav_Desc, &pUAV);
	}break;
	}
}
