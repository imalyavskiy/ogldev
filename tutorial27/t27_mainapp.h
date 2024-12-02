#pragma once
#ifndef __T27_MAIN_APP_H__
#define __T27_MAIN_APP_H__

#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t27_engine_common.h"
#include "t27_util.h"
#include "t27_pipeline.h"
#include "t27_camera.h"
#include "t27_texture.h"
#include "t27_lighting_technique.h"
#include "t27_glut_backend.h"
#include "t27_mesh.h"
#include "t27_billboard_list.h"
#include "t27_callbacks.h"


namespace t27
{
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

  private:
    int m_winWidth = 0;
    int m_winHeight = 0;

    std::shared_ptr<LightingTechnique> m_pLightingTechnique;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_dirLight;
    std::shared_ptr<Mesh> m_pGround;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Texture> m_pNormalMap;
    PersProjInfo m_persProjInfo;
    BillboardList m_billboardList;
  };
}
#endif // __T27_MAIN_APP_H__
