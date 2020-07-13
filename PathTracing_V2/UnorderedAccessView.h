#pragma once
#include <d3d11.h>
class UnorderedAccessView
{
public:
	UnorderedAccessView();
	//UnorderedAccessView(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride);
	UnorderedAccessView(ID3D11Device* device, ID3D11Resource* resource, UINT NumElements);
	~UnorderedAccessView();
	//void Init(ID3D11Device* device, void* pData, UINT ByteWidth, UINT StructureByteStride);
	void Init(ID3D11Device* device, ID3D11Resource* resource, UINT NumElements);
	ID3D11UnorderedAccessView* pUnorderedAccessView;
	ID3D11Buffer* dataBuffer;
};

