#include "ColoredVerticesShader.h"

ColoredVerticesShader::ColoredVerticesShader()
{
	LoadVertexShader(L"VScoloredVertices.hlsl");
	LoadPixelShader(L"PScoloredVertices.hlsl");
	setParametrs(2, 7 * sizeof(FLOAT), 0);
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}, // возможно тут параша(D3D11_APPEND_ALIGNED_ELEMENT) -> 0
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}, // возможно тут параша(D3D11_APPEND_ALIGNED_ELEMENT) -> 12
	};
	LoadInputLayOut(inputElementDesc);
}