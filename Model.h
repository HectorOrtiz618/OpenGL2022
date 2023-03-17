#pragma once
#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"
class Model
{
private:
	std::vector<Mesh*> subModels;
	std::vector<Texture*> textures;
	std::vector<unsigned int> meshToTex;

	void LoadNodes(aiNode *node,const aiScene *scene);
	void LoadSubModel(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene *scene);
public:
	Model();
	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();
};

