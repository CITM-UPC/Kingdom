#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture2D.h"
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

class MeshLoader
{
public:

	static std::vector<std::shared_ptr<Mesh>> loadFromFile(const std::string& path)
	{
		std::vector<std::shared_ptr<Mesh>> mesh_ptrs;

		auto scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
		for (size_t m = 0; m < scene->mNumMeshes; ++m) {
			auto mesh = scene->mMeshes[m];
			auto faces = mesh->mFaces;
			vec3f* verts = (vec3f*)mesh->mVertices;
			vec3f* texCoords = (vec3f*)mesh->mTextureCoords[0];

			std::vector<Mesh::V3T2> vertex_data;
			std::vector<unsigned int> index_data;

			for (size_t i = 0; i < mesh->mNumVertices; ++i) {
				Mesh::V3T2 v = { verts[i], vec2f(texCoords[i].x, texCoords[i].y) };
				vertex_data.push_back(v);
			}

			for (size_t f = 0; f < mesh->mNumFaces; ++f) {
				index_data.push_back(faces[f].mIndices[0]);
				index_data.push_back(faces[f].mIndices[1]);
				index_data.push_back(faces[f].mIndices[2]);
			}

			auto material = scene->mMaterials[mesh->mMaterialIndex];
			aiString aiPath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);

			size_t slash_pos = path.rfind('/');
			if (slash_pos == std::string::npos) slash_pos = path.rfind('\\');
			std::string folder_path = (slash_pos != std::string::npos) ? path.substr(0, slash_pos + 1) : "./";
			std::string texPath = folder_path + aiScene::GetShortFilename(aiPath.C_Str());

			auto mesh_ptr = std::make_shared<Mesh>(Mesh::Formats::F_V3T2, vertex_data.data(), vertex_data.size(), index_data.data(), index_data.size());
			mesh_ptr->texture = std::make_shared<Texture2D>(texPath);

			mesh_ptrs.push_back(mesh_ptr);
		}

		aiReleaseImport(scene);

		return mesh_ptrs;
	}
};