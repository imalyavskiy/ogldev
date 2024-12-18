#include "t36_mainapp.h"

namespace t36
{
  MainApp::MainApp(int winWidth, int winHeight): m_winWidth(winWidth)
                                                 , m_winHeight(winHeight)
  {
    m_scale = 0.0f;

    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = m_winHeight;
    m_persProjInfo.Width = m_winWidth;
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;

    m_frameCount = 0;
    m_fps = 0.0f;

    InitLights();
    InitBoxPositions();

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

    if (!m_DSPointLightPassTech.Init()) {
      printf("Error initializing DSPointLightPassTech\n");
      return false;
    }

    m_DSPointLightPassTech.Enable();

    m_DSPointLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSPointLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSPointLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSPointLightPassTech.SetScreenSize(m_winWidth, m_winHeight);

    if (!m_DSDirLightPassTech.Init()) {
      printf("Error initializing DSDirLightPassTech\n");
      return false;
    }

    m_DSDirLightPassTech.Enable();

    m_DSDirLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSDirLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSDirLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
    m_DSDirLightPassTech.SetScreenSize(m_winWidth, m_winHeight);

    if (!m_quad.LoadMesh("../Content/quad.obj")) {
      return false;
    }

    if (!m_box.LoadMesh("../Content/box.obj")) {
      return false;
    }

    if (!m_boundingSphere.LoadMesh("../Content/sphere.obj")) {
      return false;
    }

#ifdef FREETYPE
      if (!m_fontRenderer.InitFontRenderer()) {
        return false;
      }
#endif

    return true;
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    CalcFPS();

    m_scale += 0.05f;

    m_pGameCamera->OnRender();

    DSGeometryPass();

    BeginLightPasses();

    DSPointLightsPass();

    DSDirectionalLightPass();

    RenderFPS();

    glutSwapBuffers();
  }

  void MainApp::DSGeometryPass()
  {
    m_DSGeomPassTech.Enable();

    m_gbuffer.BindForWriting();

    // Only the geometry pass updates the depth buffer
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);

    Pipeline pipeline;
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);
    pipeline.Rotate(0.0f, m_scale, 0.0f);

    for (uint32_t i = 0; i < std::size(m_boxPositions); ++i) {
      pipeline.WorldPos(m_boxPositions[i]);
      m_DSGeomPassTech.SetWVP(pipeline.GetWVPTrans());
      m_DSGeomPassTech.SetWorldMatrix(pipeline.GetWorldTrans());
      m_box.Render();
    }

    // When we get here the depth buffer is already populated and the stencil pass
    // depends on it, but it does not write to it.
    glDepthMask(GL_FALSE);

    glDisable(GL_DEPTH_TEST);
  }

  void MainApp::BeginLightPasses()
  {
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    m_gbuffer.BindForReading();
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void MainApp::DSPointLightsPass()
  {
    m_DSPointLightPassTech.Enable();
    m_DSPointLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

    Pipeline pipeline;
    pipeline.SetCamera(m_pGameCamera->GetPos(), 
                       m_pGameCamera->GetTarget(), 
                       m_pGameCamera->GetUp()
    );
    pipeline.SetPerspectiveProj(m_persProjInfo);

    for (uint32_t i = 0; i < std::size(m_pointLight); i++) {
      m_DSPointLightPassTech.SetPointLight(m_pointLight[i]);
      pipeline.WorldPos(m_pointLight[i].Position);
      const float boundingSphereScale = 
        CalcPointLightBoundingSphere(m_pointLight[i]);
      pipeline.Scale( boundingSphereScale, 
        boundingSphereScale, 
        boundingSphereScale
      );
      m_DSPointLightPassTech.SetWVP(pipeline.GetWVPTrans());
      m_boundingSphere.Render();
    }
  }

  void MainApp::DSDirectionalLightPass()
  {
    m_DSDirLightPassTech.Enable();
    m_DSDirLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

    Matrix4f worldViewProjection;
    worldViewProjection.InitIdentity();

    m_DSDirLightPassTech.SetWVP(worldViewProjection);
    m_quad.Render();
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

  float MainApp::CalcPointLightBoundingSphere(const PointLight& light)
  {
    const float maxCh = 
      fmax(
        fmax( light.Color.x,
                  light.Color.y
            ),
        light.Color.z
      );

    const float lin = 
      light.Attenuation.Linear;
    const float exp = 
      light.Attenuation.Exp;
    const float intensity = 
      light.DiffuseIntensity;

    const float discriminant = 
      lin * lin - 4 * exp * (exp - 256 * maxCh * intensity);
    
    return (-lin + sqrtf(discriminant)) / 2 * exp;
      // here is an error - right of the division
      // must be in brackets, but in this way
      // light is cannot be seen
  }

  void MainApp::InitLights()
  {
    m_spotLight.AmbientIntensity = 0.0f;
    m_spotLight.DiffuseIntensity = 0.9f;
    m_spotLight.Color = COLOR_WHITE;
    m_spotLight.Attenuation.Linear = 0.01f;
    m_spotLight.Position = Vector3f(-20.0, 20.0, 5.0f);
    m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff = 20.0f;

    m_dirLight.AmbientIntensity = 0.1f;
    m_dirLight.Color = COLOR_CYAN;
    m_dirLight.DiffuseIntensity = 0.5f;
    m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

    m_pointLight[0].DiffuseIntensity = 0.2f;
    m_pointLight[0].Color = COLOR_GREEN;
    m_pointLight[0].Position = Vector3f(0.0f, 1.5f, 5.0f);
    m_pointLight[0].Attenuation.Constant = 0.0f;
    m_pointLight[0].Attenuation.Linear = 0.0f;
    m_pointLight[0].Attenuation.Exp = 0.3f;

    m_pointLight[1].DiffuseIntensity = 0.2f;
    m_pointLight[1].Color = COLOR_RED;
    m_pointLight[1].Position = Vector3f(2.0f, 0.0f, 5.0f);
    m_pointLight[1].Attenuation.Constant = 0.0f;
    m_pointLight[1].Attenuation.Linear = 0.0f;
    m_pointLight[1].Attenuation.Exp = 0.3f;

    m_pointLight[2].DiffuseIntensity = 0.2f;
    m_pointLight[2].Color = COLOR_BLUE;
    m_pointLight[2].Position = Vector3f(0.0f, 0.0f, 3.0f);
    m_pointLight[2].Attenuation.Constant = 0.0f;
    m_pointLight[2].Attenuation.Linear = 0.0f;
    m_pointLight[2].Attenuation.Exp = 0.3f;
  }

  void MainApp::InitBoxPositions()
  {
    m_boxPositions[0] = Vector3f(0.0f, 0.0f, 5.0f);
    m_boxPositions[1] = Vector3f(6.0f, 1.0f, 10.0f);
    m_boxPositions[2] = Vector3f(-5.0f, -1.0f, 12.0f);
    m_boxPositions[3] = Vector3f(4.0f, 4.0f, 15.0f);
    m_boxPositions[4] = Vector3f(-4.0f, 2.0f, 20.0f);
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
