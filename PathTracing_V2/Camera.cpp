#include "Camera.h"

Camera::Camera(ID3D11Device* device, const float2 resolution)
{
	//type = RTObjectType::Camera;
	int size_ConstBuf = multipleTo(sizeof(oldCameraPos) + sizeof(cameraAngle), 16);
	cameraData_constBuf.Init(device, malloc(size_ConstBuf), size_ConstBuf);
	for (int i = 0; i < 2; i++) 
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = resolution.x;
		textureDesc.Height = resolution.y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		HRESULT hr = device->CreateTexture2D(&textureDesc, NULL, &swap_viewTexture[i]);
		if (FAILED(hr))MessageBox(nullptr, L"Camera::Camera()", L"CreateTexture2D() is failed", MB_ICONERROR);
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = device->CreateShaderResourceView(swap_viewTexture[i], &shaderResourceViewDesc, &swap_shaderResource[i]);
		if (FAILED(hr))MessageBox(nullptr, L"Camera::Camera()", L"CreateShaderResourceView() is failed", MB_ICONERROR);

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = device->CreateRenderTargetView(swap_viewTexture[i], &renderTargetViewDesc, &swap_renderTarget[i]);
		if (FAILED(hr))MessageBox(nullptr, L"Camera::Camera()", L"CreateRenderTargetView() is failed", MB_ICONERROR);
	}
	screen_shaderResource = swap_shaderResource[0];
}

Camera::~Camera()
{
	for (int i = 0; i < 2; i++) 
	{
		swap_viewTexture[i]->Release();
		swap_renderTarget[i]->Release();
		swap_shaderResource[i]->Release();
	}
}
void Camera::Set(ID3D11DeviceContext* deviceCon, UINT FirstSlot_shaderResource)
{
	textureQueue = !textureQueue;
	deviceCon->OMSetRenderTargets(1, &swap_renderTarget[textureQueue], NULL);
	deviceCon->PSSetShaderResources(FirstSlot_shaderResource, 1, &swap_shaderResource[!textureQueue]);
	screen_shaderResource = swap_shaderResource[textureQueue];
}
void Camera::SetEX(ID3D11DeviceContext* deviceCon, UINT FirstSlot_shaderResource, UINT FirstSlot_constantBuffer)
{
	Set(deviceCon, FirstSlot_shaderResource);
	CommitÑhanges();
	Upload_ConstBuffer();
	Set_ConstBuffer(deviceCon, FirstSlot_constantBuffer, 1);
}
float4 Camera::GetCameraDiraction(float4 vec1, float4 angleOffset)
{
	float4 output;
	output.x = (vec1.z * sin(cameraAngle.x) * cos(cameraAngle.y)) + vec1.x * cos(cameraAngle.x) - vec1.y * sin(cameraAngle.y) * sin(cameraAngle.x);
	output.y = vec1.z * sin(cameraAngle.y) + vec1.y * cos(cameraAngle.y);
	output.z = vec1.z * cos(cameraAngle.x) * cos(cameraAngle.y) - vec1.x * sin(cameraAngle.x) - vec1.y * sin(cameraAngle.y) * cos(cameraAngle.x);
	return output;
}
int Camera::multipleTo(float Value, float multNum)
{
	float div = Value / multNum;
	int result = (int(div) + 1) * multNum;
	return result;
}
bool Camera::CommitÑhanges()
{
	float deltaSped = speed * Timer::deltaTime;
	float4 forwardDiraction = GetCameraDiraction(float4{ 0, 0, 1, 0 }, float4{ 0, 0 });
	float4 rightDiraction = GetCameraDiraction(float4{ 1, 0, 0, 0 }, float4{ 0, 0 });
	//if()
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
	bool renderState = false;
	if (cameraPos.x != oldCameraPos.x || cameraPos.y != oldCameraPos.y || cameraPos.z != oldCameraPos.z)
		renderState = true;


	CURSORINFO cursorInfo;
	cursorInfo.hCursor = GetCursor();
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);

	if (cursorInfo.flags == 0)
	{
		cameraAngle.x += InputManager::MouseX * 0.0015f * Timer::deltaTime;
		cameraAngle.y -= InputManager::MouseY * 0.0015f * Timer::deltaTime;
	}
	if (cameraAngle.x != oldCameraAngle.x || cameraAngle.y != oldCameraAngle.y)
		renderState = true;
	oldCameraAngle.x = cameraAngle.x;
	oldCameraAngle.y = cameraAngle.y;


	oldCameraPos.x = cameraPos.x;
	oldCameraPos.y = cameraPos.y;
	return renderState;
}

void Camera::Upload_ConstBuffer()
{
	cameraData_constBuf.Map();
	size_t lastSize = cameraData_constBuf.CopyMem(&cameraPos, sizeof(cameraPos));
	lastSize = cameraData_constBuf.CopyMem(&cameraAngle, sizeof(cameraAngle), lastSize);
	cameraData_constBuf.UnMap();
}

void Camera::Set_ConstBuffer(ID3D11DeviceContext* deviceCon, UINT StartSlot, UINT NumBuffers)
{
	deviceCon->PSSetConstantBuffers(StartSlot, NumBuffers, &cameraData_constBuf.pBuf);
}

