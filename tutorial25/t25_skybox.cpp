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
*/

#include <utility>
#include "t25_skybox.h"
#include "t25_pipeline.h"

namespace t25
{
  SkyBox::SkyBox(std::shared_ptr<Camera> pCamera, const PersProjInfo& p)
    : m_pCamera(std::move(pCamera))
    , m_persProjInfo(p)
  {
  }

  bool SkyBox::Init(const std::string& Directory,
    const std::string& PosXFilename,
    const std::string& NegXFilename,
    const std::string& PosYFilename,
    const std::string& NegYFilename,
    const std::string& PosZFilename,
    const std::string& NegZFilename)
  {
    m_pSkyboxTechnique = std::make_shared<SkyBoxTechnique>();

    if (!m_pSkyboxTechnique->Init()) {
      printf("Error initializing the skybox technique\n");
      return false;
    }

    m_pSkyboxTechnique->Enable();
    m_pSkyboxTechnique->SetTextureUnit(0);

    m_pCubemapTex = std::make_shared<CubemapTexture>(Directory,
      PosXFilename, NegXFilename,
      PosYFilename, NegYFilename,
      PosZFilename, NegZFilename);

    if (!m_pCubemapTex->Load()) {
      return false;
    }

    m_pMesh = std::make_shared<Mesh>();
    return m_pMesh->LoadMesh("../Content/sphere.obj");
  }

  void SkyBox::Render() const
  {
    m_pSkyboxTechnique->Enable();

    GLint OldCullFaceMode = INVALID_OGL_VALUE;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);

    GLint OldDepthFuncMode = INVALID_OGL_VALUE;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    Pipeline pipeline;
    pipeline.Scale(20.0f, 20.0f, 20.0f);
    pipeline.Rotate(0.0f, 0.0f, 0.0f);
    pipeline.WorldPos(m_pCamera->GetPos().x, m_pCamera->GetPos().y, m_pCamera->GetPos().z);
    pipeline.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
    pipeline.SetPerspectiveProj(m_persProjInfo);

    m_pSkyboxTechnique->SetWVP(pipeline.GetWVPTrans());
    m_pCubemapTex->Bind(GL_TEXTURE0);
    m_pMesh->Render();

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
  }
}