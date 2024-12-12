#pragma once
#ifndef __T33_MAIN_APP_H__
#define __T33_MAIN_APP_H__

#include <cmath>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t33_engine_common.h"
#include "ogldev_util.h"
#include "t33_pipeline.h"
#include "t33_camera.h"
#include "t33_texture.h"
#include "t33_lighting_technique.h"
#include "t33_glut_backend.h"
#include "t33_mesh.h"

#ifdef FREETYPE
#include "freetypeGL.h"
#endif

#define NUM_ROWS 50
#define NUM_COLS 20
#define NUM_INSTANCES NUM_ROWS * NUM_COLS

namespace t33 {
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

    void CalcPositions();

    int m_winWidth;
    int m_winHeight;

    std::unique_ptr<LightingTechnique> m_pEffect;
    std::unique_ptr<Camera> m_pGameCamera;
    float m_scale = 0.f;
    DirectionalLight m_directionalLight;
    std::unique_ptr<Mesh> m_pMesh;
    PersProjInfo m_persProjInfo;
#ifdef FREETYPE
    FontRenderer m_fontRenderer;
#endif
    int m_time;
    int m_frameCount = 0;
    float m_fps = 0.f;
    Vector3f m_positions[NUM_INSTANCES];
    float m_velocity[NUM_INSTANCES];
  };
}

#endif // __T33_MAIN_APP_H__
