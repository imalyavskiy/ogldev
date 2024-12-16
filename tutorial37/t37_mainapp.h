#pragma once
#ifndef __T37_MAIN_APP_H__
#define __T37_MAIN_APP_H__

#include <cmath>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t37_engine_common.h"
#include "t37_util.h"
#include "t37_pipeline.h"
#include "t37_camera.h"
#include "t37_texture.h"
#include "t37_ds_geom_pass_tech.h"
#include "t37_ds_point_light_pass_tech.h"
#include "t37_ds_dir_light_pass_tech.h"
#include "t37_null_technique.h"
#include "t37_glut_backend.h"
#include "t37_mesh.h"
#ifdef FREETYPE
#include "freetypeGL.h"
#endif
#include "t37_gbuffer.h"
#include "t37_lights_common.h"

namespace t37
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

    void DSStencilPass(unsigned int PointLightIndex);

    void DSPointLightPass(unsigned int pointLightIndex);

    void DSDirectionalLightPass();

    void DSFinalPass();
    
    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

    void MouseCB(int button, int state, int x, int y) override;

  private:

    // The calculation solves a quadratic equation (see http://en.wikipedia.org/wiki/Quadratic_equation)
    float CalcPointLightBSphere(const PointLight& light);

    void InitLights();

    void InitBoxPositions();

    void CalcFPS();

    void RenderFPS();

    int m_winWidth;
    int m_winHeight;

    DSGeomPassTech m_DSGeomPassTech;
    DSPointLightPassTech m_DSPointLightPassTech;
    DSDirLightPassTech m_DSDirLightPassTech;
    NullTechnique m_nullTech;
    std::unique_ptr<Camera> m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    DirectionalLight m_dirLight;
    PointLight m_pointLight[3];
    Mesh m_box;
    Mesh m_bsphere;
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

#endif // __T37_MAIN_APP_H__
