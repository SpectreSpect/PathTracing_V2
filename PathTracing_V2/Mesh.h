#pragma once
#include <d3d11.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
class Mesh
{
public:
	Mesh(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount);
	Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices);
	void LoadData(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount);
	void LoadData(std::vector<Vertex> vertices, std::vector<UINT> indices);
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
};

