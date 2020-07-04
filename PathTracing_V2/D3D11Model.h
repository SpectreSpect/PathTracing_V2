#pragma once
#include "Mesh.h"
#include "Vector4.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class D3D11Model
{
public:
	D3D11Model(const std::string filePath);
	void LoadModel(const std::string filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh> meshes;
};

