#pragma once
#include <d3d11.h>
#include "VertexBuffer.h"
#include "float3.h"
#include "ShaderTexturing.h"
#include "SamplerState.h"
#include <vector>
#include "RTObject.h"
#include "Camera.h"
class NewRayTracing
{
public:
	NewRayTracing(const std::vector<RTObject*> objects);
	~NewRayTracing();
	Camera* camera;
	VertexBuffer* screenQuadVertexBuffer;
	ShaderTexturing shaderTexturing;
	SamplerState samplerState;
	void Draw(ID3D11DeviceContext* deviceCon, int leftButtonState, int windowState);
private:
	DWORD HandelCursor(int leftButtonState);
};

