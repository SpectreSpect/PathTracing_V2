#include "D3D11Model.h"
D3D11Model::D3D11Model(const std::string filePath) 
{
	LoadModel(filePath);
}
void D3D11Model::LoadModel(const std::string filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || !scene->mRootNode)
		MessageBox(nullptr, L"LoadModel::importer.ReadFile() is failed", L"D3D11Model", MB_ICONERROR);
	ProcessNode(scene->mRootNode, scene);
}

void D3D11Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh D3D11Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	Vertex* vertices = new Vertex[mesh->mNumVertices];
	UINT* indices = new UINT[(size_t)mesh->mNumFaces * 3];
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].pos.x = mesh->mVertices[i].x;
		vertices[i].pos.y = mesh->mVertices[i].y;
		vertices[i].pos.z = mesh->mVertices[i].z;
	}
	int offset = 0;
	for ( int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for ( int c = 0; c < face.mNumIndices; c++)
			indices[c + offset] = face.mIndices[c];
		offset = face.mNumIndices * (i + 1);
	}
	return Mesh(vertices, sizeof(Vertex) * mesh->mNumVertices, mesh->mNumVertices, indices, sizeof(UINT) * (size_t)mesh->mNumFaces * 3, (size_t)mesh->mNumFaces * 3);

}

