#include "ShaderTexturing.h"

ShaderTexturing::ShaderTexturing()
{
	LoadVertexShader(L"VSTexturing.hlsl");
	LoadPixelShader(L"PSTexturing.hlsl");
	setParametrs(2, 5 * sizeof(FLOAT), 0);
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[2]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	LoadInputLayOut(inputElementDesc);
}
