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

#include "t20_mainapp.h"

namespace t20
{
  Vertex::Vertex(const Vector3f& pos, const Vector2f& tex)
  : m_pos(pos)
  , m_tex(tex)
  , m_normal{ 0.0f, 0.0f, 0.0f }
  { }

  MainApp::MainApp(GLint winWidth, GLint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_directionalLight.color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.ambientIntensity = -0.1f;
    m_directionalLight.diffuseIntensity = 0.0f;
    m_directionalLight.direction = Vector3f(1.0f, 0.0f, 0.0f);
  }

  bool MainApp::Init()
  {
    Vector3f pos(0.0f, 0.0f, 0.0f);
    Vector3f target(0.0f, 0.0f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

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

  void MainApp::Run()
  {
    GLUTBackendRun(this);
  }

  void MainApp::RenderSceneCB()
  {
    m_pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    m_scale += 0.01f;

    std::array<PointLight, MAX_POINT_LIGHTS> pointLights;

    pointLights[0].diffuseIntensity = 0.5f;
    pointLights[0].color = Vector3f(1.0f, 0.0f, 0.0f);
    pointLights[0].position = Vector3f(sinf(m_scale) * 10, 1.0f, cosf(m_scale) * 10);
    pointLights[0].attenuation.linear = 0.1f;

    pointLights[1].diffuseIntensity = 0.5f;
    pointLights[1].color = Vector3f(0.0f, 1.0f, 0.0f);
    pointLights[1].position = Vector3f(sinf(m_scale + 2.1f) * 10, 1.0f, cosf(m_scale + 2.1f) * 10);
    pointLights[1].attenuation.linear = 0.1f;

    pointLights[2].diffuseIntensity = 0.5f;
    pointLights[2].color = Vector3f(0.0f, 0.0f, 1.0f);
    pointLights[2].position = Vector3f(sinf(m_scale + 4.2f) * 10, 1.0f, cosf(m_scale + 4.2f) * 10);
    pointLights[2].attenuation.linear = 0.1f;

    m_pEffect->SetPointLights(pointLights.size(), pointLights.data());

    Pipeline pipeline;
    pipeline.Rotate(0.0f, 0.0f, 0.0f);
    pipeline.WorldPos(0.0f, 0.0f, 1.0f);
    pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    pipeline.SetPerspectiveProj(60.0f, static_cast<float>(m_winWidth), static_cast<float>(m_winHeight), 0.1f, 100.0f);

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
    case 'a':
      m_directionalLight.ambientIntensity -= 0.05f;
      break;
    case 's':
      m_directionalLight.ambientIntensity += 0.05f;
      break;
    case 'z':
      m_directionalLight.diffuseIntensity -= 0.05f;
      break;
    case 'x':
      m_directionalLight.diffuseIntensity += 0.05f;
      break;
    }
  }

  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
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

  void MainApp::CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
  {
    std::array<Vertex, 4> Vertices = {
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

  void MainApp::CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
  {
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
  }
}