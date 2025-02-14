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

#include "t28_util.h"
#include "t28_math_3d.h"
#include "t28_texture.h"

namespace t28
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;
    Vector3f m_tangent;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal, const Vector3f& Tangent)
    {
      m_pos    = pos;
      m_tex    = tex;
      m_normal = normal;
      m_tangent = Tangent;
                
    }
  };


  class Mesh
  {
  public:
    Mesh() = default;

    ~Mesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

  private:
    bool InitFromScene(const aiScene* pScene, const std::string& fileName);
    void InitMesh(unsigned int index, const aiMesh* pAIMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& fileName);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
      MeshEntry();

      ~MeshEntry();

      bool Init(const std::vector<Vertex>& Vertices,
                const std::vector<unsigned int>& Indices);

      GLuint VB;
      GLuint IB;
      unsigned int NumIndices;
      unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
  };
}


#endif	/* MESH_H */

