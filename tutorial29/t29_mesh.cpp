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

#include "t29_mesh.h"

#include "t29_engine_common.h"

namespace t29
{
  Vertex::Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal) : m_pos(pos)
    , m_tex(tex)
    , m_normal(normal)
  {
  }

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

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
  {
    NumIndices = indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &indices[0], GL_STATIC_DRAW);
    
    return GLCheckError();
  }

  void Mesh::Clear()
  {
    for (auto& texture : m_Textures)
      texture.reset();
  }

  bool Mesh::LoadMesh(const std::string& fileName)
  {
    // Release the previously loaded mesh (if it exists)
    Clear();
    
    bool Ret = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (pScene)
      Ret = InitFromScene(pScene, fileName);
    else
      printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());

    return Ret;
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

  void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
  {
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
      const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x, pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z));

      vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      indices.push_back(Face.mIndices[0]);
      indices.push_back(Face.mIndices[1]);
      indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].Init(vertices, indices);
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& fileName)
  {
    // Extract the directory part from the file name
    const std::string::size_type slashIndex = fileName.find_last_of("/");
    std::string Dir;

    if (slashIndex == std::string::npos)
      Dir = ".";
    else if (slashIndex == 0)
      Dir = "/";
    else
      Dir = fileName.substr(0, slashIndex);

    bool ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString Path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
          std::string p(Path.data);
                
          if (p.substr(0, 2) == ".\\")
            p = p.substr(2, p.size() - 2);

          std::string fullPath = Dir + "/" + p;
                    
          m_Textures[i] = std::make_shared<Texture>(GL_TEXTURE_2D, fullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", fullPath.c_str());
            m_Textures[i].reset();
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
    
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
      glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

      const unsigned int materialIndex = m_Entries[i].MaterialIndex;

      if (materialIndex < m_Textures.size() && m_Textures[materialIndex]) {
        m_Textures[materialIndex]->Bind(COLOR_TEXTURE_UNIT);
      }

      if (pRenderCallbacks) {
        pRenderCallbacks->DrawStartCB(i);
      }
        
      glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }


  void Mesh::Render(unsigned int drawIndex, unsigned int primID)
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

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)(primID * 3 * sizeof(GLuint)));

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
