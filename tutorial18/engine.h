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
#include <memory>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

#define TORUS
#ifndef TORUS
#define TETRAHEDRON
#endif

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

class Engine : public ICallbacks
{
public:

    Engine();

    ~Engine();

    bool Init();

    void Run();

    virtual void RenderSceneCB();

    virtual void IdleCB();

    virtual void SpecialKeyboardCB(int Key, int x, int y);

    virtual void KeyboardCB(unsigned char Key, int x, int y);

    virtual void PassiveMouseCB(int x, int y);

private:
    void CreateVertexBuffer();

    void CreateIndexBuffer();

    void CalcNormals();

    void BindBuffers();

    GLuint m_VBO = GLuint(-1);
    GLuint m_IBO = GLuint(-1);
    
    std::auto_ptr<LightingTechnique> m_pEffect;
    std::auto_ptr<Texture> m_pTexture;
    std::auto_ptr<Camera> m_pGameCamera;
    
    float m_scale = 0.0;
    DirectionLight m_directionalLight;

#if defined(TORUS)
    const GLfloat r1 = 0.6f;
    const GLfloat r2 = 0.2f;
    const GLuint SEGMENTS = 16;
    const GLuint SIDES = 25;

    const GLfloat Pi = 3.14159265358979323846f;
    const GLfloat twoPi = 2 * Pi;

    struct VertexParams {
        float phi;
        float psi;
    };
    std::vector<VertexParams> params_;
#endif
    std::vector<Vector3ui> indices_;
    std::vector<Vertex> vertices_;
};