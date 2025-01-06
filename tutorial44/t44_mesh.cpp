/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>

#include "t44_mesh.h"
#include "t44_engine_common.h"

#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2

namespace t44 {
  Mesh::Mesh()
  {
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
  }


  Mesh::~Mesh()
  {
    Clear();
  }


  void Mesh::Clear()
  {
    for (uint32_t i = 0; i < m_Textures.size(); i++) {
      SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
      glDeleteBuffers(std::size(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
      glDeleteVertexArrays(1, &m_VAO);
      m_VAO = 0;
    }
  }


  bool Mesh::LoadMesh(const std::string& fileName)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(std::size(m_Buffers), m_Buffers);

    bool Ret = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
      Ret = InitFromScene(pScene, fileName);
    }
    else {
      printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    return Ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& fileName)
  {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    std::vector<Vector3f> positions;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> texCoords;
    std::vector<uint32_t> indices;

    uint32_t numVertices = 0;
    uint32_t numIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
      m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
      m_Entries[i].BaseVertex = numVertices;
      m_Entries[i].BaseIndex = numIndices;

      numVertices += pScene->mMeshes[i]->mNumVertices;
      numIndices += m_Entries[i].NumIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    indices.reserve(numIndices);

    // Initialize the meshes in the scene one by one
    for (uint32_t i = 0; i < m_Entries.size(); i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(paiMesh, positions, normals, texCoords, indices);
    }

    if (!InitMaterials(pScene, fileName))
      return false;

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    return GLCheckError();
  }

  void Mesh::InitMesh(const aiMesh* paiMesh,
    std::vector<Vector3f>& positions,
    std::vector<Vector3f>& normals,
    std::vector<Vector2f>& texCoords,
    std::vector<uint32_t>& indices)
  {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
      const aiVector3D* pPos = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

      positions.emplace_back(pPos->x, pPos->y, pPos->z);
      normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
      texCoords.emplace_back(pTexCoord->x, pTexCoord->y);
    }

    // Populate the index buffer
    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
      const aiFace& face = paiMesh->mFaces[i];
      assert(face.mNumIndices == 3);
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& fileName)
  {
    // Extract the directory part from the file name
    std::string::size_type slashIndex = fileName.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos)
      dir = ".";
    else if (slashIndex == 0)
      dir = "/";
    else
      dir = fileName.substr(0, slashIndex);

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          std::string p(path.data);

          if (p.substr(0, 2) == ".\\") {
            p = p.substr(2, p.size() - 2);
          }

          std::string FullPath = dir + "/" + p;

          m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", FullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            //  Ret = false;
          }
          else {
            printf("Loaded texture '%s'\n", FullPath.c_str());
          }
        }
      }
    }

    return ret;
  }


  void Mesh::Render()
  {
    glBindVertexArray(m_VAO);

    for (uint32_t i = 0; i < m_Entries.size(); i++) 
    {
      const uint32_t MaterialIndex = m_Entries[i].MaterialIndex;

      assert(MaterialIndex < m_Textures.size());

      if (m_Textures[MaterialIndex]) {
        m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
      }

      glDrawElementsBaseVertex(GL_TRIANGLES,
        m_Entries[i].NumIndices,
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(sizeof(uint32_t) * m_Entries[i].BaseIndex),
        m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
  }

  void Mesh::Render(const unsigned int numInstances, const Matrix4f* WVPMats, const Matrix4f* worldMats)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, worldMats, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const uint32_t MaterialIndex = m_Entries[i].MaterialIndex;

      assert(MaterialIndex < m_Textures.size());

      if (m_Textures[MaterialIndex]) {
        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
      }

      glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
        m_Entries[i].NumIndices,
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(sizeof(uint32_t) * m_Entries[i].BaseIndex),
        numInstances,
        m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
  }
}
