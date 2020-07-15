#pragma once
#include "MainWindow.h"
#include "GameObject.h"
#include "DefaultShader.h"
#include "Vector4.h"
#include "InputManager.h"
#include <iostream>
#include "RenderTarget.h"
class Shape : public GameObject
{
public:
	Shape();
	~Shape();
	void virtual Draw();
	void Draw(Shader* shader);
	//void virtual setConstantData() = 0;
	RenderTarget* renderTarget;
	BOOL test;
	Shader* shader = new DefaultShader();
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer constantBuffer;
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* sampler;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	BOOL indexed = TRUE;
};

