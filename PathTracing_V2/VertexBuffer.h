#pragma once
#include <d3d11.h>
#include "Vertex.h"
class Shader;
class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(void* pSysMem, UINT ByteWidth, UINT verticesCount = 0, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	~VertexBuffer();
	void Release();
	void Init(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	void UpLoadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth);
	ID3D11Buffer* pVertexBuffer = nullptr;
	UINT verticesCount;
	UINT vertexSize;
	size_t bufferSize;
	void* pSysMem;
private:
	D3D11_BUFFER_DESC vertexBufferDesc{};
	D3D11_SUBRESOURCE_DATA vertexSubData{};
};

