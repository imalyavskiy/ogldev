#pragma once
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "util.h"

#include <vector>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = Vector3f(0.0f, 0.0f, 0.0f);
    }
};

class Main : public ICallbacks
{
public:

    Main();

    ~Main();

    bool Init();

    void Run();

    virtual void RenderSceneCB();

    virtual void IdleCB();

    virtual void SpecialKeyboardCB(int Key, int x, int y);

    virtual void KeyboardCB(unsigned char Key, int x, int y);

    virtual void PassiveMouseCB(int x, int y);

private:
    void CalcNormals(const std::vector<Vector3ui>& indices, std::vector<Vertex>& vertices);

    void CreateVertexBuffer(const std::vector<Vector3ui> indices);

    void CreateIndexBuffer(const std::vector<Vector3ui>& indices);


    GLuint m_VBO = GLuint(-1);
    GLuint m_IBO = GLuint(-1);
    LightingTechnique* m_pEffect = nullptr;
    Texture* m_pTexture = nullptr;
    Camera* m_pGameCamera = nullptr;
    float m_scale = 0.0;
    DirectionLight m_directionalLight;

    std::vector<Vector3ui> indices_;
    std::vector<Vertex> vertices_;
};