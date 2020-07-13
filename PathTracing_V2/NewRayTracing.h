#pragma once
#include <d3d11.h>
#include "VertexBuffer.h"
#include "float3.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
#include <vector>
#include "RTObject.h"
#include "Camera.h"
#include "ComputeShader.h"
#include "UnorderedAccessView.h"
#include "Buffer.h"
class NewRayTracing
{
public:
	NewRayTracing(ID3D11Device* device, const std::vector<RTObject*> objects);
	~NewRayTracing();
	Camera* camera;
	VertexBuffer* screenQuadVertexBuffer;
	ShaderTexturing shaderTexturing;
	SamplerState samplerState;
	ComputeShader* rayGenerationShader;
	ID3D11ShaderResourceView* shaderResource;
	Buffer UAVBuffer;
	UnorderedAccessView* UAV;
	void Draw(ID3D11DeviceContext* deviceCon, float screenWidth, float screenHeight, int leftButtonState, int windowState);
private:
	DWORD HandelCursor(int leftButtonState);
};

