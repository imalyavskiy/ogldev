#include <array>
#include <memory>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t22_pipeline.h"
#include "t22_glut_backend.h"
#include "t22_util.h"

#include "t22_mainapp.h"

namespace t22
{
  MainApp::MainApp(const GLint winWidth, const GLint winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 1.0f;
    m_directionalLight.DiffuseIntensity = 0.01f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0f, 1.0f);
  }

  bool MainApp::Init()
  {
    Vector3f Pos(-10.0f, 4.0f, 0.0f);
    Vector3f Target(1.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);
    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, Pos, Target, Up);

    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(0);

    m_pMesh = std::make_shared<Mesh>();

    return m_pMesh->LoadMesh("../Content/phoenix_ugv.md2");
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_scale += 0.01f;

    Pipeline pipeline;
    pipeline.Scale(0.1f, 0.1f, 0.1f);
    pipeline.Rotate(0.0f, m_scale, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 10.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 100.0f);

    m_pEffect->SetWVP(pipeline.GetWVPTrans());
    m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
    m_pEffect->SetMatSpecularIntensity(0.0f);
    m_pEffect->SetMatSpecularPower(0);

    m_pMesh->Render();

    glutSwapBuffers();
  }

  void MainApp::IdleCB()
  {
    RenderSceneCB();
  }

  void MainApp::SpecialKeyboardCB(int key, int x, int y)
  {
    m_pGameCamera->OnKeyboard(key);
  }

  void MainApp::KeyboardCB(unsigned char key, int x, int y)
  {
    switch (key) {
    case 0x1b: // Esc
      glutLeaveMainLoop();
      break;

    case 'a':
      m_directionalLight.AmbientIntensity += 0.05f;
      break;

    case 's':
      m_directionalLight.AmbientIntensity -= 0.05f;
      break;
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }
}
