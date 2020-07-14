#pragma once
#include "DX.h"
#include "ComputeShader.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
#include "float3.h"
class TestingComputeShader
{
public:
	TestingComputeShader(ID3D11Device* device);
	void Draw(ID3D11DeviceContext* deviceCon);
	float samplesCount;
	float3 cameraPos;
	float2 cameraAngle;
	VertexBuffer* screenQuadVertexBuffer;
	ComputeShader* rayGenerationShader;
	ComputeShader* intersectionShader;

	ID3D11Texture2D* output_texture;
	ID3D11ShaderResourceView* output_SRV;
	ID3D11UnorderedAccessView* output_UAV;

	struct Ray 
	{
		float t;
		float3 direction;
		float3 origin;
		float3 energy;
		float3 result;
	};
	ID3D11Buffer* ray_Buffer;
	ID3D11UnorderedAccessView* ray_UAV;
	ConstantBuffer* ray_ConstantBuffer;

	struct SpherePrimetive
	{
		float4 spherePos;
		//float radius;
		//int objID;
	};
	UINT spherePrimetive_Count = 1;
	ID3D11Buffer* spherePrimetive_Buffer;
	ID3D11ShaderResourceView* spherePrimetive_SRV;

private:
	void InitRayGenerator(ID3D11Device* device, ID3D11Buffer* ray_Buffer, ID3D11UnorderedAccessView* rayUAV);
	void InitRayIntersector(ID3D11Device* device, ID3D11Buffer* spherePrimetive_Buffer, ID3D11ShaderResourceView* spherePrimetive_SRV, void* spherePrimetive_pData, UINT spherePrimetive_Count);
	void GenerateRays(ID3D11DeviceContext* deviceCon, ID3D11UnorderedAccessView** outPut_UAV, ID3D11UnorderedAccessView** ray_UAV, ConstantBuffer* ray_ConstantBuffer);
	void IntersectPrimetives(ID3D11DeviceContext* deviceCon);
	float screenWidth;
	float screenHeight;
	ShaderTexturing* shaderTexturing;
	SamplerState* samplerState;
	ID3D11UnorderedAccessView* UAV_NULL = {NULL};
	ID3D11ShaderResourceView* SRV_NULL = {NULL};
	ID3D11Buffer* ConstantBuffer_NULL = {NULL};
};

