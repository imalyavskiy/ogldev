#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t25_mainapp.h"
#include "t25_glut_backend.h"
#include "t25_pipeline.h"

namespace t25
{
  MainApp::MainApp(GLint winWidth, GLint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_dirLight.AmbientIntensity = 0.2f;
    m_dirLight.DiffuseIntensity = 0.8f;
    m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_dirLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;
  }


  bool MainApp::Init()
  {
    Vector3f Pos(0.0f, 1.0f, -20.0f);
    Vector3f Target(0.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, Pos, Target, Up);

    m_pLightingTechnique = std::make_shared<LightingTechnique>();

    if (!m_pLightingTechnique->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pLightingTechnique->Enable();
    m_pLightingTechnique->SetDirectionalLight(m_dirLight);
    m_pLightingTechnique->SetTextureUnit(GL_TEXTURE0);

    m_pTankMesh = std::make_shared<Mesh>();

    if (!m_pTankMesh->LoadMesh("../Content/phoenix_ugv.md2")) {
      return false;
    }

    m_pSkyBox = std::make_shared<SkyBox>(m_pGameCamera, m_persProjInfo);

    if (!m_pSkyBox->Init(".",
      "../Content/sp3right.jpg",
      "../Content/sp3left.jpg",
      "../Content/sp3top.jpg",
      "../Content/sp3bot.jpg",
      "../Content/sp3front.jpg",
      "../Content/sp3back.jpg")) {
      return false;
    }

    return true;
  }


  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }


  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();
    m_scale += 0.05f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pLightingTechnique->Enable();

    Pipeline p;
    p.Scale(0.1f, 0.1f, 0.1f);
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(0.0f, -5.0f, 3.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);

    m_pLightingTechnique->SetWVP(p.GetWVPTrans());
    m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
    m_pTankMesh->Render();

    m_pSkyBox->Render();

    glutSwapBuffers();
  }


  void MainApp::IdleCB()
  {
    RenderSceneCB();
  }


  void MainApp::SpecialKeyboardCB(int Key, int x, int y)
  {
    m_pGameCamera->OnKeyboard(Key);
  }


  void MainApp::KeyboardCB(unsigned char Key, int x, int y)
  {
    switch (Key) {
    case 0x1b: // Esc
      glutLeaveMainLoop();
      break;
    }
  }


  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }
}
