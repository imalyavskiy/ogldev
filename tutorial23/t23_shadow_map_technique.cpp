#include "t23_shadow_map_technique.h"

namespace t23
{
  static const char* pVertexShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout (location = 0) in vec3 Position;                                             \n"\
  "  layout (location = 1) in vec2 TexCoord;                                             \n"\
  "  layout (location = 2) in vec3 Normal;                                               \n"\
  "                                                                                      \n"\
  "  uniform mat4 gWVP;                                                                  \n"\
  "                                                                                      \n"\
  "  out vec2 TexCoordOut;                                                               \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      gl_Position = gWVP * vec4(Position, 1.0);                                       \n"\
  "      TexCoordOut = TexCoord;                                                         \n"\
  "  }                                                                                     ";

  static const char* pFragmentShaderText =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  in vec2 TexCoordOut;                                                                \n"\
  "  uniform sampler2D gShadowMap;                                                       \n"\
  "                                                                                      \n"\
  "  out vec4 FragColor;                                                                 \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "      float Depth = texture(gShadowMap, TexCoordOut).x;                               \n"\
  "      Depth = 1.0 - (1.0 - Depth) * 25.0;                                             \n"\
  "      FragColor = vec4(Depth);                                                        \n"\
  "  }                                                                                     ";

  bool ShadowMapTechnique::Init() {
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

    m_textureLocation = GetUniformLocation("gShadowMap");
    if (m_textureLocation == INVALID_UNIFORM_LOCATION)
      return false;

    return true;
  }

  void ShadowMapTechnique::SetWVP(const Matrix4f& WVP) {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }

  void ShadowMapTechnique::SetTextureUnit(unsigned int textureUnit) {
    glUniform1i(m_textureLocation, textureUnit - GL_TEXTURE0);
  }
}
