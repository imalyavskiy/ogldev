#include <assert.h>

#include "t24_mesh.h"
namespace t24
{
  Mesh::MeshEntry::~MeshEntry()
  {
    if (VB != INVALID_OGL_VALUE) {
      glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE) {
      glDeleteBuffers(1, &IB);
    }
  }

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
  {
    NumIndices = indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
      &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices,
      &indices[0], GL_STATIC_DRAW);

    return true;
  }

  void Mesh::Clear()
  {
    for (unsigned int i = 0; i < m_Textures.size(); i++) {
      SAFE_DELETE(m_Textures[i]);
    }
  }

  Mesh::~Mesh()
  {
    Clear();
  }

  bool Mesh::LoadMesh(const std::string& filename)
  {
    Clear();

    bool Ret = false;

    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(filename.c_str(),
      aiProcess_Triangulate | aiProcess_GenSmoothNormals |
      aiProcess_FlipUVs);

    if (pScene) {
      Ret = InitFromScene(pScene, filename);
    }
    else {
      printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
    }

    return Ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& filename)
  {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      const aiMesh* paiMesh = pScene->mMeshes[i];
      InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, filename);
  }

  void Mesh::InitMesh(unsigned int index, const aiMesh* paiMesh)
  {
    m_Entries[index].MaterialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
      const aiVector3D* pPos = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
        &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
        Vector2f(pTexCoord->x, pTexCoord->y),
        Vector3f(pNormal->x, pNormal->y, pNormal->z));
      vertices.push_back(v);
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
      const aiFace& Face = paiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      indices.push_back(Face.mIndices[0]);
      indices.push_back(Face.mIndices[1]);
      indices.push_back(Face.mIndices[2]);
    }

    m_Entries[index].Init(vertices, indices);
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& filename)
  {
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos) {
      dir = ".";
    }
    else if (slashIndex == 0) {
      dir = "/";
    }
    else {
      dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;
      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path,
                                  nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS) {
          std::string FullPath = dir + "/" + path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath);

          if (!m_Textures[i]->Load()) {
            printf("Error loading texture '%s'\n", FullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            ret = false;
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

    for (unsigned int i = 0; i < m_Entries.size(); i++) {
      glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

      const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
      }

      glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
