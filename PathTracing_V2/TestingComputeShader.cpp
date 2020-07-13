#include "TestingComputeShader.h"

TestingComputeShader::TestingComputeShader(ID3D11Device* device)
{
	screenWidth = DX::screenResolutionWidth;
	screenHeight = DX::screenResolutionHeight;
	rayGenerationShader = new ComputeShader(device, L"RayGenerationShader.hlsl", "main", "cs_5_0");
	shaderTexturing = new ShaderTexturing();
	samplerState = new SamplerState();
	FLOAT vertices[30]
	{
		-1.0f, -1.0f, 0.0f, 0, 1,
		-1.0f, 1.0f, 0.0f, 0, 0,
		1.0f,  -1.0f, 0.0f, 1, 1,
		1.0f, 1.0f, 0.0f, 1, 0,
		1.0f, -1.0f, 0.0f, 1, 1,
		-1.0f,  1.0f, 0.0f, 0, 0,
	};
	screenQuadVertexBuffer = new VertexBuffer(vertices, sizeof(vertices), 6);


	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = screenWidth;
	textureDesc.Height = screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	DX::device->CreateTexture2D(&textureDesc, NULL, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	HRESULT hr = DX::device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &SRV);
	if (FAILED(hr))
		DX::ApplicationRun = FALSE;


	D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc{};
	unorderedAccessViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	if (FAILED(device->CreateUnorderedAccessView(texture, &unorderedAccessViewDesc, &UAV)))
		MessageBox(nullptr, L"CreateUnorderedAccessView() is failed", L"UnorderedAccessView::Init()", MB_ICONERROR);

}

void TestingComputeShader::Draw(ID3D11DeviceContext* deviceCon)
{
	deviceCon->CSSetShader(rayGenerationShader->pComputeShader, NULL, 0);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV, NULL);

	deviceCon->Dispatch(screenWidth / 8, screenHeight / 8, 1);

	deviceCon->CSSetShader(NULL, NULL, 0);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, NULL);

	deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	shaderTexturing->SetShaders(deviceCon);
	deviceCon->PSSetSamplers(0, 1, &samplerState->samplerState);
	deviceCon->PSSetShaderResources(0, 1, &SRV);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing->offset);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);
}
