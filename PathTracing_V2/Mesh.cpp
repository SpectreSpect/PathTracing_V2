#include "Mesh.h"

Mesh::Mesh(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount)
{
	LoadData(pVertices, verticesWidth, verticesCount, pIndeces, indecesWidth, indecesCount);
}
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
{
	LoadData(vertices, indices);
}
void Mesh::LoadData(void* pVertices, const size_t verticesWidth, const UINT verticesCount, void* pIndeces, size_t indecesWidth, UINT indecesCount)
{
	vertexBuffer.Init(pVertices, verticesWidth, verticesCount);
	indexBuffer.LoadData(pIndeces, indecesWidth, indecesCount);
}
void Mesh::LoadData(std::vector<Vertex> vertices, std::vector<UINT> indices)
{
	LoadData(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size(), indices.data(), indices.size() * sizeof(UINT), indices.size());
}