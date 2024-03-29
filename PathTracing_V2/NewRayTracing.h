#pragma once
#include <d3d11.h>
#include "VertexBuffer.h"
#include "float3.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
#include <vector>
#include "Camera.h"
#include "ComputeShader.h"
#include "UnorderedAccessView.h"
#include "Buffer.h"
#include "Object_PT.h"
class NewRayTracing
{
public:
	NewRayTracing(ID3D11Device* device, const std::vector<Object_PT*> objects, float screenWidth, float screenHeight);
	~NewRayTracing();
	Camera* camera;
	VertexBuffer* screenQuadVertexBuffer;
	ShaderTexturing shaderTexturing;
	SamplerState samplerState;
	ComputeShader* rayGenerationShader;

	Buffer UAVBuffer;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* shaderResource;
	UnorderedAccessView* UAV;
	ID3D11Texture2D* UAVTexture;
	void Draw(ID3D11DeviceContext* deviceCon, float screenWidth, float screenHeight, int leftButtonState, int windowState);
private:
	DWORD HandelCursor(int leftButtonState);
};

