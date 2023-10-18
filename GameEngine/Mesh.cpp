#include "Mesh.h"
#include <GL/glew.h>
#include "Engine_Globals.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <vector>
#include <array>

using namespace std;

std::vector<Mesh::Ptr> Mesh::loadFromFile(const std::string& path) {

    vector<Mesh::Ptr> mesh_ptrs;

    auto scene = aiImportFile(path.c_str(), aiProcess_Triangulate |aiProcess_FlipUVs);
    for (size_t m = 0; m < scene->mNumMeshes; ++m) {
        auto mesh = scene->mMeshes[m];
        auto faces = mesh->mFaces;
        vec3f* verts = (vec3f*)mesh->mVertices;
        vec3f* texCoords = (vec3f*)mesh->mTextureCoords[0];

        vector<V3T2> vertex_data;
        vector<unsigned int> index_data;

        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            V3T2 v = { verts[i], vec2f(texCoords[i].x, texCoords[i].y) };
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
        string texPath = aiScene::GetShortFilename(aiPath.C_Str());

        auto mesh_ptr = make_shared<Mesh>(Formats::F_V3T2, vertex_data.data(), vertex_data.size(), index_data.data(), index_data.size());
        mesh_ptr->texture = make_shared<Texture2D>(texPath);

        mesh_ptrs.push_back(mesh_ptr);
    }

    aiReleaseImport(scene);
    
    return mesh_ptrs;
}

Mesh::Mesh(Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* index_data, unsigned int numIndexs) : 
    _format(format),
    _numVerts(numVerts),
    _numIndexs(numIndexs)
{
    glGenBuffers(1, &_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);

    switch (_format) {
    case Formats::F_V3:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3) * numVerts, vertex_data, GL_STATIC_DRAW);
        break;
    case Formats::F_V3C4:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3C4) * numVerts, vertex_data, GL_STATIC_DRAW);
        break;
    case Formats::F_V3T2:
        glBufferData(GL_ARRAY_BUFFER, sizeof(V3T2) * numVerts, vertex_data, GL_STATIC_DRAW);
        break;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (index_data) {
        glGenBuffers(1, &_indexs_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndexs, index_data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
        _indexs_buffer_id = 0;
    }
}


Mesh::Mesh(Mesh&& b) noexcept :
    _format(b._format),
    _vertex_buffer_id(b._vertex_buffer_id),
    _numVerts(b._numVerts),
    _indexs_buffer_id(b._indexs_buffer_id),
    _numIndexs(b._numIndexs),
    texture(b.texture)
{
    b._vertex_buffer_id = 0;
    b._indexs_buffer_id = 0;

}

void Mesh::draw() {

    glColor4ub(255, 255, 255, 255);

    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_id);
    glEnableClientState(GL_VERTEX_ARRAY);

    switch (_format) {
    case Formats::F_V3:
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        break;
    case Formats::F_V3C4:
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(V3C4), nullptr);
        glColorPointer(4, GL_FLOAT, sizeof(V3C4), (void*)sizeof(V3));
        break;
    case Formats::F_V3T2:
        glEnable(GL_TEXTURE_2D);
        if (texture.get()) texture->bind();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(V3T2), nullptr);
        glTexCoordPointer(2, GL_FLOAT, sizeof(V3T2), (void*)sizeof(V3));
        break;
    }

    if (_indexs_buffer_id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexs_buffer_id);
        glDrawElements(GL_TRIANGLES, _numIndexs, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, _numVerts);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

Mesh::~Mesh() {
    if(_vertex_buffer_id) glDeleteBuffers(1, &_vertex_buffer_id);
    if(_indexs_buffer_id) glDeleteBuffers(1, &_indexs_buffer_id);
}