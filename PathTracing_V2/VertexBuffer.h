#pragma once
#include <d3d11.h>
class Shader;
class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(void* pSysMem, UINT ByteWidth, UINT verticesCount = 0, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	~VertexBuffer();
	void Release();
	void LoadData(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	ID3D11Buffer* pVertexBuffer = nullptr;
	UINT verticesCount;
	UINT vertexSize;
	size_t bufferSize;
	void* pSysMem;
	void Set(Shader* shader, UINT StartSlot = 0, UINT NumBuffers = 1);
private:
	D3D11_BUFFER_DESC vertexBufferDesc{};
	D3D11_SUBRESOURCE_DATA vertexSubData{};
};

