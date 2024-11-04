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

#ifndef SKYBOX_H
#define	SKYBOX_H

#include <memory>
#include "t25_camera.h"
#include "t25_skybox_technique.h"
#include "t25_cubemap_texture.h"
#include "t25_mesh.h"

namespace t25
{
  class SkyBox
  {
  public:
    SkyBox(std::shared_ptr<Camera> pCamera, const PersProjInfo& p);

    ~SkyBox() = default;
    
    bool Init(const std::string& Directory,
              const std::string& PosXFilename, const std::string& NegXFilename,
              const std::string& PosYFilename, const std::string& NegYFilename,
              const std::string& PosZFilename, const std::string& NegZFilename);
    
    void Render() const;
    
  private:    
    std::shared_ptr<SkyboxTechnique> m_pSkyboxTechnique;
    std::shared_ptr<Camera> m_pCamera;
    std::shared_ptr<CubemapTexture> m_pCubemapTex;
    std::shared_ptr<Mesh> m_pMesh;
    PersProjInfo m_persProjInfo;
  };
}

#endif	/* SKYBOX_H */

