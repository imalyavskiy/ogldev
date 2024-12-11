#pragma once
#ifndef __T31_MAIN_APP_H__
#define __T31_MAIN_APP_H__

#include <cmath>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t31_engine_common.h"
#include "t31_util.h"
#include "t31_pipeline.h"
#include "t31_camera.h"
#include "t31_texture.h"
#include "t31_lighting_technique.h"
#include "t31_glut_backend.h"
#include "t31_mesh.h"

namespace t31 {
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
    int m_winWidth;
    int m_winHeight;

    LightingTechnique m_lightingEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_directionalLight;
    std::shared_ptr<Mesh> m_pMesh;
    PersProjInfo m_persProjInfo;
    float m_tessellationLevel;
    bool m_isWireframe;
  };
}

#endif // __T31_MAIN_APP_H__