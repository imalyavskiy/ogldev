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

#include "t28_billboard_technique.h"
#include "t28_util.h"

namespace t28
{
  static const char* pVertexShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout(location = 0) in vec3 Position;                                              \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      gl_Position = vec4(Position, 1.0);                                              \n"\
  "  }                                                                                   \n";



  static const char* pGeometryShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout(points) in;                                                                  \n"\
  "  layout(triangle_strip) out;                                                         \n"\
  "  layout(max_vertices = 4) out;                                                       \n"\
  "                                                                                      \n"\
  "  uniform mat4 gVP;                                                                   \n"\
  "  uniform vec3 gCameraPos;                                                            \n"\
  "  uniform float gBillboardSize;                                                       \n"\
  "                                                                                      \n"\
  "  out vec2 TexCoord;                                                                  \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      vec3 Pos = gl_in[0].gl_Position.xyz;                                            \n"\
  "      vec3 toCamera = normalize(gCameraPos - Pos);                                    \n"\
  "      vec3 up = vec3(0.0, 1.0, 0.0);                                                  \n"\
  "      vec3 right = cross(toCamera, up) * gBillboardSize;                              \n"\
  "                                                                                      \n"\
  "      Pos -= right;                                                                   \n"\
  "      gl_Position = gVP * vec4(Pos, 1.0);                                             \n"\
  "      TexCoord = vec2(0.0, 0.0);                                                      \n"\
  "      EmitVertex();                                                                   \n"\
  "                                                                                      \n"\
  "      Pos.y += gBillboardSize;                                                        \n"\
  "      gl_Position = gVP * vec4(Pos, 1.0);                                             \n"\
  "      TexCoord = vec2(0.0, 1.0);                                                      \n"\
  "      EmitVertex();                                                                   \n"\
  "                                                                                      \n"\
  "      Pos.y -= gBillboardSize;                                                        \n"\
  "      Pos += right;                                                                   \n"\
  "      gl_Position = gVP * vec4(Pos, 1.0);                                             \n"\
  "      TexCoord = vec2(1.0, 0.0);                                                      \n"\
  "      EmitVertex();                                                                   \n"\
  "                                                                                      \n"\
  "      Pos.y += gBillboardSize;                                                        \n"\
  "      gl_Position = gVP * vec4(Pos, 1.0);                                             \n"\
  "      TexCoord = vec2(1.0, 1.0);                                                      \n"\
  "      EmitVertex();                                                                   \n"\
  "                                                                                      \n"\
  "      EndPrimitive();                                                                 \n"\
  "  }                                                                                   \n";

  static const char* pFragmentShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  uniform sampler2D gColorMap;                                                        \n"\
  "                                                                                      \n"\
  "  in vec2 TexCoord;                                                                   \n"\
  "  out vec4 FragColor;                                                                 \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      FragColor = texture2D(gColorMap, TexCoord);                                     \n"\
  "                                                                                      \n"\
  "      if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {           \n"\
  "          discard;                                                                    \n"\
  "      }                                                                               \n"\
  "  }                                                                                   \n";

  bool BillboardTechnique::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, pVertexShaderText))
      return false;

    if (!AddShader(GL_GEOMETRY_SHADER, pGeometryShaderText))
      return false;

    if (!AddShader(GL_FRAGMENT_SHADER, pFragmentShaderText))
      return false;

    if (!Finalize())
      return false;

    m_VPLocation = GetUniformLocation("gVP");
    if (m_VPLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_cameraPosLocation = GetUniformLocation("gCameraPos");
    if (m_cameraPosLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_colorMapLocation = GetUniformLocation("gColorMap");
    if (m_billboardSizeLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_billboardSizeLocation = GetUniformLocation("gBillboardSize");
    if (m_colorMapLocation == INVALID_UNIFORM_LOCATION)
      return false;
    
    return GLCheckError();
  }

  void BillboardTechnique::SetVP(const Matrix4f& VP)
  {
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(VP.m));    
  }

  void BillboardTechnique::SetCameraPosition(const Vector3f& pos)
  {
    glUniform3f(m_cameraPosLocation, pos.x, pos.y, pos.z);
  }

  void BillboardTechnique::SetColorTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_colorMapLocation, textureUnit);
  }

  void BillboardTechnique::SetBillboardSize(float billboardSize)
  {
    glUniform1f(m_billboardSizeLocation, billboardSize);
  }
}