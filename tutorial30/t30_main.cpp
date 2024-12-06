/*

  Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Tutorial 30 - Basic Tessellation
*/

#include <iostream>
#include <format>
#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "t30_engine_common.h"
#include "t30_util.h"
#include "t30_pipeline.h"
#include "t30_camera.h"
#include "t30_texture.h"
#include "t30_lighting_technique.h"
#include "t30_glut_backend.h"
#include "t30_mesh.h"

#define WINDOW_WIDTH  1680
#define WINDOW_HEIGHT 1050
#define WINDOW_BPP 32
#define WINDOW_FULLSCREEN false
#define WINDOW_TITLE "Tutorial 30"

namespace t30
{
  class MainApp : public ICallbacks
  {
  public:

    MainApp(int winWidth, int winHeight)
    : m_winWidth(winWidth)
    , m_winHeight(winHeight)
    {
      m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
      m_directionalLight.AmbientIntensity = 1.0f;
      m_directionalLight.DiffuseIntensity = 0.01f;
      m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);

      m_persProjInfo.FOV = 60.0f;
      m_persProjInfo.Height = m_winHeight;
      m_persProjInfo.Width = m_winWidth;
      m_persProjInfo.zNear = 1.0f;
      m_persProjInfo.zFar = 100.0f;

      m_dispFactor = 0.25f;
      m_isWireframe = false;
    }

    ~MainApp() = default;

    bool Init()
    {
      const Vector3f pos(0.0f, 1.0f, -5.0f);
      const Vector3f target(0.0f, -0.2f, 1.0f);
      const Vector3f up(0.0, 1.0f, 0.0f);

      m_pGameCamera = std::make_shared<Camera>(m_winWidth, m_winHeight, pos, target, up);

      if (!m_lightingEffect.Init()) {
        std::cerr << "Error initializing the lighting technique" << "\n";
        return false;
      }

      GLint maxPatchVertices = 0;
      glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
      printf("Max supported patch vertices %d\n", maxPatchVertices);
      //glPatchParameteri(GL_PATCH_VERTICES, 3);

      glActiveTexture(GL_TEXTURE4);
      m_pDisplacementMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/heightmap.jpg");

      if (!m_pDisplacementMap->Load())
        return false;

      m_pDisplacementMap->Bind(DISPLACEMENT_TEXTURE_UNIT);

      glActiveTexture(GL_TEXTURE0);
      m_pColorMap = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/diffuse.jpg");

      if (!m_pColorMap->Load())
        return false;

      m_pColorMap->Bind(COLOR_TEXTURE_UNIT);

      m_lightingEffect.Enable();
      GLExitIfError();
      m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
      m_lightingEffect.SetDisplacementMapTextureUnit(DISPLACEMENT_TEXTURE_UNIT_INDEX);
      GLExitIfError();
      m_lightingEffect.SetDirectionalLight(m_directionalLight);
      GLExitIfError();
      m_lightingEffect.SetDispFactor(m_dispFactor);
      m_pMesh = std::make_shared<Mesh>();

      return m_pMesh->LoadMesh("../Content/quad2.obj");
    }

    void Run()
    {
      GLUTBackendRun(this);
    }

    void RenderSceneCB() override
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      m_pGameCamera->OnRender();

      Pipeline pipeline;
      pipeline.Scale(2.0f, 2.0f, 2.0f);
      pipeline.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
      pipeline.SetPerspectiveProj(m_persProjInfo);

      // render the objects as usual
      //m_lightingEffect.Enable();
      GLExitIfError();
      m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());
      m_lightingEffect.SetVP(pipeline.GetVPTrans());
      m_lightingEffect.SetWorldMatrix(pipeline.GetWorldTrans());
      m_lightingEffect.SetDispFactor(m_dispFactor);
      GLExitIfError();
      m_pMesh->Render(nullptr);
      GLExitIfError();
      glutSwapBuffers();
    }

    void IdleCB() override
    {
      RenderSceneCB();
    }

    void SpecialKeyboardCB(int key, int x, int y) override
    {
      m_pGameCamera->OnKeyboard(key);
    }

    void KeyboardCB(unsigned char key, int x, int y) override
    {
      switch (key)
      {
        case 0x1b:
          glutLeaveMainLoop();
          break;

        case '+':
          m_dispFactor += 0.01f;
          break;

        case '-':
          if (m_dispFactor >= 0.01f)
            m_dispFactor -= 0.01f;
          break;

        case 'z':
          m_isWireframe = !m_isWireframe;
          if (m_isWireframe)
            glPolygonMode(GL_FRONT, GL_LINE);
          else
            glPolygonMode(GL_FRONT, GL_FILL);
          break;
      }
    }

    void PassiveMouseCB(int x, int y) override
    {
      m_pGameCamera->OnMouse(x, y);
    }


    void MouseCB(int button, int state, int x, int y) override
    {
    }

  private:
    int m_winWidth = 0;
    int m_winHeight = 0;

    LightingTechnique m_lightingEffect;
    std::shared_ptr<Camera> m_pGameCamera;
    DirectionalLight m_directionalLight;
    std::shared_ptr<Mesh> m_pMesh;
    PersProjInfo m_persProjInfo;
    std::shared_ptr<Texture> m_pDisplacementMap;
    std::shared_ptr<Texture> m_pColorMap;
    float m_dispFactor;
    bool m_isWireframe;
  };
}

int main(int argc, char** argv)
{
  std::srand(/*WINAPI->*/GetCurrentProcessId());

  t30::GLUTBackendInit(argc, argv);

  if (!t30::GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOW_FULLSCREEN, WINDOW_TITLE))
    return 1;

  const auto app = std::make_shared<t30::MainApp>(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!app->Init())
    return 1;

  app->Run();

  return 0;
}