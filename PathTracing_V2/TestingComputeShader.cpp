#include "TestingComputeShader.h"

TestingComputeShader::TestingComputeShader(ID3D11Device* device, std::vector<Object_PT*> objectsArray)
{
	screenWidth = DX::screenResolutionWidth;
	screenHeight = DX::screenResolutionHeight;
	rayGenerationShader = new ComputeShader(device, L"RayGenerationShader.hlsl", "main", "cs_5_0");
	intersectionShader = new ComputeShader(device, L"IntersectionShader.hlsl", "main", "cs_5_0");
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

	for (int i = 0; i < objectsArray.size(); i++)
	{
		if (objectsArray[i]->type == Object_PT::ObjectType_PT::Sphere)
		{
			spheres.push_back(*reinterpret_cast<Sphere_PT*>(objectsArray[i]));
		}
	}



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
	if(FAILED(device->CreateTexture2D(&textureDesc, NULL, &output_texture)))
		MessageBox(nullptr, L"CreateTexture2D() for (output_texture) is failed", L"TestingComputeShader::TestingComputeShader()", MB_ICONERROR);

	D3D11_SHADER_RESOURCE_VIEW_DESC output_SRV_Desc{};
	output_SRV_Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	output_SRV_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	output_SRV_Desc.Texture2D.MostDetailedMip = 0;
	output_SRV_Desc.Texture2D.MipLevels = 1;
	if(FAILED(device->CreateShaderResourceView(output_texture, &output_SRV_Desc, &output_SRV)))
		MessageBox(nullptr, L"CreateShaderResourceView() for (output_SRV) is failed", L"TestingComputeShader::TestingComputeShader()", MB_ICONERROR);


	D3D11_UNORDERED_ACCESS_VIEW_DESC output_UAV_Desc{};
	output_UAV_Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	output_UAV_Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	if (FAILED(device->CreateUnorderedAccessView(output_texture, &output_UAV_Desc, &output_UAV)))
		MessageBox(nullptr, L"CreateUnorderedAccessView() for (output_UAV) is failed", L"TestingComputeShader::TestingComputeShader()", MB_ICONERROR);

	InitRayGenerator(device, ray_Buffer, &ray_UAV, &ray_SRV);
	InitRayIntersector(device, spherePrimetive_Buffer, &spherePrimetive_SRV, spheres);
}

void TestingComputeShader::InitRayGenerator(ID3D11Device* device, Buffer* ray_Buffer, ID3D11UnorderedAccessView** ray_UAV, ID3D11ShaderResourceView** ray_SRV)
{
	ray_Buffer = new Buffer(device, nullptr, sizeof(Ray) * screenWidth * screenHeight, sizeof(Ray), 
							D3D11_USAGE_DEFAULT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 0,
							D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);
					
	D3D11_UNORDERED_ACCESS_VIEW_DESC ray_UAV_Desc{};
	ray_UAV_Desc.Format = DXGI_FORMAT_UNKNOWN;
	ray_UAV_Desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	ray_UAV_Desc.Buffer.FirstElement = 0;
	ray_UAV_Desc.Buffer.NumElements = screenWidth * screenHeight;
	if (FAILED(device->CreateUnorderedAccessView(ray_Buffer->pBuf, &ray_UAV_Desc, ray_UAV)))
		MessageBox(nullptr, L"CreateUnorderedAccessView() for (output_UAV) is failed", L"TestingComputeShader::TestingComputeShader()", MB_ICONERROR);

	D3D11_SHADER_RESOURCE_VIEW_DESC ray_SRV_Desc{};
	ray_SRV_Desc.Format = DXGI_FORMAT_UNKNOWN;
	ray_SRV_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	ray_SRV_Desc.Buffer.FirstElement = 0;
	ray_SRV_Desc.Buffer.ElementOffset = 0;
	ray_SRV_Desc.Buffer.ElementWidth = sizeof(Ray);
	ray_SRV_Desc.Buffer.NumElements = screenWidth * screenHeight;
	if (FAILED(device->CreateShaderResourceView(ray_Buffer->pBuf, &ray_SRV_Desc, ray_SRV)))
		MessageBox(nullptr, L"CreateShaderResourceView() for (ray_SRV) is failed", L"TestingComputeShader::InitRayGenerator()", MB_ICONERROR);

	ray_ConstantBuffer = new ConstantBuffer(malloc(16 * 2), 16 * 2);
}

void TestingComputeShader::InitRayIntersector(ID3D11Device* device, Buffer* spherePrimetive_Buffer, ID3D11ShaderResourceView** spherePrimetive_SRV, std::vector<Sphere_PT>& spheres)
{
	Sphere_PT::SpherePrimitive* spherePrimetives = new Sphere_PT::SpherePrimitive[spheres.size()];
	for (int i = 0; i < spheres.size(); i++)
	{
		spherePrimetives[i].objID = i;
		spherePrimetives[i].position = spheres[i].position;
		spherePrimetives[i].radius = spheres[i].radius;
	}
	

	spherePrimetive_Buffer = new Buffer(device, spherePrimetives, sizeof(Sphere_PT::SpherePrimitive) * spheres.size(),
								             sizeof(Sphere_PT::SpherePrimitive), D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, 
											 D3D11_CPU_ACCESS_WRITE, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);

	D3D11_SHADER_RESOURCE_VIEW_DESC spherePrimetive_SRV_Desc{};
	spherePrimetive_SRV_Desc.Format = DXGI_FORMAT_UNKNOWN;
	spherePrimetive_SRV_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	spherePrimetive_SRV_Desc.Buffer.FirstElement = 0;
	spherePrimetive_SRV_Desc.Buffer.ElementOffset = 0;
	spherePrimetive_SRV_Desc.Buffer.ElementWidth = sizeof(Sphere_PT::SpherePrimitive);
	spherePrimetive_SRV_Desc.Buffer.NumElements = spheres.size();
	if (FAILED(device->CreateShaderResourceView(spherePrimetive_Buffer->pBuf, &spherePrimetive_SRV_Desc, spherePrimetive_SRV)))
		MessageBox(nullptr, L"CreateShaderResourceView() for (spherePrimetive_SRV) is failed", L"TestingComputeShader::InitRayIntersector()", MB_ICONERROR);
}

void TestingComputeShader::GenerateRays(ID3D11DeviceContext* deviceCon, ID3D11UnorderedAccessView** outPut_UAV, ID3D11UnorderedAccessView** ray_UAV, ConstantBuffer* ray_ConstantBuffer)
{
	//UpdateConstantData
	size_t lastSize;
	ray_ConstantBuffer->Map();
	lastSize = ray_ConstantBuffer->CopyMem(&samplesCount, sizeof(samplesCount));
	lastSize = ray_ConstantBuffer->CopyMem(&cameraPos, sizeof(cameraPos), lastSize);
	lastSize = ray_ConstantBuffer->CopyMem(&cameraAngle, sizeof(cameraAngle), lastSize);
	ray_ConstantBuffer->UnMap();
	//Bind
	deviceCon->CSSetShader(rayGenerationShader->pComputeShader, NULL, 0);
	deviceCon->CSSetUnorderedAccessViews(0, 1, outPut_UAV, NULL);
	deviceCon->CSSetUnorderedAccessViews(1, 1, ray_UAV, NULL);
	deviceCon->CSSetConstantBuffers(0, 1, &ray_ConstantBuffer->pConstantBuffer);

	deviceCon->Dispatch(screenWidth / 8, screenHeight / 8, 1);

	//Unbind
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, NULL);
	deviceCon->CSSetUnorderedAccessViews(1, 1, &UAV_NULL, NULL);
	deviceCon->CSSetConstantBuffers(0, 1, &ConstantBuffer_NULL);
	deviceCon->CSSetShader(NULL, NULL, 0);
}
void TestingComputeShader::IntersectPrimetives(ID3D11DeviceContext* deviceCon, ID3D11ShaderResourceView** spherePrimetive_SRV, ID3D11ShaderResourceView** ray_SRV, ID3D11UnorderedAccessView** output_UAV)
{
	//Bind
	deviceCon->CSSetShader(intersectionShader->pComputeShader, NULL, 0);
	deviceCon->CSSetShaderResources(0, 1, spherePrimetive_SRV);
	deviceCon->CSSetShaderResources(1, 1, ray_SRV);
	deviceCon->CSSetUnorderedAccessViews(0, 1, output_UAV, NULL);

	deviceCon->Dispatch(screenWidth / 8, screenHeight / 8, 1);

	//Unbind
	deviceCon->CSSetShaderResources(0, 1, &SRV_NULL);
	deviceCon->CSSetShaderResources(1, 1, &SRV_NULL);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, NULL);
	deviceCon->CSSetShader(NULL, NULL, 0);
}
void TestingComputeShader::Draw(ID3D11DeviceContext* deviceCon)
{
	samplesCount++;
	GenerateRays(deviceCon, &output_UAV, &ray_UAV, ray_ConstantBuffer);
	IntersectPrimetives(deviceCon, &spherePrimetive_SRV, &ray_SRV, &output_UAV);
	//Bind
	deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	shaderTexturing->SetShaders(deviceCon);
	deviceCon->PSSetSamplers(0, 1, &samplerState->samplerState);
	deviceCon->PSSetShaderResources(0, 1, &output_SRV);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing->offset);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);
	//Unbind
	deviceCon->PSSetShaderResources(0, 1, &SRV_NULL);
}


