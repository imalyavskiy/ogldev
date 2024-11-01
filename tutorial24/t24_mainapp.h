#pragma once
#ifndef T24_MAIN_APP_H
#define T24_MAIN_APP_H

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

    ~MainApp() override;

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

    LightingTechnique* m_pLightingEffect;
    ShadowMapTechnique* m_pShadowMapEffect;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
    Texture* m_pGroundTex;
  };
}
#endif // T24_MAIN_APP_H

