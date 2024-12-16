#include "t35_mainapp.h"

namespace t35
{
  MainApp::MainApp(int winWidth, int winHeight): m_winWidth(winWidth)
                                                 , m_winHeight(winHeight)
  {
    m_pGameCamera = NULL;
    m_scale = 0.0f;

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;

    m_frameCount = 0;
    m_fps = 0.0f;

    m_time = glutGet(GLUT_ELAPSED_TIME);
  }

  bool MainApp::Init()
  {
    if (!m_gbuffer.Init(m_winWidth, m_winHeight)) {
      return false;
    }

    m_pGameCamera = std::make_unique<Camera>(m_winWidth, m_winHeight);

    if (!m_DSGeomPassTech.Init()) {
      printf("Error initializing DSGeomPassTech\n");
      return false;
    }

    m_DSGeomPassTech.Enable();
    m_DSGeomPassTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

    if (!m_mesh.LoadMesh("../Content/phoenix_ugv.md2")) {
      return false;
    }

#ifdef FREETYPE
      if (!m_fontRenderer.InitFontRenderer()) {
        return false;
      }
#endif

    return true;
  }

  void MainApp::RenderSceneCB()
  {
    CalcFPS();

    m_scale += 0.05f;

    m_pGameCamera->OnRender();

    DSGeometryPass();
    DSLightPass();

    RenderFPS();

    glutSwapBuffers();
  }

  void MainApp::DSGeometryPass()
  {
    m_DSGeomPassTech.Enable();

    m_gbuffer.BindForWriting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Pipeline p;
    p.Scale(0.1f, 0.1f, 0.1f);
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(-0.8f, -1.0f, 12.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);
    m_DSGeomPassTech.SetWVP(p.GetWVPTrans());
    m_DSGeomPassTech.SetWorldMatrix(p.GetWorldTrans());
    m_mesh.Render();
  }

  void MainApp::DSLightPass()
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_gbuffer.BindForReading();

    GLint HalfWidth = (GLint)(m_winWidth / 2.0f);
    GLint HalfHeight = (GLint)(m_winHeight / 2.0f);

    m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(0, 0, m_winWidth, m_winHeight, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    glBlitFramebuffer(0, 0, m_winWidth, m_winHeight, 0, HalfHeight, HalfWidth, m_winHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    glBlitFramebuffer(0, 0, m_winWidth, m_winHeight, HalfWidth, HalfHeight, m_winWidth, m_winHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
    glBlitFramebuffer(0, 0, m_winWidth, m_winHeight, HalfWidth, 0, m_winWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
    case 0x1b:
      glutLeaveMainLoop();
      break;
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::MouseCB(int Button, int State, int x, int y)
  {
  }

  void MainApp::CalcFPS()
  {
    m_frameCount++;

    int time = glutGet(GLUT_ELAPSED_TIME);

    if (time - m_time > 1000) {
      m_fps = (float)m_frameCount * 1000.0f / (time - m_time);
      m_time = time;
      m_frameCount = 0;
    }
  }

  void MainApp::RenderFPS()
  {
    char text[32];
    ZERO_MEM(text);
    SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
#ifdef FREETYPE
      m_fontRenderer.RenderText(10, 10, text);
#endif
  }
}
