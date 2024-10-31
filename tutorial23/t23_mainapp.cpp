#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t23_glut_backend.h"
#include "t23_pipeline.h"
#include "t23_mainapp.h"

namespace t23
{
  MainApp::MainApp(GLint winWidth, GLint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_pEffect = NULL;
    m_pShadowMapTech = NULL;
    m_pGameCamera = NULL;
    m_pMesh = NULL;
    m_pQuad = NULL;
    m_scale = 0.0f;

    m_spotLight.AmbientIntensity = 0.0f;
    m_spotLight.DiffuseIntensity = 0.9f;
    m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_spotLight.Attenuation.Linear = 0.01f;
    m_spotLight.Position = Vector3f(-20.0, 20.0, 5.0f);
    m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff = 20.0f;
  }

  MainApp::~MainApp()
  {
    SAFE_DELETE(m_pEffect);
    SAFE_DELETE(m_pShadowMapTech);
    SAFE_DELETE(m_pGameCamera);
    SAFE_DELETE(m_pMesh);
    SAFE_DELETE(m_pQuad);
  }

  bool MainApp::Init()
  {
    if (!m_shadowMapFBO.Init(m_winWidth, m_winHeight)) {
      return false;
    }

    m_pGameCamera = new Camera(m_winWidth, m_winHeight);

    m_pEffect = new LightingTechnique();

    if (!m_pEffect->Init()) {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pShadowMapTech = new ShadowMapTechnique();

    if (!m_pShadowMapTech->Init()) {
      printf("Error initializing the shadow map technique\n");
      return false;
    }

    m_pShadowMapTech->Enable();

    m_pQuad = new Mesh();

    if (!m_pQuad->LoadMesh("../Content/quad.obj")) {
      return false;
    }

    m_pMesh = new Mesh();

    return m_pMesh->LoadMesh("../Content/phoenix_ugv.md2");
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();
    m_scale += 0.05f;

    ShadowMapPass();
    RenderPass();

    glutSwapBuffers();
  }

  void MainApp::ShadowMapPass()
  {
    m_shadowMapFBO.BindForWriting();

    glClear(GL_DEPTH_BUFFER_BIT);

    Pipeline p;
    p.Scale(0.2f, 0.2f, 0.2f);
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 5.0f);
    p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
    p.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 50.0f);
    m_pShadowMapTech->SetWVP(p.GetWVPTrans());
    m_pMesh->Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void MainApp::RenderPass()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pShadowMapTech->SetTextureUnit(0);
    m_shadowMapFBO.BindForReading(GL_TEXTURE0);

    Pipeline p;
    p.Scale(5.0f, 5.0f, 5.0f);
    p.WorldPos(0.0f, 0.0f, 10.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 50.0f);
    m_pShadowMapTech->SetWVP(p.GetWVPTrans());
    m_pQuad->Render();
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
