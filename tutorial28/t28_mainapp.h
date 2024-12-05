#pragma once
#ifndef __T28_MAIN_APP_H__
#define __T28_MAIN_APP_H__
#include <memory>
#include <cassert>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <chrono>
#include <thread>

#include "t28_engine_common.h"
#include "t28_util.h"
#include "t28_pipeline.h"
#include "t28_camera.h"
#include "t28_texture.h"
#include "t28_lighting_technique.h"
#include "t28_glut_backend.h"
#include "t28_mesh.h"
#include "t28_particle_system.h"

namespace t28
{
  static long long GetCurrentTimeMillis()
  {
    const auto duration = std::chrono::system_clock::now().time_since_epoch();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
  }

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

    long long m_currentTimeMillis;
    std::shared_ptr<LightingTechnique> m_pLightingTechnique;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_dirLight;
    std::shared_ptr<Mesh> m_pGround;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Texture> m_pNormalMap;
    PersProjInfo m_persProjInfo;
    ParticleSystem m_particleSystem;
  };
}

#endif // __T28_MAIN_APP_H__

