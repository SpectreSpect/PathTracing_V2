#include "SamplerState.h"

SamplerState::SamplerState(
	D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV,
	D3D11_TEXTURE_ADDRESS_MODE AddressW,
	D3D11_COMPARISON_FUNC ComparisonFunc,
	FLOAT MinLOD,
	FLOAT MaxLOD)
{
	LoadData(Filter, AddressU, AddressV, AddressW, ComparisonFunc, MinLOD, MaxLOD);
}

SamplerState::SamplerState(D3D11_SAMPLER_DESC samplerDesc)
{
	DX::device->CreateSamplerState(&samplerDesc, &samplerState);
}

SamplerState::~SamplerState()
{
	Release();
}

void SamplerState::Release()
{
	if (samplerState != nullptr)
		samplerState->Release();
}

void SamplerState::LoadData(D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV,
	D3D11_TEXTURE_ADDRESS_MODE AddressW,
	D3D11_COMPARISON_FUNC ComparisonFunc,
	FLOAT MinLOD,
	FLOAT MaxLOD)
{
	Release();
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = Filter;
	samplerDesc.AddressU = AddressU;
	samplerDesc.AddressV = AddressV;
	samplerDesc.AddressW = AddressW;
	samplerDesc.ComparisonFunc = ComparisonFunc;
	samplerDesc.MinLOD = MinLOD;
	samplerDesc.MaxLOD = MaxLOD;
	DX::device->CreateSamplerState(&samplerDesc, &samplerState);
}

void SamplerState::Set(UINT startSlot, UINT NumSamplers)
{
	DX::deviceCon->PSSetSamplers(startSlot, NumSamplers, &samplerState);
}
