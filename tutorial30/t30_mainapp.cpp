#include "t30_mainapp.h"

namespace t30
{
  MainApp::MainApp(int winWidth, int winHeight): m_winWidth(winWidth)
                                                 , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 1.0f;
    m_directionalLight.DiffuseIntensity = 0.01f;
    m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;

    m_dispFactor = 0.25f;
    m_isWireframe = false;
  }

  MainApp::~MainApp() = default;

  bool MainApp::Init()
  {
    const Vector3f pos(0.0f, 1.0f, -5.0f);
    const Vector3f target(0.0f, -0.2f, 1.0f);
    const Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    if (!m_lightingEffect.Init()) {
      std::cerr << "Error initializing the lighting technique" << "\n";
      return false;
    }

    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    printf("Max supported patch vertices %d\n", maxPatchVertices);
    //glPatchParameteri(GL_PATCH_VERTICES, 3);

    glActiveTexture(GL_TEXTURE4);
    m_pDisplacementMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/heightmap.jpg");

    if (!m_pDisplacementMap->Load())
      return false;

    m_pDisplacementMap->Bind(DISPLACEMENT_TEXTURE_UNIT);

    glActiveTexture(GL_TEXTURE0);
    m_pColorMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/diffuse.jpg");

    if (!m_pColorMap->Load())
      return false;

    m_pColorMap->Bind(COLOR_TEXTURE_UNIT);

    m_lightingEffect.Enable();
    GLExitIfError();
    m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_lightingEffect.SetDisplacementMapTextureUnit(DISPLACEMENT_TEXTURE_UNIT_INDEX);
    GLExitIfError();
    m_lightingEffect.SetDirectionalLight(m_directionalLight);
    GLExitIfError();
    m_lightingEffect.SetDispFactor(m_dispFactor);
    m_pMesh = std::make_shared<Mesh>();

    return m_pMesh->LoadMesh("../Content/quad2.obj");
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
    pipeline.Scale(2.0f, 2.0f, 2.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);

    // render the objects as usual
    //m_lightingEffect.Enable();
    GLExitIfError();
    m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());
    m_lightingEffect.SetVP(pipeline.GetVPTrans());
    m_lightingEffect.SetWorldMatrix(pipeline.GetWorldTrans());
    m_lightingEffect.SetDispFactor(m_dispFactor);
    GLExitIfError();
    m_pMesh->Render(nullptr);
    GLExitIfError();
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
    switch (key)
    {
    case 0x1b:
      glutLeaveMainLoop();
      break;

    case '+':
      m_dispFactor += 0.01f;
      break;

    case '-':
      if (m_dispFactor >= 0.01f)
        m_dispFactor -= 0.01f;
      break;

    case 'z':
      m_isWireframe = !m_isWireframe;
      if (m_isWireframe)
        glPolygonMode(GL_FRONT, GL_LINE);
      else
        glPolygonMode(GL_FRONT, GL_FILL);
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
