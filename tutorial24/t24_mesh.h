#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <GL/glew.h>

#include "t24_util.h"
#include "t24_math_3d.h"
#include "t24_texture.h"
namespace t24
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() = default;

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
      m_pos    = pos;
      m_tex    = tex;
      m_normal = normal;
    }
  };

  class Mesh
  {
  public:
    Mesh() = default;
    ~Mesh();

    bool LoadMesh(const std::string& filename);
    void Render();

  private:
    bool InitFromScene(const aiScene* pScene, const std::string& filename);
    void InitMesh(unsigned int index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry{
      MeshEntry() = default;
      ~MeshEntry();

      bool Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

      GLuint VB{INVALID_OGL_VALUE};
      GLuint IB{INVALID_OGL_VALUE};

      unsigned int NumIndices = 0;
      unsigned int MaterialIndex{INVALID_MATERIAL};
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
  };
}

#endif /* MESH_H */
