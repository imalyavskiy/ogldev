#include <assert.h>

#include "t25_mesh.h"

namespace t25
{
  Mesh::MeshEntry::MeshEntry()
  {
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    NumIndices = 0;
    MaterialIndex = INVALID_MATERIAL;
  }

  Mesh::MeshEntry::~MeshEntry()
  {
    if (VB != INVALID_OGL_VALUE){
      glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE){
      glDeleteBuffers(1, &IB);
    }
  }

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                        const std::vector<unsigned int>& Indices)
  {
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Vertices.size(),
                &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*NumIndices,
                &Indices[0], GL_STATIC_DRAW);

    return true;
  }

  void Mesh::Clear()
  {
    for (unsigned int i = 0; i < m_Textures.size(); i++){
      SAFE_DELETE(m_Textures[i]);
    }
  }

  bool Mesh::LoadMesh(const std::string& Filename)
  {
    Clear();

    bool ret = false;

    Assimp::Importer Importer;

    uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
    const aiScene* pAiScene = Importer.ReadFile(Filename.c_str(), flags);

    if (pAiScene)
      ret = InitFromScene(pAiScene, Filename);
    else
      printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());

    return ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
  {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    for (unsigned int i = 0; i < m_Entries.size(); i++){
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
    for (unsigned int i = 0; i< paiMesh->mNumVertices; i++){
      const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
                          &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x,pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z));
      Vertices.push_back(v);
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++){
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
    std::string::size_type slashIndex = Filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos)
      dir = ".";
    else if (slashIndex == 0)
      dir = "/";
    else
      dir = Filename.substr(0, slashIndex);

    bool ret = true;

    for (unsigned int i = 0; i< pScene->mNumMaterials; i++){
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;
      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
        aiString Path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
        {
          std::string fullPath = dir + "/" + Path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

          if (!m_Textures[i]->Load()){
            printf("Error loading texture '%s'\n", fullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            ret = false;
          }
        }
      }

      /*if (!m_Textures[i]){
          m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");
          Ret = m_Textures[i]->Load();
      }*/
    }

    return ret;
  }

  void Mesh::Render()
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0; i < m_Entries.size(); i++){
      glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

      const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]){
        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
      }

      glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
