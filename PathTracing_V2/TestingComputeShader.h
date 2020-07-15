#pragma once
#include "DX.h"
#include "ComputeShader.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
#include "float3.h"
#include <vector>
#include "Sphere_PT.h"
#include "Buffer.h"
#include "SRV.h"
#include "Texture2D.h"
#include  "UAV.h"
class TestingComputeShader
{
public:
	TestingComputeShader(ID3D11Device* device, std::vector<Object_PT*> objectsArray);
	void Draw(ID3D11DeviceContext* deviceCon);
	float samplesCount;
	float3 cameraPos;
	float2 cameraAngle;
	VertexBuffer* screenQuadVertexBuffer;
	ComputeShader* rayGenerationShader;
	ComputeShader* intersectionShader;
	std::vector<Sphere_PT> spheres;

	Texture2D output_texture;
	SRV output_SRV;
	UAV output_UAV;


	struct Ray 
	{
		float t;
		float3 direction;
		float3 origin;
		float3 energy;
		float3 result;
	};
	StructuredBuffer ray_SBuffer;
	UAV ray_UAV;
	SRV ray_SRV;
	ConstantBuffer ray_ConstantBuffer;


	StructuredBuffer spherePrimetive_SBuffer;
	SRV spherePrimetive_SRV;

private:
	void InitRayGenerator(ID3D11Device* device, StructuredBuffer* ray_SBuffer, UAV* ray_UAV, SRV* ray_SRV);
	void InitRayIntersector(ID3D11Device* device, StructuredBuffer* spherePrimetive_SBuffer, SRV* spherePrimetive_SRV, std::vector<Sphere_PT>& spheres);
	void GenerateRays(ID3D11DeviceContext* deviceCon, ID3D11UnorderedAccessView** outPut_UAV, ID3D11UnorderedAccessView** ray_UAV, ConstantBuffer* ray_ConstantBuffer);
	void IntersectPrimetives(ID3D11DeviceContext* deviceCon, ID3D11ShaderResourceView** spherePrimetive_SRV, ID3D11ShaderResourceView** ray_SRV, ID3D11UnorderedAccessView** output_UAV);
	float screenWidth;
	float screenHeight;
	ShaderTexturing* shaderTexturing;
	SamplerState* samplerState;
	ID3D11UnorderedAccessView* UAV_NULL = {NULL};
	ID3D11ShaderResourceView* SRV_NULL = {NULL};
	ID3D11Buffer* ConstantBuffer_NULL = {NULL};
};

