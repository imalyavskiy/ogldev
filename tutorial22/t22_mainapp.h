#pragma once
#ifndef T22_MAIN_APP_H
#define T22_MAIN_APP_H
#include <memory>
#include <GL/glew.h>

#include "t22_callbacks.h"
#include "t22_camera.h"
#include "t22_lighting_technique.h"
#include "t22_mesh.h"

namespace t22
{
  class MainApp final : public ICallbacks
  {
  public:

    MainApp(GLint winWidth, GLint winHeight);

    ~MainApp() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int key, int x, int y) override;

    void KeyboardCB(unsigned char key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    GLint m_winWidth;
    GLint m_winHeight;

    GLuint m_VBO = INVALID_OGL_VALUE;
    GLuint m_IBO = INVALID_OGL_VALUE;

    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Mesh> m_pMesh;
    std::shared_ptr<Camera> m_pGameCamera;

    float m_scale = 0.0f;

    DirectionalLight m_directionalLight;
  };
}
#endif // T22_MAIN_APP_H
