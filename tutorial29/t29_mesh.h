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

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "t29_util.h"
#include "t29_math_3d.h"
#include "t29_texture.h"
#include "t29_render_callbacks.h"

namespace t29
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

    ~Mesh() = default;

    bool LoadMesh(const std::string& fileName);

    void Render(IRenderCallbacks* pRenderCallbacks);
    
    void Render(unsigned int drawIndex, unsigned int primID);

  private:
    bool InitFromScene(const aiScene* pScene, const std::string& fileName);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& fileName);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
      MeshEntry();

      ~MeshEntry();

      bool Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

      GLuint VB;
      GLuint IB;
      unsigned int NumIndices;
      unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<std::shared_ptr<Texture>> m_Textures;
  };
}
#endif	/* MESH_H */

