#pragma once
#include "DX.h"
class Shader
{
public:
	virtual ID3D11VertexShader* LoadVertexShader(LPCWSTR pathToVertexShader);
	virtual ID3D11PixelShader* LoadPixelShader(LPCWSTR pathToPixelShader);
	void SetShaders();
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D10Blob* compiledVertexShader;
	ID3D10Blob* compiledPixelShader;
	UINT numAttributes;
	UINT vertexSize;
	UINT offset;
	ID3D11InputLayout* pLayout;
protected:
	void setParametrs(UINT numAttributes, UINT vertexSize, UINT offset);
	void LoadInputLayOut(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc);
	//protected:
	//	virtual UINT NumAttributes() = 0;
	//	virtual UINT VertexSize() = 0;
	//	virtual UINT Offset() = 0;

};

