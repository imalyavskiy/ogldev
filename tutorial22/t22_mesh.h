#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "t22_util.h"
#include "t22_math_3d.h"
#include "t22_texture.h"

namespace t22
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() = default;

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal);
  };

  class Mesh
  {
  public:
    Mesh() = default;
    ~Mesh();

    bool LoadMesh(const std::string& filename);
    void Render() const;

  private:
    bool InitFromScene(const aiScene* pScene, const std::string& filename);
    void InitMesh(GLuint index, const aiMesh* pAiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

    struct MeshEntry {
      MeshEntry() = default;
      ~MeshEntry();

      bool Init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

      GLuint VBO = INVALID_OGL_VALUE;
      GLuint IBO = INVALID_OGL_VALUE;

      GLuint NumIndices = 0;
      GLuint MaterialIndex = INVALID_MATERIAL;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
  };
}

#endif /* MESH_H */
