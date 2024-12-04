#include "t29_mainapp.h"

namespace t29
{
  MainApp::MainApp(int winWidth, int winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 1.0f;
    m_directionalLight.DiffuseIntensity = 0.01f;
    m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);
    m_leftMouseButton.IsPressed = false;
    m_worldPos[0] = Vector3f(-10.0f, 0.0f, 5.0f);
    m_worldPos[1] = Vector3f(10.0f, 0.0f, 5.0f);

    m_perspProjInfo.FOV = 60.0f;
    m_perspProjInfo.Height = m_winHeight;
    m_perspProjInfo.Width = m_winWidth;
    m_perspProjInfo.zNear = 1.0f;
    m_perspProjInfo.zFar = 100.0f;
  }

  bool MainApp::Init()
  {
    const Vector3f pos(0.0f, 5.0f, -22.0f);
    const Vector3f target(0.0f, -0.2f, 1.0f);
    const Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    if (!m_lightingEffect.Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_lightingEffect.Enable();
    m_lightingEffect.SetTextureUnit(0);
    m_lightingEffect.SetDirectionalLight(m_directionalLight);

    if (!m_pickingTexture.Init(m_winWidth, m_winHeight))
      return false;

    if (!m_pickingEffect.Init())
      return false;

    if (!m_simpleColorEffect.Init())
      return false;

    m_pMesh = std::make_shared<Mesh>();

    return m_pMesh->LoadMesh("../Content/spider.obj");
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    PickingPhase();
    RenderPhase();

    glutSwapBuffers();
  }

  void MainApp::PickingPhase()
  {
    Pipeline p;
    p.Scale(0.1f, 0.1f, 0.1f);
    p.Rotate(0.0f, 90.0f, 0.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_perspProjInfo);

    m_pickingTexture.EnableWriting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pickingEffect.Enable();

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_worldPos); i++) {
      p.WorldPos(m_worldPos[i]);
      m_pickingEffect.SetObjectIndex(i);
      m_pickingEffect.SetWVP(p.GetWVPTrans());
      m_pMesh->Render(&m_pickingEffect);
    }

    m_pickingTexture.DisableWriting();
  }

  void MainApp::RenderPhase()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Pipeline p;
    p.Scale(0.1f, 0.1f, 0.1f);
    p.Rotate(0.0f, 90.0f, 0.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_perspProjInfo);

    // If the left mouse button is clicked check if it hit a triangle
    // and color it red
    if (m_leftMouseButton.IsPressed) {
      const PickingTexture::PixelInfo pixelInfo =
        m_pickingTexture.ReadPixel(m_leftMouseButton.x, m_winHeight - m_leftMouseButton.y - 1);

      if (pixelInfo.PrimID != 0) {
        m_simpleColorEffect.Enable();
        p.WorldPos(m_worldPos[pixelInfo.ObjectID]);
        m_simpleColorEffect.SetWVP(p.GetWVPTrans());
        // Must compensate for the decrement in the FS!
        m_pMesh->Render(pixelInfo.DrawID, pixelInfo.PrimID - 1);
      }
    }

    // render the objects as usual
    m_lightingEffect.Enable();
    m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_worldPos); i++) {
      p.WorldPos(m_worldPos[i]);
      m_lightingEffect.SetWVP(p.GetWVPTrans());
      m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
      m_pMesh->Render(nullptr);
    }
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

    case 'z':
      m_directionalLight.DiffuseIntensity += 0.05f;
      break;

    case 'x':
      m_directionalLight.DiffuseIntensity -= 0.05f;
      break;
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::MouseCB(int button, int state, int x, int y)
  {
    if (button == GLUT_LEFT_BUTTON) {
      m_leftMouseButton.IsPressed = (state == GLUT_DOWN);
      m_leftMouseButton.x = x;
      m_leftMouseButton.y = y;
    }
  }
}
