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

#include "t28_mesh.h"
#include "t28_engine_common.h"

namespace t28
{
  Mesh::MeshEntry::MeshEntry()
  {
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
  };

  Mesh::MeshEntry::~MeshEntry()
  {
    if (VB != INVALID_OGL_VALUE)
      glDeleteBuffers(1, &VB);

    if (IB != INVALID_OGL_VALUE)
      glDeleteBuffers(1, &IB);
  }

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                            const std::vector<unsigned int>& Indices)
  {
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
    
    return GLCheckError();
  }

  Mesh::~Mesh()
  {
    Clear();
  }

  void Mesh::Clear()
  {
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++)
      SAFE_DELETE(m_Textures[i]);
  }


  bool Mesh::LoadMesh(const std::string& Filename)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();
    
    bool ret = false;
    Assimp::Importer importer;

    const uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* pScene = importer.ReadFile(Filename.c_str(), flags);

    if (pScene)
      ret = InitFromScene(pScene, Filename);
    else
      printf("Error parsing '%s': '%s'\n", Filename.c_str(), importer.GetErrorString());

    return ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& fileName)
  {  
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, fileName);
  }

  void Mesh::InitMesh(unsigned int index, const aiMesh* pAIMesh)
  {
    m_Entries[index].MaterialIndex = pAIMesh->mMaterialIndex;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < pAIMesh->mNumVertices ; i++) {
      const aiVector3D* pPos      = &(pAIMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(pAIMesh->mNormals[i]);
      const aiVector3D* pTexCoord = pAIMesh->HasTextureCoords(0) ? &(pAIMesh->mTextureCoords[0][i]) : &Zero3D;
      const aiVector3D* pTangent  = &(pAIMesh->mTangents[i]);

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x, pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z),
               Vector3f(pTangent->x, pTangent->y, pTangent->z));
        
      vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < pAIMesh->mNumFaces ; i++) {
      const aiFace& face = pAIMesh->mFaces[i];
      assert(face.mNumIndices == 3);
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }
    
    m_Entries[index].Init(vertices, indices);
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& fileName)
  {
    // Extract the directory part from the file name
    const auto slashIndex = fileName.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos)
      dir = ".";
    else if (slashIndex == 0)
      dir = "/";
    else
      dir = fileName.substr(0, slashIndex);

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          std::string fullPath = dir + "/" + path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, fullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", fullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            ret = false;
          }
          else {
            printf("Loaded texture '%s'\n", fullPath.c_str());
          }
        }
      }
    }

    return ret;
  }

  void Mesh::Render()
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
      glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0)); // position
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12)); // texture coordinate
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20)); // normal
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(32)); // tangent

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

      const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
        m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
      }

      glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
