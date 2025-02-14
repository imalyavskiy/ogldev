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

#include "t28_ps_update_technique.h"

namespace t28
{
  static const char* pVertexShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout (location = 0) in float Type;                                                \n"\
  "  layout (location = 1) in vec3 Position;                                             \n"\
  "  layout (location = 2) in vec3 Velocity;                                             \n"\
  "  layout (location = 3) in float Age;                                                 \n"\
  "                                                                                      \n"\
  "  out float Type0;                                                                    \n"\
  "  out vec3 Position0;                                                                 \n"\
  "  out vec3 Velocity0;                                                                 \n"\
  "  out float Age0;                                                                     \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      Type0 = Type;                                                                   \n"\
  "      Position0 = Position;                                                           \n"\
  "      Velocity0 = Velocity;                                                           \n"\
  "      Age0 = Age;                                                                     \n"\
  "  }                                                                                   \n";

  static const char* pGeometryShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout(points) in;                                                                  \n"\
  "  layout(points) out;                                                                 \n"\
  "  layout(max_vertices = 30) out;                                                      \n"\
  "                                                                                      \n"\
  "  in float Type0[];                                                                   \n"\
  "  in vec3 Position0[];                                                                \n"\
  "  in vec3 Velocity0[];                                                                \n"\
  "  in float Age0[];                                                                    \n"\
  "                                                                                      \n"\
  "  out float Type1;                                                                    \n"\
  "  out vec3 Position1;                                                                 \n"\
  "  out vec3 Velocity1;                                                                 \n"\
  "  out float Age1;                                                                     \n"\
  "                                                                                      \n"\
  "  uniform float gDeltaTimeMillis;                                                     \n"\
  "  uniform float gTime;                                                                \n"\
  "  uniform sampler1D gRandomTexture;                                                   \n"\
  "  uniform float gLauncherLifetime;                                                    \n"\
  "  uniform float gShellLifetime;                                                       \n"\
  "  uniform float gSecondaryShellLifetime;                                              \n"\
  "                                                                                      \n"\
  "  #define PARTICLE_TYPE_LAUNCHER 0.0f                                                 \n"\
  "  #define PARTICLE_TYPE_SHELL 1.0f                                                    \n"\
  "  #define PARTICLE_TYPE_SECONDARY_SHELL 2.0f                                          \n"\
  "                                                                                      \n"\
  "  vec3 GetRandomDir(float TexCoord)                                                   \n"\
  "  {                                                                                   \n"\
  "       vec3 Dir = texture(gRandomTexture, TexCoord).xyz;                              \n"\
  "       Dir -= vec3(0.5, 0.5, 0.5);                                                    \n"\
  "       return Dir;                                                                    \n"\
  "  }                                                                                   \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      float Age = Age0[0] + gDeltaTimeMillis;                                         \n"\
  "                                                                                      \n"\
  "      if (Type0[0] == PARTICLE_TYPE_LAUNCHER) {                                       \n"\
  "          if (Age >= gLauncherLifetime) {                                             \n"\
  "              Type1 = PARTICLE_TYPE_SHELL;                                            \n"\
  "              Position1 = Position0[0];                                               \n"\
  "              vec3 Dir = GetRandomDir(gTime/1000.0);                                  \n"\
  "              Dir.y = max(Dir.y, 0.5);                                                \n"\
  "              Velocity1 = normalize(Dir) / 20.0;                                      \n"\
  "              Age1 = 0.0;                                                             \n"\
  "              EmitVertex();                                                           \n"\
  "              EndPrimitive();                                                         \n"\
  "              Age = 0.0;                                                              \n"\
  "          }                                                                           \n"\
  "                                                                                      \n"\
  "          Type1 = PARTICLE_TYPE_LAUNCHER;                                             \n"\
  "          Position1 = Position0[0];                                                   \n"\
  "          Velocity1 = Velocity0[0];                                                   \n"\
  "          Age1 = Age;                                                                 \n"\
  "          EmitVertex();                                                               \n"\
  "          EndPrimitive();                                                             \n"\
  "      }                                                                               \n"\
  "      else {                                                                          \n"\
  "          float DeltaTimeSecs = gDeltaTimeMillis / 1000.0f;                           \n"\
  "          float t1 = Age0[0] / 1000.0;                                                \n"\
  "          float t2 = Age / 1000.0;                                                    \n"\
  "          vec3 DeltaP = DeltaTimeSecs * Velocity0[0];                                 \n"\
  "          vec3 DeltaV = vec3(DeltaTimeSecs) * (0.0, -9.81, 0.0);                      \n"\
  "                                                                                      \n"\
  "          if (Type0[0] == PARTICLE_TYPE_SHELL)  {                                     \n"\
  "  	        if (Age < gShellLifetime) {                                                \n"\
  "  	            Type1 = PARTICLE_TYPE_SHELL;                                           \n"\
  "  	            Position1 = Position0[0] + DeltaP;                                     \n"\
  "  	            Velocity1 = Velocity0[0] + DeltaV;                                     \n"\
  "  	            Age1 = Age;                                                            \n"\
  "  	            EmitVertex();                                                          \n"\
  "  	            EndPrimitive();                                                        \n"\
  "  	        }                                                                          \n"\
  "              else {                                                                  \n"\
  "                  for (int i = 0 ; i < 10 ; i++) {                                    \n"\
  "                       Type1 = PARTICLE_TYPE_SECONDARY_SHELL;                         \n"\
  "                       Position1 = Position0[0];                                      \n"\
  "                       vec3 Dir = GetRandomDir((gTime + i)/1000.0);                   \n"\
  "                       Velocity1 = normalize(Dir) / 20.0;                             \n"\
  "                       Age1 = 0.0f;                                                   \n"\
  "                       EmitVertex();                                                  \n"\
  "                       EndPrimitive();                                                \n"\
  "                  }                                                                   \n"\
  "              }                                                                       \n"\
  "          }                                                                           \n"\
  "          else {                                                                      \n"\
  "              if (Age < gSecondaryShellLifetime) {                                    \n"\
  "                  Type1 = PARTICLE_TYPE_SECONDARY_SHELL;                              \n"\
  "                  Position1 = Position0[0] + DeltaP;                                  \n"\
  "                  Velocity1 = Velocity0[0] + DeltaV;                                  \n"\
  "                  Age1 = Age;                                                         \n"\
  "                  EmitVertex();                                                       \n"\
  "                  EndPrimitive();                                                     \n"\
  "              }                                                                       \n"\
  "          }                                                                           \n"\
  "      }                                                                               \n"\
  "  }                                                                                   \n";

  PSUpdateTechnique::PSUpdateTechnique()
    : Technique("PSUpdateTechnique")
  {
  }

  bool PSUpdateTechnique::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, pVertexShaderText))
      return false;

    if (!AddShader(GL_GEOMETRY_SHADER, pGeometryShaderText))
      return false;

    const GLchar* varyings[4] = { "Type1", "Position1", "Velocity1", "Age1" };

    glTransformFeedbackVaryings(m_shaderProg, 4, varyings, GL_INTERLEAVED_ATTRIBS);

    if (!Finalize())
      return false;

    m_deltaTimeMillisLocation = GetUniformLocation("gDeltaTimeMillis");
    if (m_deltaTimeMillisLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_randomTextureLocation = GetUniformLocation("gRandomTexture");
    if (m_timeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_timeLocation = GetUniformLocation("gTime");
    if (m_randomTextureLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_launcherLifetimeLocation = GetUniformLocation("gLauncherLifetime");
    if (m_launcherLifetimeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_shellLifetimeLocation = GetUniformLocation("gShellLifetime");
    if (m_shellLifetimeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_secondaryShellLifetimeLocation = GetUniformLocation("gSecondaryShellLifetime");
    if (m_secondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    return true;
  }

  void PSUpdateTechnique::SetDeltaTimeMillis(float deltaTimeMillis)
  {
    glUniform1f(m_deltaTimeMillisLocation, deltaTimeMillis);
  }

  void PSUpdateTechnique::SetTime(int time)
  {
    glUniform1f(m_timeLocation, (float)time);
  }

  void PSUpdateTechnique::SetRandomTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_randomTextureLocation, textureUnit);
  }

  void PSUpdateTechnique::SetLauncherLifetime(float lifetime)
  {
    glUniform1f(m_launcherLifetimeLocation, lifetime);
  }

  void PSUpdateTechnique::SetShellLifetime(float lifetime)
  {
    glUniform1f(m_shellLifetimeLocation, lifetime);
  }

  void PSUpdateTechnique::SetSecondaryShellLifetime(float lifetime)
  {
    glUniform1f(m_secondaryShellLifetimeLocation, lifetime);
  }
}