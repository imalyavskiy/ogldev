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

#include "t25_skybox_technique.h"

namespace t25
{
  static const char* pVertexShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout (location = 0) in vec3 Position;                                             \n"\
  "                                                                                      \n"\
  "  uniform mat4 gWVP;                                                                  \n"\
  "                                                                                      \n"\
  "  out vec3 TexCoord0;                                                                 \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      vec4 WVP_Pos = gWVP * vec4(Position, 1.0);                                      \n"\
  "      gl_Position = WVP_Pos.xyww;                                                     \n"\
  "      TexCoord0   = Position;                                                         \n"\
  "  }                                                                                     ";

  static const char* pFragmentShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  in vec3 TexCoord0;                                                                  \n"\
  "                                                                                      \n"\
  "  out vec4 FragColor;                                                                 \n"\
  "                                                                                      \n"\
  "  uniform samplerCube gCubemapTexture;                                                \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      FragColor = texture(gCubemapTexture, TexCoord0);                                \n"\
  "  }                                                                                     ";


  bool SkyBoxTechnique::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, pVertexShaderText))
      return false;

    if (!AddShader(GL_FRAGMENT_SHADER, pFragmentShaderText))
      return false;

    if (!Finalize())
      return false;

    m_WVPLocation = GetUniformLocation("gWVP");
    if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_textureLocation = GetUniformLocation("gCubemapTexture");
    if (m_textureLocation == INVALID_UNIFORM_LOCATION)
      return false;

    return true;
  }


  void SkyBoxTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));    
  }


  void SkyBoxTechnique::SetTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_textureLocation, textureUnit - GL_TEXTURE0);
  }
}
