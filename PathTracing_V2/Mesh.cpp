#include "Mesh.h"

Mesh::Mesh(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount)
{
	LoadData(pVertices, verticesWidth, verticesCount, pIndeces, indecesWidth, indecesCount);
}
void Mesh::LoadData(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount)
{
	vertexBuffer.LoadData(pVertices, verticesWidth, verticesCount);
	indexBuffer.LoadData(pIndeces, indecesWidth, indecesCount);
}
