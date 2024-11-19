#pragma once

#include <iostream>
#include <cstdint>
#include <cmath>
#include <memory>
#include <array>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t18_pipeline.h"
#include "t18_camera.h"
#include "t18_texture.h"
#include "t18_lighting_technique.h"
#include "t18_callbacks.h"
#include "t18_util.h"

#ifndef __MAIN_APP_H__
#define __MAIN_APP_H__

namespace t18
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() = default;

    Vertex(Vector3f pos, Vector2f tex)
      : m_pos(pos)
      , m_tex(tex)
    { }
  };

  class MainApp : public ICallbacks
  {
  public:

    MainApp(GLuint winWidth, GLuint winHeight);

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int32_t Key, int32_t x, int32_t y) override;

    void KeyboardCB(uint8_t Key, int32_t x, int32_t y) override;

    void PassiveMouseCB(int32_t x, int32_t y) override;

  private:

    void CalcNormals(const uint32_t* pIndices, uint32_t IndexCount, Vertex* pVertices, uint32_t VertexCount);

    void CreateVertexBuffer(const uint32_t* pIndices, uint32_t IndexCount);

    void CreateIndexBuffer(const uint32_t* pIndices, uint32_t SizeInBytes);

    GLuint m_winWidth;
    GLuint m_winHeight;

    GLuint m_VBO = GL_INVALID_VALUE;
    GLuint m_IBO = GL_INVALID_VALUE;
    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.f;
    DirectionLight m_directionalLight;
  };
}
#endif // __MAIN_APP_H__