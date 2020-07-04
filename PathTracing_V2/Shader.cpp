#include "Shader.h"

ID3D11VertexShader* Shader::LoadVertexShader(LPCWSTR pathToVertexShader)
{
	D3DCompileFromFile(pathToVertexShader, NULL, NULL, "main", "vs_5_0", NULL, NULL, &compiledVertexShader, NULL);
	DX::device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &pVertexShader);
	return pVertexShader;
}

ID3D11PixelShader* Shader::LoadPixelShader(LPCWSTR pathToPixelShader)
{
	D3DCompileFromFile(pathToPixelShader, NULL, NULL, "main", "ps_5_0", NULL, NULL, &compiledPixelShader, NULL);
	DX::device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pPixelShader);
	return pPixelShader;
}


void Shader::SetShaders()
{
	DX::deviceCon->VSSetShader(pVertexShader, NULL, NULL);
	DX::deviceCon->PSSetShader(pPixelShader, NULL, NULL);
	//DirectX::device->CreateInputLayout(inputElementDesc, numAttributes, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &pLayout);
	DX::deviceCon->IASetInputLayout(pLayout);
}

void Shader::setParametrs(UINT numAttributes, UINT vertexSize, UINT offset)
{
	this->numAttributes = numAttributes;
	this->vertexSize = vertexSize;
	this->offset = offset;
}

void Shader::LoadInputLayOut(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc)
{
	DX::device->CreateInputLayout(inputElementDesc, numAttributes, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &pLayout);
}
