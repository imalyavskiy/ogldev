#pragma once
#ifndef __T32_MAIN_APP_H__
#define __T32_MAIN_APP_H__

#include <cmath>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t32_engine_common.h"
#include "t32_util.h"
#include "t32_pipeline.h"
#include "t32_camera.h"
#include "t32_texture.h"
#include "t32_lighting_technique.h"
#include "t32_glut_backend.h"
#include "t32_mesh.h"

namespace t32 {
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

    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;
    DirectionalLight m_directionalLight;
    std::shared_ptr<Mesh> m_pMesh1;
    std::shared_ptr<Mesh> m_pMesh2;
    std::shared_ptr<Mesh> m_pMesh3;
    PersProjInfo m_perspProjInfo;
  };
}

#endif // __T32_MAIN_APP_H__
