#pragma once
#include "Shape.h"
#include "ShaderRayTracing.h"
#include "ShaderTexturing.h"
#include "D3D11Model.h"
#include "StructuredBuffer.h"
#include "Vertex.h"
class RayTracing : public Shape
{
public:
	RayTracing();
	~RayTracing();
	virtual void Draw() override;
	D3D11Model* figure;
	struct MeshObj
	{
		float indexOffset;
		float indecesCount;
	};
	//unsigned int MeshObjectsCount = 1;
	//unsigned int veticesCount = 3;
	//unsigned int indicesCount = 3;
	//MeshObj* meshObjects;
	//Vertex* vetices;
	//int* indices;
	ConstantBuffer* cModelBuffer;
	SamplerState* samplerState;
	ShaderResourceView* HDRshaderResource;
	ID3D11ShaderResourceView* shaderResources[7];
	//ID3D11Resource* resource;
	ID3D11BlendState* blender;
	ID3D11Texture2D* texture[2]; // 2 текстуры
	ID3D11RenderTargetView* textureRenderTarget[2]; // 2 рендер торгета для текстур, для того чтобы в них можно было рендерить
	ID3D11ShaderResourceView* shaderResourceView[2]; // 2 шейдер ресурса, для того, чтобы их можно было использовать как ресурс в шейдере
	ID3D11UnorderedAccessView* pStructuredBufferUAV;
	StructuredBuffer meshes;
	StructuredBuffer vertices;
	StructuredBuffer indices;
	Shader* shaderRayTracing;
	Shader* shaderTexturing;
	BOOL textureQueue;
	Vector4 position;
	Vector4 sphere1Pos;
	Vector4 k;
	Vector4 screenResolution;
	Vector4 cameraPos;
	Vector4 cameraAngle;
	Vector4 currentCameraAngle;
	Vector4 oldCameraAngle;
	Vector4 randomValue;
	Vector4 random1Value;
	Vector4 samplesCount;
	Vector4 velocity;
	Vector4 circlePos;
	Vector4 cyrclesPos[20];
	Vector4 cyrcleVelocity[20];
	FLOAT samplesAmount = 5000;
	UINT timer;
	float speed = 0.01f;
private:
	void InitStructuredBuffer();
	void InitModels();
	void* cModelBufferData;
	void InitShaderResource();
	void CorrectScreenResolution();
	void BindButtons();
	void HideOrShowCursor();
	void SetRandomValue();
	void Moving();
	Vector4 GetCameraDiraction(Vector4 vec1, Vector2 angleOffset);
	void setConstantData();
	void ClearFrame();
	void LoadTextures();
	void SetTextures();
};

