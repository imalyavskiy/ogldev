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

#include "camera.h"
#include "skybox_technique.h"
#include "cubemap_texture.h"
#include "mesh.h"

class SkyBox
{
public:
    SkyBox(const Camera* pCamera, const PersProjInfo& p);

    ~SkyBox();
    
    bool Init(const std::string& Directory,
              const std::string& PosXFilename,
              const std::string& NegXFilename,
              const std::string& PosYFilename,
              const std::string& NegYFilename,
              const std::string& PosZFilename,
              const std::string& NegZFilename);
    
    void Render();
    
private:    
    SkyboxTechnique* m_pSkyboxTechnique;
    const Camera* m_pCamera;
    CubemapTexture* m_pCubemapTex;
    Mesh* m_pMesh;
    PersProjInfo m_persProjInfo;
};

#endif	/* SKYBOX_H */

