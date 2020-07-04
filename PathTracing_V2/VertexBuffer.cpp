#include "Shader.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	pVertexBuffer = nullptr;
}

VertexBuffer::VertexBuffer(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	LoadData(pSysMem, ByteWidth, verticesCount, Usage, CPUAccessFlags);
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

void VertexBuffer::LoadData(void* pSysMem, UINT ByteWidth, UINT verticesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
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

void VertexBuffer::Set(Shader* shader, UINT StartSlot, UINT NumBuffers)
{
	DX::deviceCon->IASetVertexBuffers(StartSlot, NumBuffers, &pVertexBuffer, &shader->vertexSize, &shader->offset);
}
