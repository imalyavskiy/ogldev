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

#include "t26_mesh.h"
#include "t26_engine_common.h"
namespace t26
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
    {
      glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE)
    {
      glDeleteBuffers(1, &IB);
    }
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

    return true;
  }


  Mesh::~Mesh()
  {
    Clear();
  }


  void Mesh::Clear()
  {
    for (auto& m_Texture : m_Textures)
      SAFE_DELETE(m_Texture);
  }


  bool Mesh::LoadMesh(const std::string& filename)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();
    
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate |
                                                                aiProcess_GenSmoothNormals |
                                                                aiProcess_FlipUVs |
                                                                aiProcess_CalcTangentSpace);    
    if (pScene) {
      Ret = InitFromScene(pScene, filename);
    }
    else {
      printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
    }

    return Ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
  {  
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
  }

  void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
  {
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
      const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
      const aiVector3D* pTangent  = &(paiMesh->mTangents[i]);

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x, pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z),
               Vector3f(pTangent->x, pTangent->y, pTangent->z));
        
      Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      Indices.push_back(Face.mIndices[0]);
      Indices.push_back(Face.mIndices[1]);
      Indices.push_back(Face.mIndices[2]);
    }
    
    m_Entries[Index].Init(Vertices, Indices);
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
  {
    // Extract the directory part from the file name
    const std::string::size_type slashIndex = Filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos)
      dir = ".";
    else if (slashIndex == 0)
      dir = "/";
    else
      dir = Filename.substr(0, slashIndex);

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString Path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
          std::string FullPath = dir + "/" + Path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", FullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            ret = false;
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
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (auto& m_Entrie : m_Entries)
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_Entrie.VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0)); // position
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12)); // texture coordinate
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20)); // normal
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(32)); // tangent

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entrie.IB);

      const unsigned int MaterialIndex = m_Entrie.MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);

      glDrawElements(GL_TRIANGLES, m_Entrie.NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
  }
}
