#ifndef __T44_MAIN_APP_H__
#define __T44_MAIN_APP_H__

#include <windows.h>

#include <string>
#include <memory>

#include <cmath>
#include <GL/glew.h>
#include <sys/types.h>

#include "t44_engine_common.h"
#include "t44_app.h"
#include "t44_util.h"
#include "t44_pipeline.h"
#include "t44_camera.h"
#include "t44_lighting_technique.h"
#include "t44_backend.h"
#include "t44_mesh.h"

namespace t44
{
  class MainApp : public ICallbacks, public BaseApp
  {
  public:

    MainApp(int32_t winWidth, int32_t winHeight);

    ~MainApp() = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    virtual void KeyboardCB(KEYBOARD_KEY key);

    void PassiveMouseCB(int32_t x, int32_t y) override;

  private:
    int m_winWidth;
    int m_winHeight;

    LightingTechnique m_LightingTech;
    std::unique_ptr<Camera> m_pGameCamera;
    DirectionalLight m_directionalLight;
    Mesh m_mesh;
    PerspProjInfo m_perspProjInfo;
    Pipeline m_pipeline;
  };
}

#endif // __T44_MAIN_APP_H__