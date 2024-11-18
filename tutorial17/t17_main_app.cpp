#include "t17_glut_backend.h"
#include "t17_main_app.h"

namespace t17
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() = default;

    Vertex(Vector3f pos, Vector2f tex) : m_pos(pos), m_tex(tex)
    { }
  };

  MainApp::MainApp(int winWidth, int winHeight, float fov, float zNear, float zFar)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  , m_verticalFoV(fov)
  , m_zNear(zNear)
  , m_zFar(zFar)
  {
  }

  bool MainApp::Init()
  {
    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight);

    CreateVertexBuffer();
    CreateIndexBuffer();

    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init())
      return false;

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(0);

    const std::string texture("../Content/test.png");
    m_pTexture = std::make_shared<Texture>(GL_TEXTURE_2D, texture);

    if (!m_pTexture->Load()) {
      std::cerr << "[ FATAL ] Failed to load image: " << texture << "\n";
      return false;
    }

    return true;
  }

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    m_scale += 0.1f;

    Pipeline pipeline;
    pipeline.Rotate(0.0f, m_scale, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 3.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(m_verticalFoV, m_winWidth, m_winHeight, m_zNear, m_zFar);

    m_pEffect->SetWVP(pipeline.GetTrans());
    m_pEffect->SetDirectionalLight(m_directionalLight);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    m_pTexture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

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
    case 0x1b: // Esc
      glutLeaveMainLoop();
      break;

    case 'a':
      m_directionalLight.AmbientIntensity += 0.05f;
      break;

    case 's':
      m_directionalLight.AmbientIntensity -= 0.05f;
      break;
    }
  }


  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::CreateVertexBuffer()
  {
    const Vertex Vertices[4] = {
      Vertex(Vector3f(-1.00000f, -1.00000f,  0.57730f), Vector2f(0.00000f, 0.00000f)),
      Vertex(Vector3f(0.00000f, -1.00000f, -1.15475f), Vector2f(0.50000f, 0.00000f)),
      Vertex(Vector3f(1.00000f, -1.00000f,  0.57730f), Vector2f(1.00000f, 0.00000f)),
      Vertex(Vector3f(0.00000f,  1.00000f,  0.00000f), Vector2f(0.50000f, 1.00000f))
    };

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  }

  void MainApp::CreateIndexBuffer()
  {
    const uint32_t Indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      1, 2, 0
    };

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  }
}
