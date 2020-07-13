#include "UnorderedAccessView.h"

UnorderedAccessView::UnorderedAccessView()
{
	dataBuffer = nullptr;
	pUnorderedAccessView = nullptr;
}
UnorderedAccessView::UnorderedAccessView(ID3D11Device* device, ID3D11Resource* resource, UINT NumElements)
{
	Init(device, resource, NumElements);
}
//UnorderedAccessView::UnorderedAccessView(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride)
//{
//	Init(device, pData, ByteWidth, StructureByteStride);
//}

UnorderedAccessView::~UnorderedAccessView()
{
	if (dataBuffer)
		dataBuffer->Release();
	if (pUnorderedAccessView)
		pUnorderedAccessView->Release();
}
void UnorderedAccessView::Init(ID3D11Device* device, ID3D11Resource* resource, UINT NumElements)
{
	if (pUnorderedAccessView)
		pUnorderedAccessView->Release();

	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc{};
	unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	unorderedAccessViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	unorderedAccessViewDesc.Buffer.FirstElement = 0;
	unorderedAccessViewDesc.Buffer.NumElements = NumElements;
	if (FAILED(device->CreateUnorderedAccessView(resource, &unorderedAccessViewDesc, &pUnorderedAccessView)))
		MessageBox(nullptr, L"CreateUnorderedAccessView() is failed", L"UnorderedAccessView::Init()", MB_ICONERROR);
}
//void UnorderedAccessView::Init(ID3D11Device* device, void* pData,  UINT ByteWidth, UINT StructureByteStride)
//{
//	if (dataBuffer)
//		dataBuffer->Release();
//	if (pUnorderedAccessView)
//		pUnorderedAccessView->Release();
//
//	CD3D11_BUFFER_DESC bufferDesc{};
//	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
//	bufferDesc.ByteWidth = ByteWidth;
//	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//	bufferDesc.StructureByteStride = StructureByteStride;
//	D3D11_SUBRESOURCE_DATA subData{};
//	subData.pSysMem = pData;
//	if (FAILED(device->CreateBuffer(&bufferDesc, &subData, &dataBuffer)))
//		MessageBox(nullptr, L"CreateBuffer() is failed", L"UnorderedAccessView::Init()", MB_ICONERROR);
//
//	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc{};
//	unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
//	unorderedAccessViewDesc.Format = DXGI_FORMAT_UNKNOWN;
//	unorderedAccessViewDesc.Buffer.FirstElement = 0;
//	unorderedAccessViewDesc.Buffer.NumElements = ByteWidth / StructureByteStride;
//	if(FAILED(device->CreateUnorderedAccessView(dataBuffer, &unorderedAccessViewDesc, &pUnorderedAccessView)))
//		MessageBox(nullptr, L"CreateUnorderedAccessView() is failed", L"UnorderedAccessView::Init()", MB_ICONERROR);
//}


