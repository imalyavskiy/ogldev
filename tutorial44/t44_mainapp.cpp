#include "t44_mainapp.h"

namespace t44
{
  MainApp::MainApp(int winWidth, int winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.66f;
    m_directionalLight.DiffuseIntensity = 1.0f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

    m_perspProjInfo.FOV = 60.0f;
    m_perspProjInfo.Height = m_winHeight;
    m_perspProjInfo.Width = m_winWidth;
    m_perspProjInfo.zNear = 1.0f;
    m_perspProjInfo.zFar = 1000.0f;

    m_pipeline.SetPerspectiveProj(m_perspProjInfo);
    m_pipeline.WorldPos(Vector3f(0.0f, 0.0f, 0.0f));
    m_pipeline.Scale(0.1f, 0.1f, 0.1f);
  }

  bool MainApp::Init()
  {
    Vector3f Pos(0.0f, 23.0f, -5.0f);
    Vector3f Target(-1.0f, 0.0f, 0.1f);
    Vector3f Up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_unique<Camera>(m_winWidth, m_winHeight, Pos, Target, Up);

    if (!m_LightingTech.Init()) {
      OGLDEV_ERROR("Error initializing the lighting technique\n");
      return false;
    }

    m_LightingTech.Enable();

    m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_LightingTech.SetDirectionalLight(m_directionalLight);
    m_LightingTech.SetMatSpecularIntensity(0.0f);
    m_LightingTech.SetMatSpecularPower(0);

    if (!m_mesh.LoadMesh("../Content/crytek_sponza/sponza.obj")) {
      return false;
    }

    return true;
  }

  void MainApp::Run()
  {
    OgldevBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_LightingTech.SetEyeWorldPos(m_pGameCamera->GetPos());

    m_pipeline.SetCamera(*m_pGameCamera);

    m_LightingTech.SetWVP(m_pipeline.GetWVPTrans());
    m_LightingTech.SetWorldMatrix(m_pipeline.GetWorldTrans());

    m_mesh.Render();

    //    RenderFPS();     
    CalcFPS();

    OgldevBackendSwapBuffers();
  }

  void MainApp::KeyboardCB(OGLDEV_KEY OgldevKey)
  {
    switch (OgldevKey) {
    case OGLDEV_KEY_ESCAPE:
    case OGLDEV_KEY_q:
      OgldevBackendLeaveMainLoop();
      break;
    default:
      m_pGameCamera->OnKeyboard(OgldevKey);
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }
}
