#include "TestingComputeShader.h"

TestingComputeShader::TestingComputeShader(ID3D11Device* device, std::vector<Object_PT*> objectsArray)
{
	screenWidth = DX::screenResolutionWidth;
	screenHeight = DX::screenResolutionHeight;
	rayGenerationShader = new ComputeShader(device, L"RayGenerationShader.hlsl", "main", "cs_5_0");
	intersectionShader = new ComputeShader(device, L"IntersectionShader.hlsl", "main", "cs_5_0");
	clossest_HitShader = new ComputeShader(device, L"Clossest_HitShader.hlsl", "main", "cs_5_0");
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

	this->objectArray = objectsArray;
	for (int i = 0; i < objectsArray.size(); i++)
	{
		if (objectsArray[i]->type == Object_PT::ObjectType_PT::Sphere)
		{
			spheres.push_back(*reinterpret_cast<Sphere_PT*>(objectsArray[i]));
		}
	}

	output_texture.Init(device, nullptr, screenWidth, screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT,
							 D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, 0, D3D11_USAGE_DEFAULT, 0, 1, 1, 1);
	output_SRV.Init(device, &output_texture);
	output_UAV.Init(device, &output_texture);

	InitRayGenerator(device, &ray_SBuffer, &ray_UAV, &ray_SRV);
	InitRayIntersector(device, &spherePrimetive_SBuffer, &spherePrimetive_SRV, spheres);
	shade_CBuffer.Init(device, nullptr, sizeof(Shade_CBuffer_Desc));
}

void TestingComputeShader::InitRayGenerator(ID3D11Device* device, StructuredBuffer* ray_SBuffer, UAV* ray_UAV, SRV* ray_SRV)
{
	ray_SBuffer->Init(device, nullptr, sizeof(Ray), sizeof(Ray) * screenWidth * screenHeight, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 0, D3D11_USAGE_DEFAULT);
	ray_UAV->Init(device, ray_SBuffer);
	ray_SRV->Init(device, ray_SBuffer);
	ray_ConstantBuffer.Init(device, malloc(16 * 2), 16 * 2);
}

void TestingComputeShader::InitRayIntersector(ID3D11Device* device, StructuredBuffer* spherePrimetive_SBuffer, SRV* spherePrimetive_SRV, std::vector<Sphere_PT>& spheres)
{
	Sphere_PT::SpherePrimitive* spherePrimetives = new Sphere_PT::SpherePrimitive[spheres.size()];
	for (int i = 0; i < spheres.size(); i++)
	{
		spherePrimetives[i].objID = i;
		spherePrimetives[i].position = spheres[i].position;
		spherePrimetives[i].radius = spheres[i].radius;
	}
	primetiveCount_CBuffer_Description primitiveCount_Desc{};
	primitiveCount_Desc.nSpheres = spheres.size();
	primetiveCount_CBuffer.Init(device, &primitiveCount_Desc, sizeof(primitiveCount_Desc));

	hit_SBuffer.Init(device, nullptr, sizeof(Hit), screenWidth * screenHeight * sizeof(Hit), D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 0, D3D11_USAGE_DEFAULT);
	hit_UAV.Init(device, &hit_SBuffer);
	hit_SRV.Init(device, &hit_SBuffer);
	spherePrimetive_SBuffer->Init(device, spherePrimetives, sizeof(Sphere_PT::SpherePrimitive), sizeof(Sphere_PT::SpherePrimitive) * spheres.size(),
									 D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	spherePrimetive_SRV->Init(device, spherePrimetive_SBuffer);

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
	deviceCon->CSSetConstantBuffers(0, 1, &ray_ConstantBuffer->pBuf);

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
	deviceCon->CSSetUnorderedAccessViews(1, 1, &hit_UAV.pUAV, NULL);
	deviceCon->CSSetConstantBuffers(0, 1, &primetiveCount_CBuffer.pBuf);

	deviceCon->Dispatch(screenWidth / 8, screenHeight / 8, 1);

	//Unbind
	deviceCon->CSSetShaderResources(0, 1, &SRV_NULL);
	deviceCon->CSSetShaderResources(1, 1, &SRV_NULL);
	deviceCon->CSSetUnorderedAccessViews(1, 1, &UAV_NULL, NULL);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, NULL);
	deviceCon->CSSetShader(NULL, NULL, 0);
}
void TestingComputeShader::ShadePrimitives(ID3D11DeviceContext* deviceCon)
{
	Shade_CBuffer_Desc shade{};
	deviceCon->CSSetShader(clossest_HitShader->pComputeShader, NULL, 0);
	deviceCon->CSSetUnorderedAccessViews(0, 1, &output_UAV.pUAV, NULL);
	deviceCon->CSSetUnorderedAccessViews(1, 1, &ray_UAV.pUAV, NULL);
	deviceCon->CSSetShaderResources(0, 1, &hit_SRV.pSRV);
	for (int i = 0; i < objectArray.size(); i++)
	{
		shade.albedo = objectArray[i]->albedo;
		shade.emission = objectArray[i]->emission;
		shade.objID = i;
		shade._seed = rand();
		shade_CBuffer.CopyMemOnce(&shade, sizeof(Shade_CBuffer_Desc));
		deviceCon->CSSetConstantBuffers(0, 1, &shade_CBuffer.pBuf);
		deviceCon->Dispatch(screenWidth / 8, screenHeight / 8, 1);
	}
	deviceCon->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, NULL);
	deviceCon->CSSetUnorderedAccessViews(1, 1, &UAV_NULL, NULL);
	deviceCon->CSSetShaderResources(0, 1, &SRV_NULL);
	deviceCon->CSSetShader(NULL, NULL, 0);
}
void TestingComputeShader::Draw(ID3D11DeviceContext* deviceCon)
{
	samplesCount++;
	GenerateRays(deviceCon, &output_UAV.pUAV, &ray_UAV.pUAV, &ray_ConstantBuffer);
	IntersectPrimetives(deviceCon, &spherePrimetive_SRV.pSRV, &ray_SRV.pSRV, &output_UAV.pUAV);
	ShadePrimitives(deviceCon);
	//Bind
	deviceCon->OMSetRenderTargets(1, &DX::backRenderTargetView, NULL);
	shaderTexturing->SetShaders(deviceCon);
	deviceCon->PSSetSamplers(0, 1, &samplerState->samplerState);
	deviceCon->PSSetShaderResources(0, 1, &output_SRV.pSRV);
	deviceCon->IASetVertexBuffers(0, 1, &screenQuadVertexBuffer->pVertexBuffer, &screenQuadVertexBuffer->vertexSize, &shaderTexturing->offset);
	deviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCon->Draw(screenQuadVertexBuffer->verticesCount, 0);
	//Unbind
	deviceCon->PSSetShaderResources(0, 1, &SRV_NULL);
}


