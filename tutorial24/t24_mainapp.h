#pragma once
#ifndef T24_MAIN_APP_H
#define T24_MAIN_APP_H

#include <memory>
#include "t24_callbacks.h"
#include "t24_camera.h"
#include "t24_lighting_technique.h"
#include "t24_mesh.h"
#include "t24_shadow_map_fbo.h"
#include "t24_shadow_map_technique.h"

namespace t24
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

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:
    GLint m_winWidth;
    GLint m_winHeight;

    std::shared_ptr<LightingTechnique> m_pLightingEffect;
    std::shared_ptr<ShadowMapTechnique> m_pShadowMapEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;
    SpotLight m_spotLight;
    std::shared_ptr<Mesh> m_pMesh;
    std::shared_ptr<Mesh> m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
    std::shared_ptr<Texture> m_pGroundTex;
  };
}
#endif // T24_MAIN_APP_H

