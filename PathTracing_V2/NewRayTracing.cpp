#include "NewRayTracing.h"

NewRayTracing::NewRayTracing(const std::vector<RTObject*> objects)
{
	//DirectX::CreateWICTextureFromFile(DX::device, L"..\\Data\\Textures\\cfer.png", nullptr, &shaderResource);
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
void NewRayTracing::Draw(ID3D11DeviceContext* deviceCon, int leftButtonState, int windowState)
{
	camera->Set(deviceCon, 0);
	if(HandelCursor(leftButtonState) == 0 || windowState == 1)
	camera->CommitÑhanges();
	camera->Upload_ConstBuffer();
	camera->Set_ConstBuffer(deviceCon, 0, 1);

	shaderTexturing.SetShaders(deviceCon);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing.offset);
	deviceCon->PSSetSamplers(0, 1, &samplerState.samplerState);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);

	deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	deviceCon->PSSetSamplers(0, 1, &samplerState.samplerState);
	deviceCon->PSSetShaderResources(0, 1, &camera->screen_shaderResource);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing.offset);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);
}


