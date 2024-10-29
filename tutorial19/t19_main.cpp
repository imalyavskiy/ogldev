#include <memory>
#include <array>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t19_pipeline.h"
#include "t19_camera.h"
#include "t19_texture.h"
#include "t19_lighting_technique.h"
#include "t19_glut_backend.h"
#include "t19_util.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
namespace t19
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
      m_pos = pos;
      m_tex = tex;
      m_normal = Vector3f(0.0f, 0.0f, 0.0f);
    }
  };

  class Main : public ICallbacks
  {
  public:

    Main();

    ~Main() override = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;

    void KeyboardCB(unsigned char Key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount);

    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount);

    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes);

    GLuint m_VBO;
    GLuint m_IBO;
    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.0f;
    DirectionLight m_directionalLight;
  };

  Main::Main()
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.0f;
    m_directionalLight.DiffuseIntensity = 0.75f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
  }

  bool Main::Init()
  {
    Vector3f Pos(0.0f, 0.0f, -3.0f);
    Vector3f Target(0.0f, 0.0f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);
    m_pGameCamera = std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

    const std::array<uint32_t, 12> Indices { 0, 3, 1,  1, 3, 2,  2, 3, 0,  1, 2, 0 };

    CreateIndexBuffer(Indices.data(), Indices.size() * sizeof(Indices[0]));

    CreateVertexBuffer(Indices.data(), Indices.size());

    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init())
    {
      printf("Error initializing the lighting technique\n");
      return false;
    }

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

  void Main::Run()
  {
    GLUTBackendRun(this);
  }

  void Main::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    m_scale += 0.1f;

    Pipeline p;
    p.Rotate(0.0f, m_scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 1.0f);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
    m_pEffect->SetWVP(p.GetWVPTrans());
    const Matrix4f& WorldTransformation = p.GetWorldTrans();
    m_pEffect->SetWorldMatrix(WorldTransformation);
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
    m_pEffect->SetMatSpecularIntensity(1.0f);
    m_pEffect->SetMatSpecularPower(32);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    m_pTexture->Bind(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glutSwapBuffers();
  }

  void Main::IdleCB()
  {
    RenderSceneCB();
  }

  void Main::SpecialKeyboardCB(int Key, int x, int y)
  {
    m_pGameCamera->OnKeyboard(Key);
  }


  void Main::KeyboardCB(unsigned char Key, int x, int y)
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


  void Main::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void Main::CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
    Vertex* pVertices, unsigned int VertexCount) {
    for (unsigned int i = 0; i < IndexCount; i += 3) {
      unsigned int Index0 = pIndices[i];
      unsigned int Index1 = pIndices[i + 1];
      unsigned int Index2 = pIndices[i + 2];
      Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
      Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
      Vector3f Normal = v1.Cross(v2);
      Normal.Normalize();

      pVertices[Index0].m_normal += Normal;
      pVertices[Index1].m_normal += Normal;
      pVertices[Index2].m_normal += Normal;
    }

    for (unsigned int i = 0; i < VertexCount; i++) {
      pVertices[i].m_normal.Normalize();
    }
  }


  void Main::CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
  {
    Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                           Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
                           Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                           Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };

    unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

    CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  }

  void Main::CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
  {
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
  }
}


int main(int argc, char** argv)
{
  t19::GLUTBackendInit(argc, argv);

  // Не работает на полный экран, хз почему....
  if (!t19::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false/*true*/, "OpenGL tutors")) {
    return 1;
  }

  t19::Main* pApp = new t19::Main();

  if (!pApp->Init()) {
    return 1;
  }

  pApp->Run();

  delete pApp;

  return 0;
}
