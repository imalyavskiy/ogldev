#include <array>
#include <memory>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t20_pipeline.h"
#include "t20_camera.h"
#include "t20_texture.h"
#include "t20_lighting_technique.h"
#include "t20_glut_backend.h"
#include "t20_util.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_BPP 32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "tutorial 20"

namespace t20
{
  struct Vertex
  {
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() = default;

    Vertex(const Vector3f& pos, const Vector2f& tex)
    : m_pos(pos)
    , m_tex(tex)
    , m_normal{ 0.0f, 0.0f, 0.0f }
    { }
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
    DirectionalLight m_directionalLight;
  };

  Main::Main()
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = -0.1f;
    m_directionalLight.DiffuseIntensity = 0.0f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);
  }

  bool Main::Init()
  {
    Vector3f pos(0.0f, 0.0f, 0.0f);
    Vector3f target(0.0f, 0.0f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

    const std::array<unsigned int, 6> indices = { 0, 2, 1,  0, 3, 2 };

    CreateIndexBuffer(indices.data(), indices.size() * sizeof(indices[0]));

    CreateVertexBuffer(indices.data(), indices.size());

    m_pEffect = std::make_shared<LightingTechnique>();

    if (!m_pEffect->Init())
    {
      printf("Error initializing the lighting technique\n");
      return false;
    }

    m_pEffect->Enable();

    m_pEffect->SetTextureUnit(0);

    m_pTexture = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/test.png");

    if (!m_pTexture->Load())
      return false;

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

    m_scale += 0.01f;

    std::array<PointLight, 3> pointLights;

    pointLights[0].DiffuseIntensity = 0.5f;
    pointLights[0].Color = Vector3f(1.0f, 0.0f, 0.0f);
    pointLights[0].Position = Vector3f(sinf(m_scale) * 10, 1.0f, cosf(m_scale) * 10);
    pointLights[0].Attenuation.Linear = 0.1f;

    pointLights[1].DiffuseIntensity = 0.5f;
    pointLights[1].Color = Vector3f(0.0f, 1.0f, 0.0f);
    pointLights[1].Position = Vector3f(sinf(m_scale + 2.1f) * 10, 1.0f, cosf(m_scale + 2.1f) * 10);
    pointLights[1].Attenuation.Linear = 0.1f;

    pointLights[2].DiffuseIntensity = 0.5f;
    pointLights[2].Color = Vector3f(0.0f, 0.0f, 1.0f);
    pointLights[2].Position = Vector3f(sinf(m_scale + 4.2f) * 10, 1.0f, cosf(m_scale + 4.2f) * 10);
    pointLights[2].Attenuation.Linear = 0.1f;

    m_pEffect->SetPointLights(pointLights.size(), pointLights.data());

    Pipeline pipeline;
    pipeline.Rotate(0.0f, 0.0f, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 1.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);

    m_pEffect->SetWVP(pipeline.GetWVPTrans());

    const Matrix4f& WorldTransformation = pipeline.GetWorldTrans();

    m_pEffect->SetWorldMatrix(WorldTransformation);
    m_pEffect->SetDirectionalLight(m_directionalLight);
    m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
    m_pEffect->SetMatSpecularIntensity(1.0f);
    m_pEffect->SetMatSpecularPower(32);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>( 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(20));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    m_pTexture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
        m_directionalLight.AmbientIntensity -= 0.05f;
        break;
      case 's':
        m_directionalLight.AmbientIntensity += 0.05f;
        break;
      case 'z':
        m_directionalLight.DiffuseIntensity -= 0.05f;
        break;
      case 'x':
        m_directionalLight.DiffuseIntensity += 0.05f;
        break;
    }
  }

  void Main::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void Main::CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
  {
    for (unsigned int i = 0; i < IndexCount; i += 3) {
      const unsigned int i0 = pIndices[i + 0];
      const unsigned int i1 = pIndices[i + 1];
      const unsigned int i2 = pIndices[i + 2];
      Vector3f v1 = pVertices[i1].m_pos - pVertices[i0].m_pos;
      Vector3f v2 = pVertices[i2].m_pos - pVertices[i0].m_pos;
      Vector3f Normal = v1.Cross(v2);
      Normal.Normalize();

      pVertices[i0].m_normal += Normal;
      pVertices[i1].m_normal += Normal;
      pVertices[i2].m_normal += Normal;
    }

    for (unsigned int i = 0; i < VertexCount; i++)
      pVertices[i].m_normal.Normalize();
  }

  void Main::CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
  {
    std::array<Vertex,4> Vertices = {
      Vertex(Vector3f(-10.0f, -2.0f, -10.0f), Vector2f(0.0f, 0.0f)),
      Vertex(Vector3f(10.0f, -2.0f, -10.0f), Vector2f(1.0f, 0.0f)),
      Vertex(Vector3f(10.0f, -2.0f, 10.0f), Vector2f(1.0f, 1.0f)),
      Vertex(Vector3f(-10.0f, -2.0f, 10.0f), Vector2f(0.0f, 1.0f))
    };

    unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

    CalcNormals(pIndices, IndexCount, Vertices.data(), Vertices.size());

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertices[0]), Vertices.data(), GL_STATIC_DRAW);
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
  t20::GLUTBackendInit(argc, argv);

  if (!t20::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE)) {
    return 1;
  }

  const auto pApp = std::make_shared<t20::Main>();

  if (!pApp->Init()) {
    return 1;
  }

  pApp->Run();

  return 0;
}