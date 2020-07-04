#include "Shape.h"
struct kek
{
	int k;
	float b;
};

Shape::Shape()
{
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

	FLOAT constantData[4];
	constantBuffer = new ConstantBuffer(constantData, sizeof(constantData));
	renderTarget = new RenderTarget(DX::screenResolutionWidth, DX::screenResolutionHeight);
}

Shape::~Shape()
{
	delete vertexBuffer;
	delete shader;
	delete indexBuffer;
	delete constantBuffer;
}
void Shape::Draw()
{

	constantBuffer->Set(0, 1);
	shader->SetShaders();
	if (indexed == TRUE)
		DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &shader->vertexSize, &shader->offset);
	DX::deviceCon->IASetPrimitiveTopology(primitiveTopology);
	if (indexed == FALSE)
	{
		DX::deviceCon->Draw(vertexBuffer->verticesCount, 0);
	}
	else DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);
}

void Shape::Draw(Shader* shader)
{
	constantBuffer->Set(0, 1);
	shader->SetShaders();
	if (indexed == TRUE)
		DX::deviceCon->IASetIndexBuffer(indexBuffer->pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DX::deviceCon->IASetVertexBuffers(0, 1, &vertexBuffer->pVertexBuffer, &vertexBuffer->vertexSize, &shader->offset);
	DX::deviceCon->IASetPrimitiveTopology(primitiveTopology);
	if (indexed == FALSE)
	{
		DX::deviceCon->Draw(vertexBuffer->verticesCount, 0);
	}
	else    DX::deviceCon->DrawIndexed(indexBuffer->indecesCount, 0, 0);
}

