#include "NewRayTracing.h"

NewRayTracing::NewRayTracing(ID3D11Device* device, const std::vector<RTObject*> objects, float screenWidth, float screenHeight)
{
	//DirectX::CreateWICTextureFromFile(DX::device, L"..\\Data\\Textures\\cfer.png", nullptr, &shaderResource);
	rayGenerationShader = new ComputeShader(device, L"RayGenerationShader.hlsl", "main", "cs_5_0");

	size_t width = 1920;
	size_t height = 1080;

	size_t structureSize = sizeof(FLOAT) * 4;
	size_t dataSize = width * height * structureSize;
	void* dataAddress = malloc(dataSize);

	//UAVBuffer.InitBuffer_UAV(device, dataAddress, dataSize, structureSize);
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	device->CreateTexture2D(&textureDesc, NULL, &UAVTexture);
	UAV = new UnorderedAccessView(device, UAVTexture, dataSize / structureSize);

	//////////////////////////////////////////////////
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	device->CreateTexture2D(&textureDesc, NULL, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	HRESULT hr = DX::device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResource);
	if (FAILED(hr))
		DX::ApplicationRun = FALSE;
	///////////////////////////////////////////


	camera = new Camera(DX::device, float2{DX::screenResolutionWidth, DX::screenResolutionHeight});
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
}

NewRayTracing::~NewRayTracing()
{
	delete screenQuadVertexBuffer;
	delete camera;
	delete rayGenerationShader;
	delete UAV;
}
DWORD NewRayTracing::HandelCursor(int leftButtonState)
{
	CURSORINFO cursorInfo;
	cursorInfo.hCursor = GetCursor();
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);
	if (leftButtonState == 1)
	{
		if (cursorInfo.flags == 1)
		{
			ShowCursor(false);
			DX::ConfineCursor(TRUE);
		}
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		if (cursorInfo.flags == 0)
		{
			ShowCursor(true);
			DX::ConfineCursor(FALSE);
		}
	}
	return cursorInfo.flags;
}
void NewRayTracing::Draw(ID3D11DeviceContext* deviceCon,float screenWidth, float screenHeight, int leftButtonState, int windowState)
{
	//camera->Set(deviceCon, 0);
	if (HandelCursor(leftButtonState) == 0 || windowState == 1);
		//camera->CommitÑhanges();
		//camera->Upload_ConstBuffer();
		//camera->Set_ConstBuffer(deviceCon, 0, 1);


	//shaderTexturing.SetShaders(deviceCon);
	//deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing.offset);
	//deviceCon->PSSetSamplers(0, 1, &samplerState.samplerState);
	//deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);

	deviceCon->CSSetShader(rayGenerationShader->pComputeShader, NULL, 0);
	//deviceCon->CSSetShaderResources(0, 1, &shaderResource);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV->pUnorderedAccessView, NULL);
	deviceCon->Dispatch(std::ceil(screenWidth / 8), std::ceil(screenHeight / 8), 1);

	deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	shaderTexturing.SetShaders(deviceCon);
	deviceCon->PSSetSamplers(0, 1, &samplerState.samplerState);
	deviceCon->PSSetShaderResources(0, 1, &shaderResource);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing.offset);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);
}


