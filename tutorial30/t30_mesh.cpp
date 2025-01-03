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

#include "t30_engine_common.h"
#include "t30_mesh.h"

namespace t30
{
  Mesh::MeshEntry::~MeshEntry()
  {
    if (VB != INVALID_OGL_VALUE)
      glDeleteBuffers(1, &VB);

    if (IB != INVALID_OGL_VALUE)
      glDeleteBuffers(1, &IB);
  }

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
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
    for (auto& texture : m_Textures)
      SAFE_DELETE(texture);
  }

  bool Mesh::LoadMesh(const std::string& Filename)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
      Ret = InitFromScene(pScene, Filename);
    }
    else {
      printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
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

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x, pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z));

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
    const auto slashIndex = Filename.find_last_of("/");
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

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          std::string p(Path.data);

          if (p.substr(0, 2) == ".\\") {
            p = p.substr(2, p.size() - 2);
          }

          std::string fullPath = dir + "/" + p;

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


  void Mesh::Render(IRenderCallbacks* pRenderCallbacks)
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) 
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

      const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);

      if (pRenderCallbacks)
        pRenderCallbacks->DrawStartCB(i);
        
      glDrawElements(GL_PATCHES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }


  void Mesh::Render(unsigned int drawIndex, unsigned int primId)
  {
    assert(drawIndex < m_Entries.size());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_Entries[drawIndex].VB);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[drawIndex].IB);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(primId * 3 * sizeof(GLuint)));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}