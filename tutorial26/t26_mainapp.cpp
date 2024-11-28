#include "t26_mainapp.h"

namespace t26
{
  MainApp::MainApp(GLint winWidth, GLint winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_dirLight.AmbientIntensity = 0.2f;
    m_dirLight.DiffuseIntensity = 0.8f;
    m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;
  }

  bool MainApp::Init()
  {
    Vector3f Pos(0.5f, 1.025f, 0.25f);
    Vector3f Target(0.0f, -0.5f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, Pos, Target, Up);

    m_pLightingTechnique = std::make_shared<LightingTechnique>();

    if (!m_pLightingTechnique->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pLightingTechnique->Enable();
    m_pLightingTechnique->SetDirectionalLight(m_dirLight);
    m_pLightingTechnique->SetColorTextureUnit(COLOR_TEXTURE_UNIT);
    m_pLightingTechnique->SetNormalMapTextureUnit(NORMAL_TEXTURE_UNIT);

    m_pSphereMesh = std::make_shared<Mesh>();

    if (!m_pSphereMesh->LoadMesh("../Content/box.obj")) {
      return false;
    }

    m_pTexture = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/bricks.jpg");

    if (!m_pTexture->Load()) {
      return false;
    }

    m_pTexture->Bind(COLOR_TEXTURE_UNIT);

    m_pNormalMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/normal_map.jpg");

    if (!m_pNormalMap->Load()) {
      return false;
    }

    m_pTrivialNormalMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/normal_up.jpg");

    if (!m_pTrivialNormalMap->Load()) {
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
    m_scale += 0.01f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pLightingTechnique->Enable();

    Pipeline p;
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);

    m_pTexture->Bind(COLOR_TEXTURE_UNIT);

    if (m_bumpMapEnabled)
      m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);
    else
      m_pTrivialNormalMap->Bind(NORMAL_TEXTURE_UNIT);

    m_pLightingTechnique->SetWVP(p.GetWVPTrans());
    m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
    m_pSphereMesh->Render();

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

    case 'b':
      m_bumpMapEnabled = !m_bumpMapEnabled;
      break;
    }
  }


  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }
}