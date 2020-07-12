#pragma once
#include "Shape.h"
#include "ShaderRayTracing.h"
#include "ShaderTexturing.h"
#include "D3D11Model.h"
#include "StructuredBuffer.h"
#include "Vertex.h"
#include "Timer.h"
#include "Camera.h"
class RayTracing
{
public:
	RayTracing();
	~RayTracing();
	void Draw();
	D3D11Model* figure;
	struct MeshObj
	{
		float indexOffset;
		float indecesCount;
	};
	Camera* camera;
	ConstantBuffer* constantBuffer;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* cModelBuffer;
	SamplerState* samplerState;
	ShaderResourceView* HDRshaderResource;
	ID3D11ShaderResourceView* shaderResources[7];
	ID3D11BlendState* blender;
	ID3D11Texture2D* texture[2]; // 2 ��������
	ID3D11RenderTargetView* textureRenderTarget[2]; // 2 ������ ������� ��� �������, ��� ���� ����� � ��� ����� ���� ���������
	ID3D11ShaderResourceView* shaderResourceView[2]; // 2 ������ �������, ��� ����, ����� �� ����� ���� ������������ ��� ������ � �������
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
	float speed = 0.006f;
private:
	Vector4 oldCameraPos;
	void InitStructuredBuffer();
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