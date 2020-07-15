#pragma once
#include <d3d11.h>
class SRV 
{
public:
	SRV();
	//void Init_Buffered();
	ID3D11ShaderResourceView* pSRV;
};