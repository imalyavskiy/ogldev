#include "t33_mainapp.h"

namespace t33 {
  MainApp::MainApp(int winWidth, int winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;
  }

  bool MainApp::Init()
  {
    Vector3f pos(7.0f, 3.0f, 0.0f);
    Vector3f target(0.0f, -0.2f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_unique<Camera>(m_winWidth, m_winHeight, pos, target, up);

    m_pEffect = std::make_unique<LightingTechnique>();

    if (!m_pEffect->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetMatSpecularIntensity(0.0f);
    m_pEffect->SetMatSpecularPower(0);
    m_pEffect->SetColor(0, Vector4f(1.0f, 0.5f, 0.5f, 0.0f));
    m_pEffect->SetColor(1, Vector4f(0.5f, 1.0f, 1.0f, 0.0f));
    m_pEffect->SetColor(2, Vector4f(1.0f, 0.5f, 1.0f, 0.0f));
    m_pEffect->SetColor(3, Vector4f(1.0f, 1.0f, 1.0f, 0.0f));

    m_pMesh = std::make_unique<Mesh>();

    if (!m_pMesh->LoadMesh("../Content/spider.obj"))
      return false;

#ifdef FREETYPE
    if (!m_fontRenderer.InitFontRenderer())
      return false;
#endif

    m_time = glutGet(GLUT_ELAPSED_TIME);

    CalcPositions();

    return true;
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    CalcFPS();

    m_scale += 0.005f;

    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pEffect->Enable();
    m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

    Pipeline pipeline;
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);
    pipeline.Rotate(0.0f, 90.0f, 0.0f);
    pipeline.Scale(0.005f, 0.005f, 0.005f);

    Matrix4f matricesWorldViewProjection[NUM_INSTANCES];
    Matrix4f matricesWorld[NUM_INSTANCES];

    for (unsigned int i = 0; i < NUM_INSTANCES; i++) {
      Vector3f pos(m_positions[i]);
      pos.y += sinf(m_scale) * m_velocity[i];
      pipeline.WorldPos(pos);
      matricesWorldViewProjection[i] = pipeline.GetWVPTrans().Transpose();
      matricesWorld[i] = pipeline.GetWorldTrans().Transpose();
    }

    m_pMesh->Render(NUM_INSTANCES, matricesWorldViewProjection, matricesWorld);

    RenderFPS();

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

  void MainApp::CalcFPS()
  {
    m_frameCount++;

    const int time = glutGet(GLUT_ELAPSED_TIME);

    if (time - m_time > 1000) {
      m_fps = static_cast<float>(m_frameCount) * 1000.0f / (time - m_time);
      m_time = time;
      m_frameCount = 0;
    }
  }

  void MainApp::RenderFPS()
  {
    char text[32];
    SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
#ifdef FREETYPE
    m_fontRenderer.RenderText(10, 10, text);
#endif
  }

  void MainApp::CalcPositions()
  {
    for (unsigned int i = 0; i < NUM_ROWS; i++) {
      for (unsigned int j = 0; j < NUM_COLS; j++) {
        unsigned int Index = i * NUM_COLS + j;
        m_positions[Index].x = static_cast<float>(j);
        m_positions[Index].y = RandomFloat() * 5.0f;
        m_positions[Index].z = static_cast<float>(i);
        m_velocity[Index] = RandomFloat();
        if (i & 1) {
          m_velocity[Index] *= (-1.0f);
        }
      }
    }
  }
}
