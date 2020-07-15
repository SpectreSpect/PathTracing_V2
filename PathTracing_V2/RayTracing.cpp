#include "RayTracing.h"
RayTracing::RayTracing()
{
	camera = new Camera(DX::device, float2{DX::screenResolutionWidth, DX::screenResolutionHeight});
	unsigned short indeces[6] =
	{
		0, 1, 2,
		2, 3, 0
	};
	indexBuffer = new IndexBuffer(indeces, sizeof(indeces), 6);

	FLOAT vertices[20] =
	{
		-1, 1, 0.0f, 0, 0,
		1, 1, 0.0f, 1, 0,
		1, -1, 0.0f, 1, 1,
		-1, -1, 0.0f, 0, 1
	};
	vertexBuffer = new VertexBuffer(vertices, sizeof(vertices), 4);


	figure = new D3D11Model("..\\Data\\Models\\Monkey.obj");
	LoadTextures();
	HDRshaderResource = new ShaderResourceView(L"..\\Data\\Textures\\HDRs\\Diffuse.png");
	shaderRayTracing = new ShaderRayTracing();
	shaderTexturing = new ShaderTexturing();

	FLOAT constantData[44];
	constantBuffer = new ConstantBuffer(constantData, sizeof(constantData));
	InitStructuredBuffer();
	samplerState = new SamplerState();
	InitShaderResource();

	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	rtbd.BlendEnable = TRUE;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtbd;
	DX::device->CreateBlendState(&blendDesc, &blender);
}
RayTracing::~RayTracing()
{
	delete figure;
	delete cModelBufferData;
	delete cModelBuffer;
	delete samplerState;
	delete HDRshaderResource;
	for (int i = 0; i < 7; i++)
		shaderResources[i]->Release();
	blender->Release();
	for (int i = 0; i < 2; i++)
	{
		texture[i]->Release();
		textureRenderTarget[i]->Release();
		shaderResourceView[i]->Release();
	}
	delete shaderRayTracing;
	delete shaderTexturing;
}
void RayTracing::InitStructuredBuffer()
{
	const unsigned int MeshObjectsCount = 1;
	MeshObj meshObjects[MeshObjectsCount]
	{ 0, figure->meshes[0].indexBuffer.indecesCount };
	HCheck(meshes_SB.Init(DX::device, nullptr, sizeof(MeshObj), sizeof(MeshObj) * MeshObjectsCount,
							D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC), L"meshes.InitSBuffer() was failed", L"RayTracing");

	HCheck(vertices_SB.Init(DX::device, nullptr, sizeof(Vertex), sizeof(Vertex) * figure->meshes[0].vertexBuffer.verticesCount,
							  D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC),L"vertices.InitSBuffer() was failed", L"RayTracing" );

	HCheck(indices_SB.Init(DX::device, nullptr, sizeof(int), sizeof(int) * figure->meshes[0].indexBuffer.indecesCount,
							 D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC), L"indices.InitSBuffer() was failed", L"RayTracing");

	HCheck(meshes_SB.UploadData(DX::deviceCon, meshObjects, sizeof(MeshObj) * MeshObjectsCount), L"meshes.UploadData() was failed", L"RayTracing");
	HCheck(vertices_SB.UploadData(DX::deviceCon, figure->meshes[0].vertexBuffer.pSysMem, sizeof(Vertex) * figure->meshes[0].vertexBuffer.verticesCount), L"vertices.UploadData was failed", L"RayTracing");
	HCheck(indices_SB.UploadData(DX::deviceCon, figure->meshes[0].indexBuffer.pSysMem, sizeof(int) * figure->meshes[0].indexBuffer.indecesCount), L"indices.UploadData() was failed", L"RayTracing");


	HCheck(meshes_SRV.Init(DX::device, &meshes_SB), L"meshes_SRV.Init() was failed", L"RayTracing::InitStructuredBuffer()");
	HCheck(vertices_SRV.Init(DX::device, &vertices_SB), L"vertices_SRV.Init() was failed", L"RayTracing::InitStructuredBuffer()");
	HCheck(indices_SRV.Init(DX::device, &indices_SB), L"vertices_SRV.Init() was failed", L"RayTracing::InitStructuredBuffer()");

}
void RayTracing::InitShaderResource()
{
	for (int i = 0; i < 2; i++)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = DX::screenResolutionWidth;
		textureDesc.Height = DX::screenResolutionHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		DX::device->CreateTexture2D(&textureDesc, NULL, &texture[i]);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		HRESULT hr = DX::device->CreateShaderResourceView(texture[i], &shaderResourceViewDesc, &shaderResourceView[i]);
		if (FAILED(hr))
			DX::ApplicationRun = FALSE;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = DX::device->CreateRenderTargetView(texture[i], &renderTargetViewDesc, &textureRenderTarget[i]);
		if (FAILED(hr))
			DX::ApplicationRun = FALSE;
	}
}
void RayTracing::CorrectScreenResolution()
{
	screenResolution.x = DX::screenResolutionWidth;
	screenResolution.y = DX::screenResolutionHeight;
	screenResolution.w = DX::screenResolutionWidth / DX::screenResolutionHeight;
}

void RayTracing::setConstantData()
{
	size_t lastSize;
	constantBuffer->Map();
	lastSize = constantBuffer->CopyMem(&position, sizeof(position));
	lastSize = constantBuffer->CopyMem(&sphere1Pos, sizeof(sphere1Pos), lastSize);
	lastSize = constantBuffer->CopyMem(&k, sizeof(k), lastSize);
	lastSize = constantBuffer->CopyMem(&screenResolution, sizeof(screenResolution), lastSize);
	lastSize = constantBuffer->CopyMem(&cameraPos, sizeof(cameraPos), lastSize);
	lastSize = constantBuffer->CopyMem(&cameraAngle, sizeof(cameraAngle), lastSize);
	lastSize = constantBuffer->CopyMem(&randomValue, sizeof(randomValue), lastSize);
	lastSize = constantBuffer->CopyMem(&random1Value, sizeof(random1Value), lastSize);
	lastSize = constantBuffer->CopyMem(&samplesCount, sizeof(samplesCount), lastSize);
	lastSize = constantBuffer->CopyMem(&velocity, sizeof(velocity), lastSize);
	lastSize = constantBuffer->CopyMem(&circlePos, sizeof(circlePos), lastSize);
	constantBuffer->UnMap();
}

void RayTracing::ClearFrame()
{
	if (currentCameraAngle.x != cameraAngle.x || currentCameraAngle.y != cameraAngle.y)
		k.w = 1;
}

void RayTracing::LoadTextures()
{
	ShaderResourceView::LoadToShaderResource(&shaderResources[0], L"..\\Data\\Textures\\SandRock.jpg");
	ShaderResourceView::LoadToShaderResource(&shaderResources[1], L"..\\Data\\Textures\\SandRockNormal.jpg");

	ShaderResourceView::LoadToShaderResource(&shaderResources[2], L"..\\Data\\Textures\\Brick.png");
	ShaderResourceView::LoadToShaderResource(&shaderResources[3], L"..\\Data\\Textures\\LavaBrick.png");

	ShaderResourceView::LoadToShaderResource(&shaderResources[4], L"..\\Data\\Textures\\HDRs\\LightMap.png");
	ShaderResourceView::LoadToShaderResource(&shaderResources[5], L"..\\Data\\Textures\\MetalDesignerWeaveSteel\\MetalDesignerWeaveSteel002_NRM_3K_SPECULAR.jpg");
	ShaderResourceView::LoadToShaderResource(&shaderResources[6], L"..\\Data\\Textures\\MetalDesignerWeaveSteel\\MetalDesignerWeaveSteel002_REFL_3K_SPECULAR.jpg");
}

void RayTracing::SetTextures()
{
	DX::deviceCon->PSSetShaderResources(1, 1, &HDRshaderResource->shaderResourceView);
	DX::deviceCon->PSSetShaderResources(2, 1, &shaderResources[0]);
	DX::deviceCon->PSSetShaderResources(3, 1, &shaderResources[1]);
	DX::deviceCon->PSSetShaderResources(4, 1, &shaderResources[2]);
	DX::deviceCon->PSSetShaderResources(5, 1, &shaderResources[3]);
	DX::deviceCon->PSSetShaderResources(6, 1, &shaderResources[4]);
	DX::deviceCon->PSSetShaderResources(7, 1, &shaderResources[5]);
	DX::deviceCon->PSSetShaderResources(8, 1, &shaderResources[6]);
}

void RayTracing::BindButtons()
{
	if (GetAsyncKeyState('Q'))
	{
		k.x = 0;
	}
	if (GetAsyncKeyState('E'))
	{
		k.x = 1;
	}
	if (GetAsyncKeyState('I'))
	{
		sphere1Pos.z += 0.001;
	}
	else if (GetAsyncKeyState('K'))
	{
		sphere1Pos.z -= 0.001;
	}
	if (GetAsyncKeyState('J'))
	{
		sphere1Pos.x -= 0.001;
	}
	else 	if (GetAsyncKeyState('L'))
	{
		sphere1Pos.x += 0.001;
	}
}

void RayTracing::HideOrShowCursor()
{
	CURSORINFO cursorInfo;
	cursorInfo.hCursor = GetCursor();
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);
	if (GetAsyncKeyState('X'))
	{

		if (cursorInfo.flags == 1)
		{
			ShowCursor(false);
			DX::ConfineCursor(TRUE);
		}
	}
	if (GetAsyncKeyState('C'))
	{
		if (cursorInfo.flags == 0)
		{
			ShowCursor(true);
			DX::ConfineCursor(FALSE);
		}
	}
	if (cursorInfo.flags == 0)
	{
		currentCameraAngle.x += InputManager::MouseX * 0.0015f * Timer::deltaTime;
		currentCameraAngle.y -= InputManager::MouseY * 0.0015f * Timer::deltaTime;
	}
	ClearFrame();
	cameraAngle.x = currentCameraAngle.x;
	cameraAngle.y = currentCameraAngle.y;


}



void RayTracing::SetRandomValue()
{
	randomValue.x = rand();
	random1Value.x = rand();
	k.z = rand() % 1000;
}

void RayTracing::Moving()
{
	float deltaSped = speed * Timer::deltaTime;
	Vector4 forwardDiraction = GetCameraDiraction(Vector4{ 0, 0, 1, 0 }, Vector2{ 0, 0 });
	Vector4 rightDiraction = GetCameraDiraction(Vector4{ 1, 0, 0, 0 }, Vector2{ 0, 0 });

	if (GetAsyncKeyState('W'))
	{
		cameraPos.x += forwardDiraction.x * deltaSped;
		cameraPos.y += forwardDiraction.y * deltaSped;
		cameraPos.z += forwardDiraction.z * deltaSped;
	}
	else if (GetAsyncKeyState('S'))
	{
		cameraPos.x -= forwardDiraction.x * deltaSped;
		cameraPos.y -= forwardDiraction.y * deltaSped;
		cameraPos.z -= forwardDiraction.z * deltaSped;
	}

	if (GetAsyncKeyState('A'))
	{
		cameraPos.x -= rightDiraction.x * deltaSped;
		cameraPos.y -= rightDiraction.y * deltaSped;
		cameraPos.z -= rightDiraction.z * deltaSped;
	}
	else if (GetAsyncKeyState('D'))
	{
		cameraPos.x += rightDiraction.x * deltaSped;
		cameraPos.y += rightDiraction.y * deltaSped;
		cameraPos.z += rightDiraction.z * deltaSped;
	}

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		cameraPos.y -= deltaSped;
	}
	else if (GetAsyncKeyState(VK_SPACE))
	{
		cameraPos.y += deltaSped;
	}
	if (cameraPos.x != oldCameraPos.x || cameraPos.y != oldCameraPos.y || cameraPos.z != oldCameraPos.z)
		k.w = 1;

	oldCameraPos = cameraPos;
}


Vector4 RayTracing::GetCameraDiraction(Vector4 vec1, Vector2 angleOffset)
{
	Vector4 output;
	output.x = (vec1.z * sin(cameraAngle.x) * cos(cameraAngle.y)) + vec1.x * cos(cameraAngle.x) - vec1.y * sin(cameraAngle.y) * sin(cameraAngle.x);
	output.y = vec1.z * sin(cameraAngle.y) + vec1.y * cos(cameraAngle.y);
	output.z = vec1.z * cos(cameraAngle.x) * cos(cameraAngle.y) - vec1.x * sin(cameraAngle.x) - vec1.y * sin(cameraAngle.y) * cos(cameraAngle.x);
	return output;
}

void RayTracing::Draw()
{
	//velocity.x = 1;
	//velocity.w = 1;
	//if (k.w == 1)
	//	samplesCount.y = 0;
	//DX::deviceCon->OMSetRenderTargets(1, &textureRenderTarget[textureQueue], NULL); // Ставлю текстуру, под индексом 0 
	//k.w = 0;
	//samplesCount.x = samplesAmount;
	//samplesCount.y++;

	//CorrectScreenResolution();
	//BindButtons();
	//HideOrShowCursor();

	//SetRandomValue();
	//Moving();
	//setConstantData();
	//samplerState->Set(0, 1);
	//SetTextures();

	//UINT initCount = -1;
	//DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[!textureQueue]);
	//DX::deviceCon->PSSetShaderResources(50, 1, &meshes.pSBShaderResource);
	//DX::deviceCon->PSSetShaderResources(51, 1, &vertices.pSBShaderResource);
	//DX::deviceCon->PSSetShaderResources(52, 1, &indices.pSBShaderResource);


	////Draw(shaderRayTracing);
	//constantBuffer->Set(1, 1);
	//shaderRayTracing->SetShaders();
	//DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &vertexBuffer->vertexSize, &shaderRayTracing->offset);
	//DX::deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);

	////Draw(shaderTexturing);
	//DX::deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	//shaderTexturing->SetShaders();
	//constantBuffer->Set(1, 1);
	//samplerState->Set(0, 1);
	//DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[textureQueue]);
	//DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &vertexBuffer->vertexSize, &shaderTexturing->offset);
	//DX::deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);
	//textureQueue = !textureQueue;
	WINDOWPLACEMENT winPlacment{};
	winPlacment.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(DX::currentWindow, &winPlacment);
	//if(winPlacment.showCmd == 1)
	//sphere1Pos.x = 1;
	if (*DX::windowState == 1) 
	{
		sphere1Pos.x = 1;
	}else sphere1Pos.x = 0;
		

	camera->CommitСhanges();
	camera->Upload_ConstBuffer();
	velocity.x = 1;
	velocity.w = 1;
	if (k.w == 1)
		samplesCount.y = 0;
	DX::deviceCon->OMSetRenderTargets(1, &textureRenderTarget[textureQueue], NULL); // Ставлю текстуру, под индексом 0 
	k.w = 0;
	samplesCount.x = samplesAmount;
	samplesCount.y++;

	CorrectScreenResolution();
	BindButtons();
	HideOrShowCursor();

	SetRandomValue();
	Moving();
	setConstantData();
	samplerState->Set(0, 1);
	SetTextures();
	DX::deviceCon->PSSetConstantBuffers(0, 1, &camera->cameraData_constBuf->pConstantBuffer);
	UINT initCount = -1;
	DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[!textureQueue]);
	DX::deviceCon->PSSetShaderResources(50, 1, &meshes_SRV.pSRV);
	DX::deviceCon->PSSetShaderResources(51, 1, &vertices_SRV.pSRV);
	DX::deviceCon->PSSetShaderResources(52, 1, &indices_SRV.pSRV);


	//Draw(shaderRayTracing);
	constantBuffer->Set(1, 1);
	shaderRayTracing->SetShaders();
	DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &vertexBuffer->vertexSize, &shaderRayTracing->offset);
	DX::deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);

	//Draw(shaderTexturing);
	DX::deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	shaderTexturing->SetShaders();
	constantBuffer->Set(1, 1);
	samplerState->Set(0, 1);
	DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[textureQueue]);
	DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &vertexBuffer->vertexSize, &shaderTexturing->offset);
	DX::deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);
	textureQueue = !textureQueue;
}

