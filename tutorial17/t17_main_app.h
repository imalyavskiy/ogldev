#pragma once
#ifndef __MAIN_APP_H__
#define __MAIN_APP_H__ 

#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t17_callbacks.h"
#include "t17_pipeline.h"
#include "t17_camera.h"
#include "t17_texture.h"
#include "t17_lighting_technique.h"

namespace t17
{
  class MainApp : public ICallbacks
  {
  public:
    MainApp(int winWidth, int winHeight, float fov, float zFar, float zNear);

    ~MainApp() = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    void CreateVertexBuffer();

    void CreateIndexBuffer();

    GLint   m_winWidth;
    GLint   m_winHeight;
    GLfloat m_verticalFoV;
    GLfloat m_zNear;
    GLfloat m_zFar;

    GLuint m_VBO = 0xFFFFFFFF;
    GLuint m_IBO = 0xFFFFFFFF;

    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Camera> m_pGameCamera;

    float m_scale = 0.f;

    DirectionLight m_directionalLight{ { 1.0f, 1.0f, 1.0f }, 0.5f };
  };
}

#endif