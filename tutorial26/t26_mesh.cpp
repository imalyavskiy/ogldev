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

#define CALCULATE_TANGENT

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
    
    bool ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene =
#ifndef CALCULATE_TANGENT
      Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
#else
      Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );
#endif
    if (pScene)
      ret = InitFromScene(pScene, filename);
    else
      printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());

    return ret;
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

#ifdef CALCULATE_TANGENT
  void CalcTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
  {
    for (unsigned int i = 0; i < indices.size(); i += 3) {
      Vertex& v0 = vertices[indices[i + 0]];
      Vertex& v1 = vertices[indices[i + 1]];
      Vertex& v2 = vertices[indices[i + 2]];

      const Vector3f edge1 = v1.m_pos - v0.m_pos;
      const Vector3f edge2 = v2.m_pos - v0.m_pos;

      const float dU1 = v1.m_tex.x - v0.m_tex.x;
      const float dV1 = v1.m_tex.y - v0.m_tex.y;
      const float dU2 = v2.m_tex.x - v0.m_tex.x;
      const float dV2 = v2.m_tex.y - v0.m_tex.y;

      float f = 1.0f / (dU1 * dV2 - dU2 * dV1);

      Vector3f tangent, bitangent;

      tangent.x = f * (dV2 * edge1.x - dV1 * edge2.x);
      tangent.y = f * (dV2 * edge1.y - dV1 * edge2.y);
      tangent.z = f * (dV2 * edge1.z - dV1 * edge2.z);

      bitangent.x = f * (-dU2 * edge1.x - dU1 * edge2.x);
      bitangent.y = f * (-dU2 * edge1.y - dU1 * edge2.y);
      bitangent.z = f * (-dU2 * edge1.z - dU1 * edge2.z);

      v0.m_tangent += tangent;
      v1.m_tangent += tangent;
      v2.m_tangent += tangent;
    }

    for (auto& vertex : vertices)
      vertex.m_tangent.Normalize();
  }
#endif //  CALCULATE_TANGENT

  void Mesh::InitMesh(unsigned int index, const aiMesh* paiMesh)
  {
    m_Entries[index].MaterialIndex = paiMesh->mMaterialIndex;
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
      const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
#ifndef CALCULATE_TANGENT
      const aiVector3D* pTangent  = &(paiMesh->mTangents[i]);
#endif

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
        Vector2f(pTexCoord->x, pTexCoord->y),
        Vector3f(pNormal->x, pNormal->y, pNormal->z),
#ifndef CALCULATE_TANGENT               
        Vector3f(pTangent->x, pTangent->y, pTangent->z)
#else
        Vector3f(0.0f, 0.0f, 0.0f)
#endif
      );
        
      vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
      const aiFace& face = paiMesh->mFaces[i];
      assert(face.mNumIndices == 3);
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }

#ifdef CALCULATE_TANGENT
    CalcTangents(vertices, indices);
#endif // CALCULATE_TANGENT

    m_Entries[index].Init(vertices, indices);
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

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
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

    for (auto& entry : m_Entries)
    {
      glBindBuffer(GL_ARRAY_BUFFER, entry.VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0)); // position
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12)); // texture coordinate
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20)); // normal
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(32)); // tangent

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entry.IB);

      const unsigned int MaterialIndex = entry.MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);

      glDrawElements(GL_TRIANGLES, entry.NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
