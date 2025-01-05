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
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "t44_util.h"
#include "t44_math_3d.h"
#include "t44_texture.h"
#include "t44_pipeline.h"

namespace t44 {
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

    void Render(uint32_t numInstances, const Matrix4f* WVPMats, const Matrix4f* worldMats);

    Orientation& GetOrientation() { return m_orientation; }

  private:
    bool InitFromScene(const aiScene* pScene, const std::string& fileName);
    void InitMesh(const aiMesh* paiMesh,
      std::vector<Vector3f>& positions,
      std::vector<Vector3f>& normals,
      std::vector<Vector2f>& texCoords,
      std::vector<uint32_t>& indices);

    bool InitMaterials(const aiScene* pScene, const std::string& fileName);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5

    GLuint m_VAO;
    GLuint m_Buffers[6];

    struct MeshEntry {
      MeshEntry()
      {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
        MaterialIndex = INVALID_MATERIAL;
      }

      uint32_t NumIndices;
      uint32_t BaseVertex;
      uint32_t BaseIndex;
      uint32_t MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
    Orientation m_orientation;
  };
}

#endif	/* MESH_H */

