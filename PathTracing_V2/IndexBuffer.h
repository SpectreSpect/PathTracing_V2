#pragma once
#include <d3d11.h>
class Shader;
class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(void* pSysMem, UINT ByteWidth, UINT indecesCount, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	~IndexBuffer();
	void Release();
	void LoadData(void* pSysMem, UINT ByteWidth, UINT indecesCount, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, UINT CPUAccessFlags = 0);
	void Set(DXGI_FORMAT Format = DXGI_FORMAT_R16_UINT, UINT Offset = 0);
	ID3D11Buffer* pIndexBuffer = nullptr;
	void* pSysMem;
	UINT indecesCount;
	size_t bufferSize;
private:
	D3D11_BUFFER_DESC indexBufferDesc{};
	D3D11_SUBRESOURCE_DATA indexSubData{};
};

