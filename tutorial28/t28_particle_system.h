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

#ifndef PARTICLE_SYSTEM_H
#define	PARTICLE_SYSTEM_H

#include <memory>
#include <GL/glew.h>

#include "t28_ps_update_technique.h"
#include "t28_random_texture.h"
#include "t28_billboard_technique.h"
#include "t28_texture.h"

namespace t28
{
  class ParticleSystem
  {
  public:
    ParticleSystem();
    
    ~ParticleSystem();
    
    bool InitParticleSystem(const Vector3f& pos);
    
    void Render(int deltaTimeMillis, const Matrix4f& VP, const Vector3f& cameraPos);
    
  private:
    
    void UpdateParticles(int deltaTimeMillis);
    void RenderParticles(const Matrix4f& VP, const Vector3f& cameraPos);
    
    bool m_isFirst = true;
    uint32_t m_currVB = 0; // Vertex Buffer
    uint32_t m_currTFB = 1; // Transform Feedback Buffer
    GLuint m_particleBuffer[2] = {0};
    GLuint m_transformFeedback[2] = {0};
    PSUpdateTechnique m_updateTechnique;
    BillboardTechnique m_billboardTechnique;
    RandomTexture m_randomTexture;
    std::shared_ptr<Texture> m_pTexture;
    int m_time = 0;
  };
}

#endif	/* PARTICLE_SYSTEM_H */

