#include "Shader.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(void* pSysMem, UINT ByteWidth, UINT indecesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	LoadData(pSysMem, ByteWidth, indecesCount, Usage, CPUAccessFlags);
}

IndexBuffer::~IndexBuffer()
{
	Release();
}

void IndexBuffer::Release()
{
	if (pIndexBuffer != nullptr)
		pIndexBuffer->Release();
}

void IndexBuffer::LoadData(void* pSysMem, UINT ByteWidth, UINT indecesCount, D3D11_USAGE Usage, UINT CPUAccessFlags)
{
	Release();
	this->indecesCount = indecesCount;
	this->bufferSize = ByteWidth;
	indexBufferDesc.Usage = Usage;
	indexBufferDesc.ByteWidth = ByteWidth;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = CPUAccessFlags;
	indexSubData.pSysMem = pSysMem;
	this->pSysMem = pSysMem;
	HRESULT hr = DX::device->CreateBuffer(&indexBufferDesc, &indexSubData, &pIndexBuffer);
	if (FAILED(hr))
		MessageBox(nullptr, L"LoadData() failed", L"IndexBuffer", MB_ICONERROR);
}

void IndexBuffer::Set(DXGI_FORMAT Format, UINT Offset)
{
	DX::deviceCon->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}
