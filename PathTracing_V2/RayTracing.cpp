#include "RayTracing.h"
RayTracing::RayTracing()
{
	figure = new D3D11Model("..\\Data\\Models\\Monkey.obj");
	LoadTextures();
	HDRshaderResource = new ShaderResourceView(L"..\\Data\\Textures\\HDRs\\Diffuse.png");
	//shader = new ShaderRayTracing();
	shaderRayTracing = new ShaderRayTracing();
	shaderTexturing = new ShaderTexturing();

	FLOAT constantData[44];
	constantBuffer = new ConstantBuffer(constantData, sizeof(constantData));

	//InitModels();
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
	//delete[MeshObjectsCount] meshObjects;
	//delete[veticesCount] vetices;
	//delete[indicesCount] indices;
	delete figure;
	delete cModelBufferData;
	delete cModelBuffer;
	delete samplerState;
	delete HDRshaderResource;
	for (int i = 0; i < 7; i++)
		shaderResources[i]->Release();
	//resource->Release();
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
	const unsigned int veticesCount = figure->meshes[0].vertexBuffer.verticesCount;
	const unsigned int indicesCount = figure->meshes[0].indexBuffer.indecesCount;
	//const unsigned int veticesCount = 4;
	//const unsigned int indicesCount = 6;
	Vector3 k1 = *(Vector3*)(figure->meshes[0].vertexBuffer.pSysMem);
	Vector3 k2 = *(Vector3*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(Vector3));
	Vector3 k3 = *(Vector3*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(Vector3) * 2);
	Vector3 k4 = *(Vector3*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(Vector3) * 3);
	//float k2 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float));
	//float k3 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 2);

	//float k4 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 3);
	//float k5 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 4);
	//float k6 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 5);

	//float k7 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 6);
	//float k8 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 7);
	//float k9 = *(float*)((size_t)figure->meshes[0].vertexBuffer.pSysMem + sizeof(float) * 8);

	
	size_t vertexSize = sizeof(Vertex);

	MeshObj meshObjects[MeshObjectsCount]
	{ 0, indicesCount };

	void* vertices = figure->meshes[0].vertexBuffer.pSysMem;
	void* indices = figure->meshes[0].indexBuffer.pSysMem;
	//Vertex vertices[veticesCount]{
	//{0, 0, 0},
	//{1, 0, 0},
	//{0, 1, 0},
	//{1, 1, 0}
	//};
	//int indices[indicesCount]{
	//0, 1, 2,
	//3, 1, 2
	//};

	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = sizeof(MeshObj);
	sbDesc.ByteWidth = sizeof(MeshObj) * MeshObjectsCount;
	DX::device->CreateBuffer(&sbDesc, 0, &sbMeshes);

	sbDesc.StructureByteStride = sizeof(Vertex);
	sbDesc.ByteWidth = sizeof(Vertex) * veticesCount;
	DX::device->CreateBuffer(&sbDesc, 0, &sbVertices);

	sbDesc.StructureByteStride = sizeof(int);
	sbDesc.ByteWidth = sizeof(int) * indicesCount;
	DX::device->CreateBuffer(&sbDesc, 0, &sbIndices);

	D3D11_MAPPED_SUBRESOURCE mappedSub{};
	DX::deviceCon->Map(sbMeshes, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, meshObjects, sizeof(MeshObj) * MeshObjectsCount);
	DX::deviceCon->Unmap(sbMeshes, 0);

	DX::deviceCon->Map(sbVertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, vertices, sizeof(Vertex) * veticesCount);
	DX::deviceCon->Unmap(sbVertices, 0);

	DX::deviceCon->Map(sbIndices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	memcpy(mappedSub.pData, indices, sizeof(int) * indicesCount);
	DX::deviceCon->Unmap(sbIndices, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.ElementOffset = 0;
	shaderResourceViewDesc.Buffer.ElementWidth = sizeof(MeshObj);
	shaderResourceViewDesc.Buffer.FirstElement = 0;
	shaderResourceViewDesc.Buffer.NumElements = MeshObjectsCount;
	DX::device->CreateShaderResourceView(sbMeshes, &shaderResourceViewDesc, &srMeshes);
	shaderResourceViewDesc.Buffer.ElementWidth = sizeof(Vertex);
	shaderResourceViewDesc.Buffer.NumElements = veticesCount;
	DX::device->CreateShaderResourceView(sbVertices, &shaderResourceViewDesc, &srVertices);
	shaderResourceViewDesc.Buffer.ElementWidth = sizeof(int);
	shaderResourceViewDesc.Buffer.NumElements = indicesCount;
	DX::device->CreateShaderResourceView(sbIndices, &shaderResourceViewDesc, &srIndices);
}
void RayTracing::InitModels()
{
	//meshObjects = new MeshObj[MeshObjectsCount]
	//{
	//	{1, 3}
	//};
	//vetices = new Vertex[veticesCount]
	//{
	//	{1, 1, 1},
	//	{1, 1, 1},
	//	{1, 1, 1}
	//};
	//indices = new int[indicesCount]
	//{
	//	1, 1, 2
	//};

	////unsigned int cModelBufferSize = sizeof(MeshObj) * MeshObjectsCount + sizeof(Vertex) * veticesCount + sizeof(int) * indicesCount;
	//unsigned int cModelBufferSize = sizeof(float) * 3;
	//cModelBufferData = malloc(cModelBufferSize);
	//cModelBuffer = new ConstantBuffer(cModelBufferData, DX::multipleTo(cModelBufferSize, 16));
	//float l[3]{1, 0.5f, 1};
	//size_t lastSize;
	//cModelBuffer->Map();
	////lastSize = cModelBuffer->CopyMem(meshObjects, sizeof(MeshObj) * MeshObjectsCount);
	////lastSize = cModelBuffer->CopyMem(vetices, sizeof(Vertex) * veticesCount, lastSize);
	////lastSize = cModelBuffer->CopyMem(indices, sizeof(int) * indicesCount, lastSize);
	//lastSize = cModelBuffer->CopyMem(&l, sizeof(float) * 3);
	//cModelBuffer->UnMap();
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
	//DX::deviceCon->UpdateSubresource(constantBuffer->pConstantBuffer, 0, constantData,  );
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
	{
		k.w = 1;
	}
	else k.w = 0;

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

	HDRshaderResource->Set(1, 1);
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
	if (GetAsyncKeyState('R'))
	{

	}
	if (GetAsyncKeyState('F'))
	{

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
		currentCameraAngle.x += InputManager::MouseX / 500;
		currentCameraAngle.y -= InputManager::MouseY / 500;
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

	Vector4 forwardDiraction = GetCameraDiraction(Vector4{ 0, 0, 1, 0 }, Vector2{ 0, 0 });
	Vector4 rightDiraction = GetCameraDiraction(Vector4{ 1, 0, 0, 0 }, Vector2{ 0, 0 });

	if (GetAsyncKeyState('W'))
	{
		cameraPos.x += forwardDiraction.x * speed;
		cameraPos.y += forwardDiraction.y * speed;
		cameraPos.z += forwardDiraction.z * speed;
	}
	else if (GetAsyncKeyState('S'))
	{
		cameraPos.x -= forwardDiraction.x * speed;
		cameraPos.y -= forwardDiraction.y * speed;
		cameraPos.z -= forwardDiraction.z * speed;
	}

	if (GetAsyncKeyState('A'))
	{
		cameraPos.x -= rightDiraction.x * speed;
		cameraPos.y -= rightDiraction.y * speed;
		cameraPos.z -= rightDiraction.z * speed;
	}
	else if (GetAsyncKeyState('D'))
	{
		cameraPos.x += rightDiraction.x * speed;
		cameraPos.y += rightDiraction.y * speed;
		cameraPos.z += rightDiraction.z * speed;
	}

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		cameraPos.y -= speed;
	}
	else if (GetAsyncKeyState(VK_SPACE))
	{
		cameraPos.y += speed;
	}
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

	for (int i = 0; i < 1; i++)
	{
		//circlePos.x = 0;
		//circlePos.y = i / 20;
		velocity.x = 1;
		velocity.w = 1;
		if (k.w == 1)
			samplesCount.y = 0;
		DX::deviceCon->OMSetRenderTargets(1, &textureRenderTarget[textureQueue], NULL); // ������ ��������, ��� �������� 0 
		// textureQueue - ������� ����������, textureQueue = FALSE ����������� textureQueue = 0, ������� ���������� �������� ��� �������� 0
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

		UINT initCount = -1;
		DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[!textureQueue]);
		//DX::deviceCon->CSSetUnorderedAccessViews(0, 1, &pStructuredBufferUAV, &initCount);
		DX::deviceCon->PSSetShaderResources(50, 1, &srMeshes);
		DX::deviceCon->PSSetShaderResources(51, 1, &srVertices);
		DX::deviceCon->PSSetShaderResources(52, 1, &srIndices);
		//UINT initilCount = 1;
		//DX::deviceCon->OMSetRenderTargetsAndUnorderedAccessViews(0, nullptr, nullptr, 1, 1, &pStructuredBufferUAV, &initilCount);
		//cModelBuffer->Set(1, 1);
		Shape::Draw(shaderRayTracing);
	}

	DX::deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	samplerState->Set(0, 1);
	DX::deviceCon->PSSetShaderResources(0, 1, &shaderResourceView[textureQueue]);
	Shape::Draw(shaderTexturing);
	textureQueue = !textureQueue;
}
