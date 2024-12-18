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

#include <assert.h>

#include "t37_mesh.h"

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define WVP_LOCATION 3
#define WORLD_LOCATION 7

namespace t37
{
  Mesh::Mesh()
  {
    m_VAO = 0;
    ZERO_MEM(m_buffers);
  }


  Mesh::~Mesh()
  {
    Clear();
  }


  void Mesh::Clear()
  {
    for (unsigned int i = 0; i < m_Textures.size(); i++) {
      SAFE_DELETE(m_Textures[i]);
    }

    if (m_buffers[0] != 0) {
      glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
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
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
      Ret = InitFromScene(pScene, fileName);
    }
    else {
      printf("Error parsing '%s': '%s'\n", fileName.c_str(), Importer.GetErrorString());
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
    std::vector<unsigned int> indices;

    uint32_t numVertices = 0;
    uint32_t numIndices = 0;

    // Count the number of vertices and indices
    for (uint32_t i = 0; i < m_Entries.size(); i++) {
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
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(paiMesh, positions, normals, texCoords, indices);
    }

    if (!InitMaterials(pScene, fileName)) {
      return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WVP_MAT_VB]);

    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(WVP_LOCATION + i);
      glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), (const GLvoid*)(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(WVP_LOCATION + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_MAT_VB]);

    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(WORLD_LOCATION + i);
      glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), (const GLvoid*)(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(WORLD_LOCATION + i, 1);
    }

    return GLCheckError();
  }

  void Mesh::InitMesh(const aiMesh* paiMesh, std::vector<Vector3f>& positions, std::vector<Vector3f>& normals, std::vector<Vector2f>& texCoords, std::vector<unsigned int>& indices)
  {
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
      const aiVector3D* pPos = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

      positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
      normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
      texCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
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
    std::string Dir;

    if (slashIndex == std::string::npos) {
      Dir = ".";
    }
    else if (slashIndex == 0) {
      Dir = "/";
    }
    else {
      Dir = fileName.substr(0, slashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString aiPath;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          std::string path(aiPath.data);

          if (path.substr(0, 2) == ".\\")
            path = path.substr(2, path.size() - 2);

          std::string fullPath = Dir + "/" + path;

          m_Textures[i] = new Texture(GL_TEXTURE_2D, fullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", fullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            Ret = false;
          }
          else {
            printf("Loaded texture '%s'\n", fullPath.c_str());
          }
        }
      }
    }

    return Ret;
  }


  void Mesh::Render()
  {
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const unsigned int materialIndex = m_Entries[i].MaterialIndex;

      assert(materialIndex < m_Textures.size());

      if (m_Textures[materialIndex]) {
        m_Textures[materialIndex]->Bind(GL_TEXTURE0);
      }

      glDrawElementsBaseVertex(GL_TRIANGLES,
        m_Entries[i].NumIndices,
        GL_UNSIGNED_INT,
        (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
        m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
  }

  void Mesh::Render(unsigned int numInstances, const Matrix4f* WVPMats, const Matrix4f* WorldMats)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, WorldMats, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

      assert(MaterialIndex < m_Textures.size());

      if (m_Textures[MaterialIndex]) {
        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
      }

      glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
        m_Entries[i].NumIndices,
        GL_UNSIGNED_INT,
        (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
        numInstances,
        m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
  }
}