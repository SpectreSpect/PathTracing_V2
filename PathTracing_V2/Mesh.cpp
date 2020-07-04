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

//void Mesh::LoadData(const std::string& filePath)
//{
//	Assimp::Importer importer;
//	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
//	if (pScene == nullptr)
//		MessageBox(nullptr, L"LoadData::ReadFile() failed", L"Mesh", MB_ICONERROR);
//}
