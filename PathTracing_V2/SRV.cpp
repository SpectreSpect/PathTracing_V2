#include "SRV.h"

SRV::SRV()
{
	pSRV = nullptr;
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
