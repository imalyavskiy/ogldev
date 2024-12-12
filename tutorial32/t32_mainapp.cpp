#include "t32_mainapp.h"

namespace t32
{
  MainApp::MainApp(int winWidth, int winHeight): m_winWidth(winWidth)
                                                 , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.25f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

    m_perspProjInfo.FOV = 60.0f;
    m_perspProjInfo.Height = m_winHeight;
    m_perspProjInfo.Width = m_winWidth;
    m_perspProjInfo.zNear = 1.0f;
    m_perspProjInfo.zFar = 100.0f;
  }

  bool MainApp::Init()
  {
    Vector3f pos(3.0f, 7.0f, -10.0f);
    Vector3f target(0.0f, -0.2f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetMatSpecularIntensity(0.0f);
    m_pEffect->SetMatSpecularPower(0);

    m_pMesh1 = std::make_shared<Mesh>();

    if (!m_pMesh1->LoadMesh("../Content/phoenix_ugv.md2"))
      return false;

    m_pMesh2 = std::make_shared<Mesh>();

    if (!m_pMesh2->LoadMesh("../Content/jeep.obj"))
      return false;

    m_pMesh3 = std::make_shared<Mesh>();

    if (!m_pMesh3->LoadMesh("../Content/hheli.obj"))
      return false;

    return true;
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_scale += 0.01f;

    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

    Pipeline pipeline;
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.Rotate(0.0f, m_scale, 0.0f);
    pipeline.SetPerspectiveProj(m_perspProjInfo);

    pipeline.Scale(0.1f, 0.1f, 0.1f);
    pipeline.WorldPos(-6.0f, -2.0f, 10.0f);
    m_pEffect->SetWVP(pipeline.GetWVPTrans());
    m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    m_pMesh1->Render();

    pipeline.Scale(0.01f, 0.01f, 0.01f);
    pipeline.WorldPos(6.0f, -2.0f, 10.0f);
    m_pEffect->SetWVP(pipeline.GetWVPTrans());
    m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    m_pMesh2->Render();

    pipeline.Scale(0.04f, 0.04f, 0.04f);
    pipeline.WorldPos(0.0f, 6.0f, 10.0f);
    m_pEffect->SetWVP(pipeline.GetWVPTrans());
    m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    m_pMesh3->Render();

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
    case 0x1b:
      glutLeaveMainLoop();
      break;
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::MouseCB(int button, int state, int x, int y)
  {
  }
}
