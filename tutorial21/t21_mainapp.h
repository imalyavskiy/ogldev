#pragma once
#ifndef T21_MAIN_APP_H
#define T21_MAIN_APP_H

#include <GL/glew.h>
#include "t21_lighting_technique.h"
#include "t21_math_3d.h"
#include "t21_camera.h"
#include "t21_texture.h"

namespace t21
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() = default;

    Vertex(const Vector3f& pos, const Vector2f& tex);
  };

  class MainApp : public ICallbacks
  {
  public:

    MainApp(GLint winWidth, GLint winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;

    void KeyboardCB(unsigned char Key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int vertexCount);

    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount);

    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes);

    GLint m_winWidth;
    GLint m_winHeight;

    GLuint m_VBO;
    GLuint m_IBO;
    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;
    DirectionalLight m_directionalLight;
  };
}
#endif // T21_MAIN_APP_H
