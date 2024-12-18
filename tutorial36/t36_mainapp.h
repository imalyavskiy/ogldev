#pragma once
#ifndef __T36_MAIN_APP_H__
#define __T36_MAIN_APP_H__

#include <cmath>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t36_engine_common.h"
#include "t36_util.h"
#include "t36_pipeline.h"
#include "t36_camera.h"
#include "t36_texture.h"
#include "t36_ds_geom_pass_tech.h"
#include "t36_ds_point_light_pass_tech.h"
#include "t36_ds_dir_light_pass_tech.h"
#include "t36_glut_backend.h"
#include "t36_mesh.h"
#ifdef FREETYPE
#include "freetypeGL.h"
#endif
#include "t36_gbuffer.h"
#include "t36_lights_common.h"

namespace t36
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight);


    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void DSGeometryPass();

    void BeginLightPasses();

    void DSPointLightsPass();

    void DSDirectionalLightPass();

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

    void MouseCB(int button, int state, int x, int y) override;

  private:

    // The calculation solves a quadratic equation (see http://en.wikipedia.org/wiki/Quadratic_equation)
    float CalcPointLightBoundingSphere(const PointLight& light);

    void InitLights();

    void InitBoxPositions();

    void CalcFPS();

    void RenderFPS();

    DSGeomPassTech m_DSGeomPassTech;
    DSPointLightPassTech m_DSPointLightPassTech;
    DSDirLightPassTech m_DSDirLightPassTech;

    int m_winWidth;
    int m_winHeight;

    std::unique_ptr<Camera> m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    DirectionalLight m_dirLight;
    PointLight m_pointLight[3];
    Mesh m_box;
    Mesh m_boundingSphere;
    Mesh m_quad;
    PerspProjInfo m_persProjInfo;
#ifdef FREETYPE
    FontRenderer m_fontRenderer;
#endif
    int m_time;
    int m_frameCount;
    float m_fps;
    GBuffer m_gbuffer;
    Vector3f m_boxPositions[5];
  };
}

#endif // __T36_MAIN_APP_H__
