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

#include "t29_simple_color_technique.h"
#include "t29_util.h"

namespace t29
{
  static const char* pVertexShaderText =
  "  #version 410                                                                        \n"\
  "                                                                                      \n"\
  "  layout (location = 0) in vec3 Position;                                             \n"\
  "                                                                                      \n"\
  "  uniform mat4 gWVP;                                                                  \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      gl_Position = gWVP * vec4(Position, 1.0);                                       \n"\
  "  }                                                                                   \n";


  static const char* pFragmentShaderText =
  "  #version 410                                                                        \n"\
  "                                                                                      \n"\
  "  layout(location = 0) out vec4 FragColor;                                            \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                           \n"\
  "  }                                                                                   \n";



  SimpleColorTechnique::SimpleColorTechnique()
    : Technique("SimpleColorTechnique")
  {   
  }

  bool SimpleColorTechnique::Init()
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

    return true;
  }

  void SimpleColorTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));    
  }
}
