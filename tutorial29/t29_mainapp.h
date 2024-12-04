#pragma once
#ifndef __T29_MAIN_APP_H__
#define __T29_MAIN_APP_H__

#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t29_util.h"
#include "t29_pipeline.h"
#include "t29_camera.h"
#include "t29_texture.h"
#include "t29_lighting_technique.h"
#include "t29_glut_backend.h"
#include "t29_mesh.h"
#include "t29_picking_texture.h"
#include "t29_picking_technique.h"
#include "t29_simple_color_technique.h"

namespace t29
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void PickingPhase();

    void RenderPhase();

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

    void MouseCB(int button, int state, int x, int y) override;

  private:
    int m_winWidth;
    int m_winHeight;

    LightingTechnique m_lightingEffect;
    PickingTechnique m_pickingEffect;
    SimpleColorTechnique m_simpleColorEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_directionalLight;
    std::shared_ptr<Mesh> m_pMesh;
    PickingTexture m_pickingTexture;
    struct {
      bool IsPressed;
      int x;
      int y;
    } m_leftMouseButton;
    Vector3f m_worldPos[2];
    PersProjInfo m_perspProjInfo;
  };
}

#endif // __T29_MAIN_APP_H__
