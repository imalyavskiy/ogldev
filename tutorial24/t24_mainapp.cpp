#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t24_glut_backend.h"
#include "t24_pipeline.h"

#include "t24_mainapp.h"

namespace t24
{
  MainApp::MainApp(GLint winWidth, GLint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_spotLight.AmbientIntensity = 0.1f;
    m_spotLight.DiffuseIntensity = 0.9f;
    m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_spotLight.Attenuation.Linear = 0.01f;
    m_spotLight.Position = Vector3f(-20.0, 20.0, 1.0f);
    m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff = 20.0f;
  }

  bool MainApp::Init()
  {
    const Vector3f pos(3.0f, 8.0f, -10.0f);
    const Vector3f target(0.0f, -0.2f, 1.0f);
    const Vector3f up(0.0, 1.0f, 0.0f);

    if (!m_shadowMapFBO.Init(m_winWidth, m_winHeight)) {
      return false;
    }

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    m_pLightingEffect = std::make_shared<LightingTechnique>();
    if (!m_pLightingEffect->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }
    m_pLightingEffect->Enable();
    m_pLightingEffect->SetSpotLights(1, &m_spotLight);
    m_pLightingEffect->SetTextureUnit(GL_TEXTURE0);
    m_pLightingEffect->SetShadowMapTextureUnit(GL_TEXTURE1);

    m_pShadowMapEffect = std::make_shared<ShadowMapTechnique>();

    if (!m_pShadowMapEffect->Init()) {
      printf("Error initializing the shadow map technique\n");
      return false;
    }

    m_pQuad = std::make_shared<Mesh>();

    if (!m_pQuad->LoadMesh("../Content/quad.obj")) {
      return false;
    }

    m_pGroundTex = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/test.png");

    if (!m_pGroundTex->Load()) {
      return false;
    }

    m_pMesh = std::make_shared<Mesh>();

    return m_pMesh->LoadMesh("../Content/phoenix_ugv.md2");
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();
    m_scale += 0.5f;

    ShadowMapPass();
    RenderPass();

    glutSwapBuffers();
  }

  void MainApp::ShadowMapPass()
  {
    m_shadowMapFBO.BindForWriting();

    glClear(GL_DEPTH_BUFFER_BIT);

    m_pShadowMapEffect->Enable();

    Pipeline pipeline;
    pipeline.Scale(0.1f, 0.1f, 0.1f);
    pipeline.Rotate(0.0f, m_scale, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 5.0f);
    pipeline.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
    pipeline.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 50.0f);

    m_pShadowMapEffect->SetWVP(pipeline.GetWVPTrans());
    m_pMesh->Render();

    m_shadowMapFBO.UnbindForWriting();
  }

  void MainApp::RenderPass()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pLightingEffect->Enable();

    m_shadowMapFBO.BindForReading(GL_TEXTURE1);

    Pipeline pipeline;
    pipeline.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 50.0f);
    pipeline.Scale(10.0f, 10.0f, 10.0f);
    pipeline.WorldPos(0.0f, 0.0f, 1.0f);
    pipeline.Rotate(90.0f, 0.0f, 0.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

    m_pLightingEffect->SetWVP(pipeline.GetWVPTrans());
    m_pLightingEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    pipeline.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
    m_pLightingEffect->SetLightWVP(pipeline.GetWVPTrans());
    m_pLightingEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
    m_pGroundTex->Bind(GL_TEXTURE0);
    m_pQuad->Render();

    pipeline.Scale(0.1f, 0.1f, 0.1f);
    pipeline.Rotate(0.0f, m_scale, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 3.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    m_pLightingEffect->SetWVP(pipeline.GetWVPTrans());
    m_pLightingEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    pipeline.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
    m_pLightingEffect->SetLightWVP(pipeline.GetWVPTrans());

    m_pMesh->Render();
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
