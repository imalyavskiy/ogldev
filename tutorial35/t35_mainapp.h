#pragma once
#ifndef __T35_MAIN_APP_H__
#define __T35_MAIN_APP_H__

#include <cmath>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t35_engine_common.h"
#include "t35_util.h"
#include "t35_pipeline.h"
#include "t35_camera.h"
#include "t35_texture.h"
#include "t35_ds_geom_pass_tech.h"
#include "t35_glut_backend.h"
#include "t35_mesh.h"

#ifdef FREETYPE
#include "freetypeGL.h"
#endif

#include "t35_gbuffer.h"

namespace t35
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run()
    {
      GLUTBackendRun(this);
    }


    void RenderSceneCB() override;


    void DSGeometryPass();

    void DSLightPass();

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;


    void KeyboardCB(unsigned char Key, int x, int y) override;


    void PassiveMouseCB(int x, int y) override;


    void MouseCB(int Button, int State, int x, int y) override;

  private:

    void CalcFPS();

    void RenderFPS();

    int m_winWidth;
    int m_winHeight;

    DSGeomPassTech m_DSGeomPassTech;
    std::unique_ptr<Camera> m_pGameCamera;
    float m_scale;
    Mesh m_mesh;
    PersProjInfo m_persProjInfo;
#ifdef FREETYPE
    FontRenderer m_fontRenderer;
#endif
    int m_time;
    int m_frameCount;
    float m_fps;
    GBuffer m_gbuffer;
  };
}

#endif // __T35_MAIN_APP_H__
