#include "Shader.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	pVertexBuffer = nullptr;
}

VertexBuffer::VertexBuffer(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	Init(pSysMem, ByteWidth, verticesCount, Usage, CPUAccessFlags);
}

VertexBuffer::~VertexBuffer()
{
	Release();
}

void VertexBuffer::Release()
{
	if (pVertexBuffer != nullptr)
		pVertexBuffer->Release();
}

void VertexBuffer::Init(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	Release();
	this->verticesCount = verticesCount;
	this->vertexSize = ByteWidth / verticesCount;
	this->bufferSize = ByteWidth;
	vertexBufferDesc.Usage = Usage;
	vertexBufferDesc.ByteWidth = ByteWidth;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = CPUAccessFlags;
	vertexSubData.pSysMem = pSysMem;
	this->pSysMem = pSysMem;
	HRESULT hr = DX::device->CreateBuffer(&vertexBufferDesc, &vertexSubData, &pVertexBuffer);
	if (FAILED(hr))
		MessageBox(nullptr, L"LoadData() failed", L"VertexBuffer", MB_ICONERROR);
}
void VertexBuffer::UpLoadData(ID3D11DeviceContext* deviceCon, const void* pData, const size_t dataWidth)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub{};
	HRESULT hr = deviceCon->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, pData, dataWidth);
	deviceCon->Unmap(pVertexBuffer, 0);
}
