#pragma once
#include <d3d11.h>
#include "Timer.h"
#include "ConstantBuffer.h"
#include "InputManager.h"
#include "Object_PT.h"
class Camera : public Object_PT
{
public:
	Camera(ID3D11Device* device, const float2 resolution);
	~Camera();
	void Set(ID3D11DeviceContext* deviceCon, UINT FirstSlot_shaderResource);
	void SetEX(ID3D11DeviceContext* deviceCon, UINT FirstSlot_shaderResource, UINT FirstSlot_constantBuffer);
	//Return render state. If the camera moved then the Camera viewTexture is cleared
	bool Commit—hanges();
	void Upload_ConstBuffer();
	void Set_ConstBuffer(ID3D11DeviceContext* deviceCon, UINT StartSlot, UINT NumBuffers);

	float4 cameraPos;
	float2 cameraAngle;
	ID3D11ShaderResourceView* screen_shaderResource;
	float speed = 0.006f;
	ConstantBuffer cameraData_constBuf;
private:
	float3 oldCameraPos;
	float2 oldCameraAngle;
	BOOL textureQueue;
	ID3D11Texture2D* swap_viewTexture[2];
	ID3D11RenderTargetView* swap_renderTarget[2];
	ID3D11ShaderResourceView* swap_shaderResource[2];
	int multipleTo(float Value, float multNum);
	float4 GetCameraDiraction(float4 vec1, float4 angleOffset);
};

