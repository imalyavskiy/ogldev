#include "t34_mainapp.h"

namespace t34
{
  MainApp::MainApp(int winWidth, int winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;


    m_positions[0] = Vector3f(-2.0f, 0.0f, 6.0f);
    m_positions[1] = Vector3f(0.0f, 0.0f, 6.0f);
    m_positions[2] = Vector3f(2.0f, 0.0f, 6.0f);
    m_positions[3] = Vector3f(4.0f, 0.0f, 6.0f);

    m_velocity[0] = 10.0f;
    m_velocity[1] = 15.0f;
    m_velocity[2] = 50.0f;
    m_velocity[3] = 20.0f;

    m_colors[0] = Vector4f(1.0f, 0.0, 0.0, 0.0f);
    m_colors[1] = Vector4f(0.0f, 1.0, 0.0, 0.0f);
    m_colors[2] = Vector4f(0.0f, 0.0, 1.0, 0.0f);
    m_colors[3] = Vector4f(1.0f, 0.0, 1.0, 0.0f);
  }

  bool MainApp::Init()
  {
    const Vector3f pos(0.8f, 0.0f, 0.0f);
    const Vector3f target(0.0f, 0.0f, 1.0f);
    const Vector3f up(0.0, 1.0f, 0.0f);

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

    if (!m_mesh[0].LoadMesh("../Content/glfx/g.obj"))
      return false;

    if (!m_mesh[1].LoadMesh("../Content/glfx/l.obj"))
      return false;

    if (!m_mesh[2].LoadMesh("../Content/glfx/f.obj"))
      return false;

    if (!m_mesh[3].LoadMesh("../Content/glfx/x.obj"))
      return false;

#ifdef FREETYPE
    if (!m_fontRenderer.InitFontRenderer()) 
      return false;

    if (!m_fontRenderer2.InitFontRenderer())
      return false;
#endif

    m_time = glutGet(GLUT_ELAPSED_TIME);

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
    pipeline.Scale(0.1f, 0.1f, 0.1f);

    Matrix4f WVPMatrics[1];
    Matrix4f WorldMatrices[1];

    for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_mesh); i++) {
      m_pEffect->SetColor(m_colors[i]);

      pipeline.Rotate(0.0, m_scale * m_velocity[i], 0.0f);
      Vector3f pos(m_positions[i]);
      pipeline.WorldPos(pos);
      m_pEffect->SetWVP(pipeline.GetWVPTrans());
      m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());

      m_mesh[i].Render(1, WVPMatrics, WorldMatrices);
    }

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
    char text[32] = {};
    SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
#ifdef FREETYPE
    m_fontRenderer.RenderText(10, 10, text);
    // The FontRenderer class resides in the original repo placed here(https://github.com/emeiri/ogldev) at
    // https://github.com/emeiri/ogldev/blob/master/Common/FreetypeGL/freetypeGL.h but in order to made the
    // code compilable some unidentified libraries required. Thus I decided to got rid of this stuff and get
    // back later also I suppose it is better to use self contained ImGUI(https://github.com/ocornut/imgui)
    // library for such tasks
#endif
  }
}
