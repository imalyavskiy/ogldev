#pragma once
#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <memory>
#include "t25_callbacks.h"
#include "t25_camera.h"
#include "t25_lighting_technique.h"
#include "t25_skybox.h"

namespace t25
{
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

    GLint m_winWidth;
    GLint m_winHeight;

    std::shared_ptr<LightingTechnique> m_pLightingTechnique;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.f;
    DirectionalLight m_dirLight;
    std::shared_ptr<Mesh> m_pTankMesh;
    std::shared_ptr<SkyBox> m_pSkyBox;
    PersProjInfo m_persProjInfo;
  };
}

#endif // MAIN_APP_H