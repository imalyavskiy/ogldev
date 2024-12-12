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

#include "t33_mesh.h"

using namespace std;

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define WVP_LOCATION 3
#define WORLD_LOCATION 7

namespace t33 {
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
    for (unsigned int i = 0; i < m_Textures.size(); i++) {
      SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
      glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
      glDeleteVertexArrays(1, &m_VAO);
      m_VAO = 0;
    }
  }


  bool Mesh::LoadMesh(const string& fileName)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool ret = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
      ret = InitFromScene(pScene, fileName);
    }
    else {
      printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    return ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const string& fileName)
  {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    vector<Vector3f> positions;
    vector<Vector3f> normals;
    vector<Vector2f> texCoords;
    vector<unsigned int> indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
      m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
      m_Entries[i].BaseVertex = NumVertices;
      m_Entries[i].BaseIndex = NumIndices;

      NumVertices += pScene->mMeshes[i]->mNumVertices;
      NumIndices += m_Entries[i].NumIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(NumVertices);
    normals.reserve(NumVertices);
    texCoords.reserve(NumVertices);
    indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(paiMesh, positions, normals, texCoords, indices);
    }

    if (!InitMaterials(pScene, fileName)) {
      return false;
    }

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

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);

    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(WVP_LOCATION + i);
      glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(WVP_LOCATION + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);

    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(WORLD_LOCATION + i);
      glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(WORLD_LOCATION + i, 1);
    }

    return GLCheckError();
  }

  void Mesh::InitMesh(const aiMesh* paiMesh, vector<Vector3f>& positions, vector<Vector3f>& normals, vector<Vector2f>& texCoords, vector<unsigned int>& indices)
  {
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
      const aiVector3D* pPos = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

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

  bool Mesh::InitMaterials(const aiScene* pScene, const string& Filename)
  {
    // Extract the directory part from the file name
    const string::size_type slashIndex = Filename.find_last_of("/");
    string dir;

    if (slashIndex == string::npos)
      dir = ".";
    else if (slashIndex == 0)
      dir = "/";
    else
      dir = Filename.substr(0, slashIndex);

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          string p(path.data);

          if (p.substr(0, 2) == ".\\") {
            p = p.substr(2, p.size() - 2);
          }

          string fullPath = dir + "/" + p;

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


  void Mesh::Render(unsigned int numInstances, const Matrix4f* matricesWorldViewProjection, const Matrix4f* matricesWorld)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, matricesWorldViewProjection, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * numInstances, matricesWorld, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const unsigned int materialIndex = m_Entries[i].MaterialIndex;

      assert(materialIndex < m_Textures.size());

      if (m_Textures[materialIndex]) {
        m_Textures[materialIndex]->Bind(GL_TEXTURE0);
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
