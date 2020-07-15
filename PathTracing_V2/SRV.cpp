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
	if (buffer->bufferType == Buffer::BufferType::STRUCTURED_BUFFER)
	{
		StructuredBuffer* structuredBuffer = reinterpret_cast<StructuredBuffer*>(buffer);
		srv_Desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv_Desc.Buffer.ElementWidth = structuredBuffer->ElementWidth;
		srv_Desc.Buffer.NumElements = structuredBuffer->NumElements;
		srv_Desc.Buffer.FirstElement = structuredBuffer->FirstElement;
		srv_Desc.Buffer.ElementOffset = structuredBuffer->ElementOffset;
		return device->CreateShaderResourceView(structuredBuffer->pBuf, &srv_Desc, &pSRV);
	}
}

//void SRV::Init_Buffered(ID3D11Device* device, UINT ElementWidth, UINT NumElements, UINT FirstElement, UINT ElementOffset)
//{
//	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc{};
//	srv_Desc.Format = DXGI_FORMAT_UNKNOWN;
//	srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//	srv_Desc.Buffer.ElementWidth = ElementWidth;
//	srv_Desc.Buffer.NumElements = NumElements;
//	srv_Desc.Buffer.FirstElement = FirstElement;
//	srv_Desc.Buffer.ElementOffset = ElementOffset;
//
//	if (FAILED(device->CreateShaderResourceView(ray_Buffer->pBuf, &ray_SRV_Desc, pSRV)))
//		MessageBox(nullptr, L"CreateShaderResourceView() for (ray_SRV) is failed", L"TestingComputeShader::InitRayGenerator()", MB_ICONERROR);
//}
