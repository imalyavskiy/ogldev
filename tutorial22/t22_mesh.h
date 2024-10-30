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

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

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
        Mesh(){};
        ~Mesh(){
            Clear();
        };
        bool LoadMesh(const std::string& Filename);
        void Render();

    private:
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(unsigned int Index, const aiMesh* paiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();

        #define INVALID_MATERIAL 0xFFFFFFFF

        struct MeshEntry{
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

#endif /* MESH_H */
