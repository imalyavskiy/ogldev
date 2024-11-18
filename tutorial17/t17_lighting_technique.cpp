#include "t17_lighting_technique.h"

namespace t17
{
static const char* pVertexShader =
"  #version 330                                                                        \n"\
"                                                                                      \n"\
"  layout (location = 0) in vec3 Position;                                             \n"\
"  layout (location = 1) in vec2 TexCoord;                                             \n"\
"                                                                                      \n"\
"  uniform mat4 gWVP;                                                                  \n"\
"                                                                                      \n"\
"  out vec2 TexCoord0;                                                                 \n"\
"                                                                                      \n"\
"  void main()                                                                         \n"\
"  {                                                                                   \n"\
"    gl_Position = gWVP * vec4(Position, 1.0);                                         \n"\
"    TexCoord0 = TexCoord;                                                             \n"\
"  }                                                                                     ";

static const char* pFragmentShader =
"  #version 330                                                                        \n"\
"                                                                                      \n"\
"  in vec2 TexCoord0;                                                                  \n"\
"                                                                                      \n"\
"  out vec4 FragColor;                                                                 \n"\
"                                                                                      \n"\
"  struct DirectionalLight                                                             \n"\
"  {                                                                                   \n"\
"    vec3 Color;                                                                       \n"\
"    float AmbientIntensity;                                                           \n"\
"  };                                                                                  \n"\
"                                                                                      \n"\
"  uniform DirectionalLight gDirectionalLight;                                         \n"\
"  uniform sampler2D gSampler;                                                         \n"\
"                                                                                      \n"\
"  void main() {                                                                       \n"\
"    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *                         \n"\
"                        gDirectionalLight.AmbientIntensity;                           \n"\
"                                                                                      \n"\
"    FragColor = texture2D(gSampler, TexCoord0.xy) *                                   \n"\
"                AmbientColor;                                                         \n"\
"  }                                                                                     ";


  bool LightingTechnique::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, pVertexShader))
      return false;

    if (!AddShader(GL_FRAGMENT_SHADER, pFragmentShader))
      return false;

    if (!Finalize())
      return false;

    m_WVPLocation = GetUniformLocation("gWVP");
    if (m_WVPLocation == 0xFFFFFFFF)
      return false;

    m_samplerLocation = GetUniformLocation("gSampler");
    if (m_samplerLocation == 0xFFFFFFFF)
      return false;

    m_dirLightColorLocation = GetUniformLocation("gDirectionalLight.Color");
    if (m_dirLightColorLocation == 0xFFFFFFFF)
      return false;

    m_dirLightAmbientIntensityLocation = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    if (m_dirLightAmbientIntensityLocation == 0xFFFFFFFF)
      return false;

    return true;
  }

  void LightingTechnique::SetWVP(const Matrix4f* WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP->m));
  }


  void LightingTechnique::SetTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_samplerLocation, textureUnit);
  }


  void LightingTechnique::SetDirectionalLight(const DirectionLight& light)
  {
    glUniform3f(m_dirLightColorLocation, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_dirLightAmbientIntensityLocation, light.AmbientIntensity);
  }
}