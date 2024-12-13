#pragma once
#ifndef __T34_MAIN_APP_H__
#define __T34_MAIN_APP_H__

#include <cmath>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t34_engine_common.h"
#include "t34_util.h"
#include "t34_pipeline.h"
#include "t34_camera.h"
#include "t34_texture.h"
#include "t34_lighting_technique.h"
#include "t34_glut_backend.h"
#include "t34_mesh.h"

#ifdef FREETYPE
#include "freetypeGL.h"
#endif

#ifdef FREETYPE
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
                   {.1,1.0,1.0,.5}, {1,1,1,0},
                   0, {1,0,0,1}, 0, {1,0,0,1},
                   0, {0,0,0,1}, 0, {0,0,0,1} };
#endif

namespace t34 {
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

    void MouseCB(int button, int state, int x, int y) override;

  private:

    void CalcFPS();

    void RenderFPS();

    int m_winWidth;
    int m_winHeight;

    std::unique_ptr<LightingTechnique> m_pEffect;
    std::unique_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;
    DirectionalLight m_directionalLight;
    Mesh m_mesh[4];
    Vector3f m_positions[4];
    float m_velocity[4];
    Vector4f m_colors[4];
    PersProjInfo m_persProjInfo;
#ifdef FREETYPE
    FontRenderer m_fontRenderer;
    FontRenderer m_fontRenderer2;
#endif
    int m_time;
    int m_frameCount = 0;
    float m_fps = 0.0f;
  };
}

#endif // __T34_MAIN_APP_H__
