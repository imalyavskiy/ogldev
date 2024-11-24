#include <array>
#include <memory>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t21_pipeline.h"
#include "t21_glut_backend.h"
#include "t21_util.h"

#include "t21_mainapp.h"

namespace t21
{
  Vertex::Vertex(const Vector3f& pos, const Vector2f& tex)
  : m_pos(pos)
  , m_tex(tex)
  , m_normal{ 0.0f, 0.0f, 0.0f }
  {
  }

  MainApp::MainApp(GLint winWidth, GLint winHeight)
  : m_winWidth(winWidth)
  , m_winHeight(winHeight)
  {
    m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.0f;
    m_directionalLight.DiffuseIntensity = 0.0f;
    m_directionalLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);
  }

  bool MainApp::Init()
  {
    Vector3f pos(-10.0f, 0.0f, -10.0f);
    Vector3f target(1.0f, 0.0f, 1.0f);
    Vector3f up(0.0, 1.0f, 0.0f);

    m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

    const std::array<uint32_t, 6> indices = { 0, 2, 1,  0, 3, 2 };

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

    std::array<SpotLight, 2> spotLights;

    spotLights[0].DiffuseIntensity = 15.0f;
    spotLights[0].Color = Vector3f(1.0f, 1.0f, 0.7f);
    spotLights[0].Position = Vector3f(-0.0f, -1.9f, -0.0f);
    spotLights[0].Direction = Vector3f(sinf(m_scale), 0.0f, cosf(m_scale));
    spotLights[0].Attenuation.Linear = 0.1f;
    spotLights[0].Cutoff = 20.0f;

    spotLights[1].DiffuseIntensity = 5.0f;
    spotLights[1].Color = Vector3f(0.0f, 1.0f, 1.0f);
    spotLights[1].Position = m_pGameCamera->GetPos();
    spotLights[1].Direction = m_pGameCamera->GetTarget();
    spotLights[1].Attenuation.Linear = 0.1f;
    spotLights[1].Cutoff = 10.0f;

    m_pEffect->SetSpotLights(2, spotLights.data());

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(0));
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
    case 0x1b:
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


  void MainApp::PassiveMouseCB(int x, int y)
  {
    m_pGameCamera->OnMouse(x, y);
  }

  void MainApp::CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int vertexCount) {
    for (unsigned int i = 0; i < IndexCount; i += 3) {
      const uint32_t i0 = pIndices[i + 0];
      const uint32_t i1 = pIndices[i + 1];
      const uint32_t i2 = pIndices[i + 2];

      Vector3f v1 = pVertices[i1].m_pos - pVertices[i0].m_pos;
      Vector3f v2 = pVertices[i2].m_pos - pVertices[i0].m_pos;
      Vector3f Normal = v1.Cross(v2);

      Normal.Normalize();

      pVertices[i0].m_normal += Normal;
      pVertices[i1].m_normal += Normal;
      pVertices[i2].m_normal += Normal;
    }

    for (unsigned int i = 0; i < vertexCount; i++) {
      pVertices[i].m_normal.Normalize();
    }
  }

  void MainApp::CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
  {
    std::array<Vertex, 4> Vertices{
      Vertex(Vector3f(-10.0f, -2.0f, -10.0f), Vector2f(0.0f, 0.0f)),
      Vertex(Vector3f(10.0f, -2.0f, -10.0f), Vector2f(1.0f, 0.0f)),
      Vertex(Vector3f(10.0f, -2.0f,  10.0f), Vector2f(1.0f, 1.0f)),
      Vertex(Vector3f(-10.0f, -2.0f,  10.0f), Vector2f(0.0f, 1.0f))
    };

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