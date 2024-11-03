#pragma once
#ifndef T23_MAIN_APP_H
#define T23_MAIN_APP_H

#include <memory>
#include "t23_callbacks.h"
#include "t23_camera.h"
#include "t23_lighting_technique.h"
#include "t23_mesh.h"
#include "t23_shadow_map_fbo.h"
#include "t23_shadow_map_technique.h"

namespace t22
{
  class Camera;
}

namespace t23
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(GLint winWidth, GLint winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    virtual void ShadowMapPass();

    virtual void RenderPass();

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;

    void KeyboardCB(unsigned char Key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    GLint m_winWidth;
    GLint m_winHeight;

    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<ShadowMapTechnique> m_pShadowMapTech;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;

    SpotLight m_spotLight;
    std::shared_ptr<Mesh> m_pMesh;
    std::shared_ptr<Mesh> m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
  };
}

#endif // T23_MAIN_APP_H
