#include "DefaultShader.h"

DefaultShader::DefaultShader()
{
	LoadVertexShader(L"VSDefault.hlsl");
	LoadPixelShader(L"PSDefault.hlsl");
	setParametrs(1, 3 * sizeof(FLOAT), 0);
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[1]{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} };
	LoadInputLayOut(inputElementDesc);
}

