#pragma once
#include <d3d11.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
class Mesh
{
public:
	Mesh(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount);
	void LoadData(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount);
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
};

