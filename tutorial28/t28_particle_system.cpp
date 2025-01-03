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

#include "t28_engine_common.h"
#include "t28_util.h"
#include "t28_particle_system.h"
#include "t28_math_3d.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

namespace t28
{
  struct Particle
  {
    float    Type;
    Vector3f Pos;
    Vector3f Vel;
    float    LifetimeMillis;
  };


  ParticleSystem::ParticleSystem()
  {
    ZERO_MEM(m_transformFeedback);
    ZERO_MEM(m_particleBuffer);
  }


  ParticleSystem::~ParticleSystem()
  {
    if (m_transformFeedback[0] != 0)
      glDeleteTransformFeedbacks(2, m_transformFeedback);

    if (m_particleBuffer[0] != 0)
      glDeleteBuffers(2, m_particleBuffer);
  }


  bool ParticleSystem::Init(const Vector3f& pos)
  {
    Particle particles[MAX_PARTICLES];
    memset(particles, 0, sizeof particles);

    particles[0].Type = PARTICLE_TYPE_LAUNCHER;
    particles[0].Pos = pos;
    particles[0].Vel = Vector3f(0.0f, 0.0001f, 0.0f);
    particles[0].LifetimeMillis = 0.0f;

    glGenTransformFeedbacks(2, m_transformFeedback);
    glGenBuffers(2, m_particleBuffer);

    for (unsigned int i = 0; i < 2; i++) {
      constexpr GLuint transformFeedbackBufferIndex = 0;

      glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);

      glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, transformFeedbackBufferIndex, m_particleBuffer[i]);

      glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackBufferIndex); // IM: this call was absent from code but present in the docs, the fact of presence does not change anything

      glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
    }

    if (!m_updateTechnique.Init())
      return false;
    m_updateTechnique.Enable();
    m_updateTechnique.SetRandomTextureUnit(INDEX_OF(RANDOM_TEXTURE_UNIT));
    m_updateTechnique.SetLauncherLifetime(100.0f);
    m_updateTechnique.SetShellLifetime(10000.0f);
    m_updateTechnique.SetSecondaryShellLifetime(2500.0f);

    if (!m_randomTexture.InitRandomTexture(1000))
      return false;

    m_randomTexture.Bind(RANDOM_TEXTURE_UNIT);

    if (!m_billboardTechnique.Init())
      return false;
    m_billboardTechnique.Enable();
    m_billboardTechnique.SetColorTextureUnit(INDEX_OF(COLOR_TEXTURE_UNIT));
    m_billboardTechnique.SetBillboardSize(0.01f);

    m_pTexture = std::make_shared<Texture>(GL_TEXTURE_2D, "../Content/fireworks_red.jpg");
    if (!m_pTexture->Load())
      return false;

    return GLCheckError();
  }


  void ParticleSystem::Render(int deltaTimeMillis, const Matrix4f& VP, const Vector3f& cameraPos)
  {
    m_time += deltaTimeMillis;

    UpdateParticles(deltaTimeMillis);

    RenderParticles(VP, cameraPos);

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
  }


  void ParticleSystem::UpdateParticles(int dtMillis)
  {
    m_updateTechnique.Enable();
    m_updateTechnique.SetTime(m_time);
    m_updateTechnique.SetDeltaTimeMillis(dtMillis);

    m_randomTexture.Bind(RANDOM_TEXTURE_UNIT);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid*>( 0)); // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid*>( 4)); // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid*>(16)); // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid*>(28)); // lifetime

    glBeginTransformFeedback(GL_POINTS);

    if (m_isFirst) {
      glDrawArrays(GL_POINTS, 0, 1);

      m_isFirst = false;
    }
    else {
      glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
  }


  void ParticleSystem::RenderParticles(const Matrix4f& VP, const Vector3f& cameraPos)
  {
    m_billboardTechnique.Enable();
    m_billboardTechnique.SetCameraPosition(cameraPos);
    m_billboardTechnique.SetVP(VP);
    m_pTexture->Bind(COLOR_TEXTURE_UNIT);

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid*>(4)); // position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

    glDisableVertexAttribArray(0);
  }
}
