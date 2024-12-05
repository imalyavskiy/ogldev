#include "t28_mainapp.h"

namespace t28
{
  MainApp::MainApp(int winWidth, int winHeight)
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

    m_currentTimeMillis = GetCurrentTimeMillis();
  }

  bool MainApp::Init()
  {
    Vector3f pos(0.0f, 0.4f, -0.5f);
    Vector3f target(0.0f, 0.2f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    m_pLightingTechnique = std::make_shared<LightingTechnique>();

    if (!m_pLightingTechnique->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pLightingTechnique->Enable();
    m_pLightingTechnique->SetDirectionalLight(m_dirLight);
    m_pLightingTechnique->SetColorTextureUnit(INDEX_OF(COLOR_TEXTURE_UNIT));
    m_pLightingTechnique->SetNormalMapTextureUnit(INDEX_OF(NORMAL_TEXTURE_UNIT));

    m_pGround = std::make_shared<Mesh>();

    if (!m_pGround->LoadMesh("../Content/quad.obj")) {
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

    const Vector3f ParticleSystemPos = Vector3f(0.0f, 0.0f, 1.0f);

    return m_particleSystem.Init(ParticleSystemPos);
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    const int64_t timeNowMillis = GetCurrentTimeMillis();
    assert(timeNowMillis >= m_currentTimeMillis);

    const auto deltaTimeMillis = static_cast<uint32_t>(timeNowMillis - m_currentTimeMillis);
    m_currentTimeMillis = timeNowMillis;
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pLightingTechnique->Enable();

    m_pTexture->Bind(COLOR_TEXTURE_UNIT);
    m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);

    Pipeline pipeline;
    pipeline.Scale(20.0f, 20.0f, 1.0f);
    pipeline.Rotate(90.0f, 0.0, 0.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);

    m_pLightingTechnique->SetWVP(pipeline.GetWVPTrans());
    m_pLightingTechnique->SetWorldMatrix(pipeline.GetWorldTrans());

    m_pGround->Render();

    m_particleSystem.Render(deltaTimeMillis, pipeline.GetVPTrans(), m_pGameCamera->GetPos());

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
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }
}
