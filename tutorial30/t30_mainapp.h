#pragma once
#ifndef __MAIN_APP_H__
#define __MAIN_APP_H__

#include <iostream>
#include <format>
#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t30_engine_common.h"
#include "t30_util.h"
#include "t30_pipeline.h"
#include "t30_camera.h"
#include "t30_texture.h"
#include "t30_lighting_technique.h"
#include "t30_glut_backend.h"
#include "t30_mesh.h"

namespace t30
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight);

    ~MainApp() override;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

    void MouseCB(int button, int state, int x, int y) override;

  private:
    int m_winWidth = 0;
    int m_winHeight = 0;

    LightingTechnique m_lightingEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_directionalLight;
    std::shared_ptr<Mesh> m_pMesh;
    PersProjInfo m_persProjInfo;
    std::shared_ptr<Texture> m_pDisplacementMap;
    std::shared_ptr<Texture> m_pColorMap;
    float m_dispFactor;
    bool m_isWireframe;
  };
}

#endif // __MAIN_APP_H__