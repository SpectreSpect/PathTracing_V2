#pragma once
#include "DX.h"
#include "ComputeShader.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
class TestingComputeShader
{
public:
	TestingComputeShader(ID3D11Device* device);
	void Draw(ID3D11DeviceContext* deviceCon);
	VertexBuffer* screenQuadVertexBuffer;
	ComputeShader* rayGenerationShader;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* SRV;
	ID3D11UnorderedAccessView* UAV;
	float screenWidth;
	float screenHeight;
private:
	ShaderTexturing* shaderTexturing;
	SamplerState* samplerState;
	ID3D11UnorderedAccessView* UAV_NULL = {NULL};
};

