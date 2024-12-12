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

#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "t32_util.h"
#include "t32_math_3d.h"
#include "t32_texture.h"

namespace t32 {
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
      m_pos = pos;
      m_tex = tex;
      m_normal = normal;
    }
  };


  class Mesh
  {
  public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const std::string& fileName);

    void Render();

  private:

    static constexpr uint32_t INVALID_MATERIAL = 0xFFFFFFFF;
    static constexpr uint32_t INDEX_BUFFER     = 0;
    static constexpr uint32_t POS_VB           = 1;
    static constexpr uint32_t NORMAL_VB        = 2;
    static constexpr uint32_t TEXCOORD_VB      = 3;

    bool InitFromScene(const aiScene* pScene, const std::string& fileName);
    void InitMesh(const aiMesh* paiMesh, std::vector<Vector3f>& positions, std::vector<Vector3f>& normals, std::vector<Vector2f>& texCoords, std::vector<uint32_t>& indices);

    bool InitMaterials(const aiScene* pScene, const std::string& fileName);
    void Clear();

    GLuint m_VAO;
    GLuint m_Buffers[4];

    struct MeshEntry {
      uint32_t NumIndices = 0;
      uint32_t BaseVertex = 0;
      uint32_t BaseIndex = 0;
      uint32_t MaterialIndex = INVALID_MATERIAL;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
  };
}

#endif	/* MESH_H */

