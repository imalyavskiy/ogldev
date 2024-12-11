#include "t31_mainapp.h"
namespace t31 {
  MainApp::MainApp(int winWidth, int winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.1f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = Vector3f(0.0f, 0.0, 1.0);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;

    m_tessellationLevel = 5.0f;
    m_isWireframe = false;
  }

  bool MainApp::Init()
  {
    Vector3f pos(0.0f, 1.5f, -6.5f);
    Vector3f target(0.0f, -0.2f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    if (!m_lightingEffect.Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    printf("Max supported patch vertices %d\n", maxPatchVertices);
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    m_lightingEffect.Enable();
    m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_lightingEffect.SetDirectionalLight(m_directionalLight);
    m_pMesh = std::make_shared<Mesh>();

    return m_pMesh->LoadMesh("../Content/monkey.obj");
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pGameCamera->OnRender();

    Pipeline pipeline;
    pipeline.WorldPos(-3.0f, 0.0f, 0.0f);
    pipeline.Scale(2.0f, 2.0f, 2.0f);
    pipeline.Rotate(-90.0f, 15.0f, 0.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);
    m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());

    m_lightingEffect.SetVP(pipeline.GetVPTrans());
    m_lightingEffect.SetWorldMatrix(pipeline.GetWorldTrans());
    m_lightingEffect.SetTessellationLevel(m_tessellationLevel);
    m_pMesh->Render(nullptr);

    pipeline.WorldPos(3.0f, 0.0f, 0.0f);
    pipeline.Rotate(-90.0f, -15.0f, 0.0f);
    m_lightingEffect.SetVP(pipeline.GetVPTrans());
    m_lightingEffect.SetWorldMatrix(pipeline.GetWorldTrans());
    m_lightingEffect.SetTessellationLevel(1.0f);
    m_pMesh->Render(nullptr);
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

    case '+':
      m_tessellationLevel += 1.0f;
      break;

    case '-':
      if (m_tessellationLevel >= 2.0f) {
        m_tessellationLevel -= 1.0f;
      }
      break;

    case 'z':
      m_isWireframe = !m_isWireframe;

      if (m_isWireframe) {
        glPolygonMode(GL_FRONT, GL_LINE);
      }
      else {
        glPolygonMode(GL_FRONT, GL_FILL);
      }
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
