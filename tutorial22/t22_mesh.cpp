#include <cassert>

#include "t22_mesh.h"

namespace t22
{
  Vertex::Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
  {
    m_pos = pos;
    m_tex = tex;
    m_normal = normal;
  }

  Mesh::~Mesh()
  {
    Clear();
  }

  Mesh::MeshEntry::~MeshEntry()
  {
    if (VBO != INVALID_OGL_VALUE) 
      glDeleteBuffers(1, &VBO);

    if (IBO != INVALID_OGL_VALUE)
      glDeleteBuffers(1, &IBO);
  }

  bool Mesh::MeshEntry::Init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
  {
    NumIndices = indices.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    return true;
  }

  void Mesh::Clear()
  {
    for (auto& m_Texture : m_Textures)
      SAFE_DELETE(m_Texture);
  }

  bool Mesh::LoadMesh(const std::string& filename)
  {
    Clear();

    bool ret = false;

    Assimp::Importer importer;

    GLuint flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
    const aiScene* pAiScene = importer.ReadFile(filename.c_str(), flags);

    if (pAiScene)
      ret = InitFromScene(pAiScene, filename);
    else
      printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());

    return ret;
  }

  bool Mesh::InitFromScene(const aiScene* pScene, const std::string& filename)
  {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    for (GLuint i = 0; i < m_Entries.size(); ++i)
      InitMesh(i, pScene->mMeshes[i]);

    return InitMaterials(pScene, filename);
  }

  void Mesh::InitMesh(GLuint index, const aiMesh* pAiMesh)
  {
    m_Entries[index].MaterialIndex = pAiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i< pAiMesh->mNumVertices; i++){
      const aiVector3D* pPos      = &(pAiMesh->mVertices[i]);
      const aiVector3D* pNormal   = &(pAiMesh->mNormals[i]);
      const aiVector3D* pTexCoord = pAiMesh->HasTextureCoords(0) ?
                          &(pAiMesh->mTextureCoords[0][i]) : &Zero3D;

      Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
               Vector2f(pTexCoord->x,pTexCoord->y),
               Vector3f(pNormal->x, pNormal->y, pNormal->z));
      vertices.push_back(v);
    }

    for (unsigned int i = 0; i < pAiMesh->mNumFaces; i++){
      const aiFace& Face = pAiMesh->mFaces[i];
      assert(Face.mNumIndices == 3);
      indices.push_back(Face.mIndices[0]);
      indices.push_back(Face.mIndices[1]);
      indices.push_back(Face.mIndices[2]);
    }

    m_Entries[index].Init(vertices, indices);
  }

  bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
  {
    const std::string::size_type slashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (slashIndex == std::string::npos)
      Dir = ".";
    else if (slashIndex == 0)
      Dir = "/";
    else
      Dir = Filename.substr(0, slashIndex);

    bool ret = true;

    for (unsigned int i = 0; i< pScene->mNumMaterials; i++){
      const aiMaterial* pMaterial = pScene->mMaterials[i];

      m_Textures[i] = nullptr;
      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
        aiString path;

        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS){
          std::string FullPath = Dir + "/" + path.data;
          m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath);

          if (!m_Textures[i]->Load()){
            printf("Error loading texture '%s'\n", FullPath.c_str());
            delete m_Textures[i];
            m_Textures[i] = nullptr;
            ret = false;
          }
        }
      }

      if (!m_Textures[i]){
        m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");
        ret = m_Textures[i]->Load();
      }
    }

    return ret;
  }

  void Mesh::Render() const
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (const auto& entry : m_Entries)
    {
      glBindBuffer(GL_ARRAY_BUFFER, entry.VBO);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entry.IBO);

      const unsigned int MaterialIndex = entry.MaterialIndex;

      if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
        m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);

      glDrawElements(GL_TRIANGLES, entry.NumIndices, GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }
}
