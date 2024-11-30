#pragma once
#ifndef __T26_MAIN_APP_H__
#define __T26_MAIN_APP_H__

#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>

#include "t26_engine_common.h"
#include "t26_util.h"
#include "t26_pipeline.h"
#include "t26_camera.h"
#include "t26_texture.h"
#include "t26_lighting_technique.h"
#include "t26_glut_backend.h"
#include "t26_mesh.h"

namespace t26
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(GLint winWidth, GLint winHeight);

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;

    void KeyboardCB(unsigned char Key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:
    const GLint m_winWidth;
    const GLint m_winHeight;

    float m_scale = 0.0f;

    std::shared_ptr<LightingTechnique> m_pLightingTechnique;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_dirLight;
    std::shared_ptr<Mesh> m_pSphereMesh;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Texture> m_pNormalMap;
    std::shared_ptr<Texture> m_pTrivialNormalMap;
    PersProjInfo m_persProjInfo;
    bool m_bumpMapEnabled = true;
  };
}
#endif // __T26_MAIN_APP_H__
