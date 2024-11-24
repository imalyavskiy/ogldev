#include "t18_main_app.h"
#include "t18_glut_backend.h"

namespace t18
{
  MainApp::MainApp(GLuint winWidth, GLuint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.0f;
    m_directionalLight.DiffuseIntensity = 0.75f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
  }

  bool MainApp::Init()
  {
    Vector3f Pos(0.0f, 0.0f, -3.0f);
    Vector3f Target(0.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);
    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, Pos, Target, Up);

    const std::array<uint32_t, 12> indices{ 0, 3, 1,  1, 3, 2,  2, 3, 0,  1, 2, 0 };

    CreateIndexBuffer(indices.data(), indices.size() * sizeof(indices[0]));

    CreateVertexBuffer(indices.data(), indices.size());


    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init())
    {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(GL_TEXTURE0);

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
    pipeline.WorldPos(0.0f, 0.0f, 1.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(60.0f, m_winWidth, m_winHeight, 1.0f, 100.0f);

    m_pEffect->SetWVP(pipeline.GetWVPTrans());
    m_pEffect->SetWorldMatrix(pipeline.GetWorldTrans());
    m_pEffect->SetDirectionalLight(m_directionalLight);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    m_pTexture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glutSwapBuffers();
  }

  void MainApp::IdleCB()
  {
    RenderSceneCB();
  }

  void MainApp::SpecialKeyboardCB(int32_t Key, int32_t x, int32_t y)
  {
    m_pGameCamera->OnKeyboard(Key);
  }

  void MainApp::KeyboardCB(uint8_t Key, int32_t x, int32_t y)
  {
    switch (Key) {
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

  void MainApp::PassiveMouseCB(int32_t x, int32_t y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::CalcNormals(const uint32_t* pIndices, uint32_t IndexCount, Vertex* pVertices, uint32_t VertexCount)
  {
    for (unsigned int i = 0; i < IndexCount; i += 3) {
      const uint32_t i0 = pIndices[i + 0];
      const uint32_t i1 = pIndices[i + 1];
      const uint32_t i2 = pIndices[i + 2];
      const Vector3f v1 = pVertices[i1].m_pos - pVertices[i0].m_pos;
      const Vector3f v2 = pVertices[i2].m_pos - pVertices[i0].m_pos;
      const Vector3f Normal = v1.Cross(v2).Normalize();

      pVertices[i0].m_normal += Normal;
      pVertices[i1].m_normal += Normal;
      pVertices[i2].m_normal += Normal;
    }

    for (unsigned int i = 0; i < VertexCount; i++) {
      pVertices[i].m_normal.Normalize();
    }
  }


  void MainApp::CreateVertexBuffer(const uint32_t* pIndices, uint32_t IndexCount)
  {
    Vertex Vertices[] = {
      {{ -1.0f, -1.0f,  0.57730f}, {0.0f, 0.0f}},
      {{  0.0f, -1.0f, -1.15475f}, {0.5f, 0.0f}},
      {{  1.0f, -1.0f,  0.57730f}, {1.0f, 0.0f}},
      {{  0.0f,  1.0f,  0.00000f}, {0.5f, 1.0f}}
    };

    constexpr uint32_t VertexCount =
      sizeof(Vertices) / sizeof(Vertices[0]);

    CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  }

  void MainApp::CreateIndexBuffer(const uint32_t* pIndices, uint32_t SizeInBytes)
  {
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
  }
}