#include "Model.h"

void Model::LoadNodes(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadSubModel(scene->mMeshes[node->mMeshes[i]], scene);

	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNodes(node->mChildren[i], scene);
	}
}

void Model::LoadSubModel(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(),{0.0f,0.0f});
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals [i] .y, -mesh->mNormals[i].z });
	}
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	Mesh* submodel = new Mesh();
	submodel->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	subModels.push_back(submodel);
	meshToTex.push_back(mesh->mMaterialIndex); 
}

void Model::LoadMaterials(const aiScene* scene)
{
	textures.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene -> mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textures[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE,0,&path) == AI_SUCCESS)
			{
				//Some people export models with absolute file paths for textures, we need to fix that removing everything before the escape(\\) character, we're starting from the back
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx+1);
				std::string texPath = std::string("textures/" + filename);

				textures[i] = new Texture(texPath.c_str());
				if (!textures[i]->LoadTexture())
				{
					printf("Failed to load texture at %s\n", texPath.c_str());
					delete textures[i];
					textures[i] = nullptr;
				}
			}
		}
		if (!textures[i])
		{
			textures[i] = new Texture("textures/plain.png");
			textures[i]->LoadTextureWithAlpha();
		}
	}
}

Model::Model()
{
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_GenSmoothNormals|aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model %s failed to load %s", fileName, importer.GetErrorString());
		return;
	}
	LoadNodes(scene->mRootNode,scene);
	LoadMaterials(scene);
}

void Model::RenderModel()
{
	for (size_t i = 0; i < subModels.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];
		if (materialIndex < textures.size() && textures[materialIndex] )
		{
			textures[materialIndex]->UseTexture();
		}
		subModels[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < subModels.size(); i++)
	{
		if (subModels[i])
		{
			delete subModels[i];
			subModels[i] = nullptr;
		}
	}
}

Model::~Model()
{
	ClearModel();
}
