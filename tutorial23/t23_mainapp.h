#pragma once
#ifndef T23_MAIN_APP_H
#define T23_MAIN_APP_H

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
      
    virtual ~MainApp();

    bool Init();

    void Run();

    virtual void RenderSceneCB();

    virtual void ShadowMapPass();

    virtual void RenderPass();

    virtual void IdleCB();

    virtual void SpecialKeyboardCB(int Key, int x, int y);

    virtual void KeyboardCB(unsigned char Key, int x, int y);

    virtual void PassiveMouseCB(int x, int y);

  private:

    GLint m_winWidth;
    GLint m_winHeight;

    LightingTechnique* m_pEffect;
    ShadowMapTechnique* m_pShadowMapTech;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
  };
}

#endif // T23_MAIN_APP_H
