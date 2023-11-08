#pragma once
#include <memory>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Mesh.h"
#include "Texture2D.h"
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

class MeshLoader
{
public:

	static std::vector<std::shared_ptr<Mesh>> loadMeshFromFile(GameObject& parentGO, const std::string& path)
	{
		std::vector<std::shared_ptr<Mesh>> mesh_ptrs;

		auto scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
		for (size_t m = 0; m < scene->mNumMeshes; ++m) {
			auto mesh = scene->mMeshes[m];
			auto faces = mesh->mFaces;

			int numTexCoords = mesh->mNumVertices; // Number of texture coordinates is same as the number of vertices
			int numNormals = mesh->mNumVertices; // Number of normals is same as the number of vertices
			int numFaces = mesh->mNumFaces;

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

			auto mesh_ptr = std::make_shared<Mesh>(parentGO, Mesh::Formats::F_V3T2, vertex_data.data(), vertex_data.size(), index_data.data(), index_data.size(), numTexCoords, numNormals, numFaces);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D normal = mesh->mNormals[i];
				vec3f glmNormal(normal.x, normal.y, normal.z);
				mesh_ptr->mNormals.push_back(glmNormal);
			}

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D vert = mesh->mVertices[i];
				vec3f glmNormal(vert.x, vert.y, vert.z);
				mesh_ptr->mVertices.push_back(glmNormal);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];

				vec3f v0(mesh->mVertices[face.mIndices[0]].x, mesh->mVertices[face.mIndices[0]].y, mesh->mVertices[face.mIndices[0]].z);
				vec3f v1(mesh->mVertices[face.mIndices[1]].x, mesh->mVertices[face.mIndices[1]].y, mesh->mVertices[face.mIndices[1]].z);
				vec3f v2(mesh->mVertices[face.mIndices[2]].x, mesh->mVertices[face.mIndices[2]].y, mesh->mVertices[face.mIndices[2]].z);

				vec3f faceNormal = glm::cross(v1 - v0, v2 - v0);
				faceNormal = glm::normalize(faceNormal);
				mesh_ptr->mFaceNormals.push_back(faceNormal);

				vec3f faceCenter = (v0 + v1 + v2) / 3.0f;
				mesh_ptr->mFaceCenters.push_back(faceCenter);
			}

			mesh_ptrs.push_back(mesh_ptr);
		}

		aiReleaseImport(scene);

		return mesh_ptrs;
	}

	static std::vector<std::shared_ptr<Texture2D>> loadTextureFromFile(GameObject& parentGO, const std::string& path)
	{
		std::vector<std::shared_ptr<Texture2D>> texture_ptrs;

		auto scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
		for (size_t m = 0; m < scene->mNumMeshes; ++m) {
			auto mesh = scene->mMeshes[m];

			auto material = scene->mMaterials[mesh->mMaterialIndex];
			aiString aiPath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);

			size_t slash_pos = path.rfind('/');
			if (slash_pos == std::string::npos) slash_pos = path.rfind('\\');
			std::string folder_path = (slash_pos != std::string::npos) ? path.substr(0, slash_pos + 1) : "./";
			std::string texPath = folder_path + aiScene::GetShortFilename(aiPath.C_Str());

			auto texture_ptr = std::make_shared<Texture2D>(parentGO, texPath);

			texture_ptrs.push_back(texture_ptr);
		}

		aiReleaseImport(scene);

		return texture_ptrs;
	}
};