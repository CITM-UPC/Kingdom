#pragma once
#include <memory>
#include <vector>
#include <string>

#include "GameObject.h"
#include "MeshInfo.h"
#include "Texture2D.h"
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

class MeshLoader
{
public:

	static std::vector<MeshInfo> loadMeshFromFile(const std::string& path)
	{
		std::vector<MeshInfo> meshInfoVec;

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

			auto meshInfo_ptr = std::make_unique<MeshInfo>(vertex_data.data(), vertex_data.size(), index_data.data(), index_data.size(), numTexCoords, numNormals, numFaces);

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D normal = mesh->mNormals[i];
				vec3f glmNormal(normal.x, normal.y, normal.z);
				meshInfo_ptr->mNormals.push_back(glmNormal);
			}

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D vert = mesh->mVertices[i];
				vec3f glmNormal(vert.x, vert.y, vert.z);
				meshInfo_ptr->mVertices.push_back(glmNormal);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];

				vec3f v0(mesh->mVertices[face.mIndices[0]].x, mesh->mVertices[face.mIndices[0]].y, mesh->mVertices[face.mIndices[0]].z);
				vec3f v1(mesh->mVertices[face.mIndices[1]].x, mesh->mVertices[face.mIndices[1]].y, mesh->mVertices[face.mIndices[1]].z);
				vec3f v2(mesh->mVertices[face.mIndices[2]].x, mesh->mVertices[face.mIndices[2]].y, mesh->mVertices[face.mIndices[2]].z);

				vec3f faceNormal = glm::cross(v1 - v0, v2 - v0);
				faceNormal = glm::normalize(faceNormal);
				meshInfo_ptr->mFaceNormals.push_back(faceNormal);

				vec3f faceCenter = (v0 + v1 + v2) / 3.0f;
				meshInfo_ptr->mFaceCenters.push_back(faceCenter);
			}

			meshInfoVec.push_back(std::move(*meshInfo_ptr));
		}

		aiReleaseImport(scene);

		return meshInfoVec;
	}

	static std::vector<std::unique_ptr<Texture2D>> loadTextureFromFile(GameObject& parentGO, const std::string& path)
	{
		std::vector<std::unique_ptr<Texture2D>> texture_ptrs;

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

			auto texture_ptr = std::make_unique<Texture2D>(parentGO, texPath);
			texture_ptr->path = path;

			if (scene->HasTextures()) {
				texture_ptr->height = scene->mTextures[0]->mHeight;
				texture_ptr->width = scene->mTextures[0]->mWidth;
			}
			else {
				texture_ptr->height = 1024; // assumption
				texture_ptr->width = 1024;
			}

			texture_ptrs.push_back(texture_ptr);
		}

		aiReleaseImport(scene);

		return texture_ptrs;
	}
};