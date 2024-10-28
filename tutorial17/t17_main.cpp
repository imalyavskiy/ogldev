#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t17_pipeline.h"
#include "t17_camera.h"
#include "t17_texture.h"
#include "t17_lighting_technique.h"
#include "t17_glut_backend.h"

#define VERTICAL_FOV 60.f
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define Z_NEAR_CLIPPING_PLANE 1.f
#define Z_FAR_CLIPPING_PLANE 100.f

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

  class MainApp : public ICallbacks
  {
  public:
    MainApp() = default;

    ~MainApp() = default;

    bool Init();

    void Run();

    void RenderSceneCB() override;

    void IdleCB() override;

    void SpecialKeyboardCB(int Key, int x, int y) override;

    void KeyboardCB(unsigned char Key, int x, int y) override;

    void PassiveMouseCB(int x, int y) override;

  private:

    void CreateVertexBuffer();

    void CreateIndexBuffer();

    GLuint m_VBO;
    GLuint m_IBO;
    std::shared_ptr<LightingTechnique> m_pEffect;
    std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Camera> m_pGameCamera;
    float m_scale = 0.f;
    DirectionLight m_directionalLight{ { 1.0f, 1.0f, 1.0f }, 0.5f };
  };

  bool MainApp::Init()
  {
    m_pGameCamera = std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT);

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
    pipeline.SetPerspectiveProj(VERTICAL_FOV, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR_CLIPPING_PLANE, Z_FAR_CLIPPING_PLANE);

    m_pEffect->SetWVP(pipeline.GetTrans());
    m_pEffect->SetDirectionalLight(m_directionalLight);

    glEnableVertexAttribArray(0); // These magic 0 and 1 are locations(aka attributes of the vertex taken from vertex shader)
    glEnableVertexAttribArray(1); // Please look at "in vec3 Position" and "in vec 2 TexCoord" variable definitions in the shader.

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // Here the actual data plugged to shader's input:
    //    attr - 0,
    //    3 - items,
    //    of float,
    //    not normalized,
    //    stride width of 20 bytes(float - 4bytes * 5)
    //    first byte addr from the attached buffer beginning

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(12));
    // Here the actual data plugged to shader's input:
    //    attr - 1,
    //    2 - items,
    //    of float,
    //    not normalized,
    //    stride width of 20 bytes(float - 4bytes * 5)
    //    12th(0 based) byte addr from the attached buffer beginning

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    m_pTexture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0); // These magic 0 and 1 are locations(aka attributes of the vertex taken from vertex shader)
    glDisableVertexAttribArray(1); // Please look at "in vec3 Position" and "in vec 2 TexCoord" variable definitions in the shader.

    glutSwapBuffers();
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
    switch (Key)
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

int main(int argc, char** argv)
{
  t17::GLUTBackendInit(argc, argv);

  if (!t17::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 17")) {
    return 1;
  }

  auto app = std::make_shared<t17::MainApp>();

  if (!app->Init()) {
    return 1;
  }

  app->Run();

  return 0;
}
