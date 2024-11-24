#include "t18_lighting_technique.h"
namespace t18
{
  static const char* pVertexShader =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  layout (location = 0) in vec3 Position;                                             \n"\
  "  layout (location = 1) in vec2 TexCoord;                                             \n"\
  "  layout (location = 2) in vec3 Normal;                                               \n"\
  "                                                                                      \n"\
  "  uniform mat4 gWVP;                                                                  \n"\
  "  uniform mat4 gWorld;                                                                \n"\
  "                                                                                      \n"\
  "  out vec2 TexCoord0;                                                                 \n"\
  "  out vec3 Normal0;                                                                   \n"\
  "                                                                                      \n"\
  "  void main()                                                                         \n"\
  "  {                                                                                   \n"\
  "    gl_Position = gWVP * vec4(Position, 1.0);                                         \n"\
  "    TexCoord0 = TexCoord;                                                             \n"\
  "    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;                                       \n"\
  "  }                                                                                     ";

  static const char* pFragmentShader =
  "  #version 330                                                                        \n"\
  "                                                                                      \n"\
  "  in vec2 TexCoord0;                                                                  \n"\
  "  in vec3 Normal0;                                                                    \n"\
  "                                                                                      \n"\
  "  out vec4 FragColor;                                                                 \n"\
  "                                                                                      \n"\
  "  struct DirectionalLight {                                                           \n"\
  "    vec3  Color;                                                                      \n"\
  "    float AmbientIntensity;                                                           \n"\
  "    vec3  Direction;                                                                  \n"\
  "    float DiffuseIntensity;                                                           \n"\
  "  };                                                                                  \n"\
  "                                                                                      \n"\
  "  uniform DirectionalLight gDirectionalLight;                                         \n"\
  "  uniform sampler2D gSampler;                                                         \n"\
  "                                                                                      \n"\
  "  void main() {                                                                       \n"\
  "    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *                         \n"\
  "                        gDirectionalLight.AmbientIntensity;                           \n"\
  "                                                                                      \n"\
  "    vec3 LightDirection = (-1) * gDirectionalLight.Direction;                         \n"\
  "    vec3 Normal = normalize(Normal0);                                                 \n"\
  "                                                                                      \n"\
  "    float DiffuseFactor = clamp( dot( Normal, LightDirection ), 0, 1 );               \n"\
  "                                                                                      \n"\
  "    vec4  DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *                        \n"\
  "                         gDirectionalLight.DiffuseIntensity *                         \n"\
  "                         DiffuseFactor;                                               \n"\
  "                                                                                      \n"\
  "    FragColor = texture2D(gSampler, TexCoord0.xy) *                                   \n"\
  "                (AmbientColor + DiffuseColor);                                        \n"\
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

    m_WVPLocation =
      GetUniformLocation("gWVP");
    if (m_WVPLocation == 0xFFFFFFFF)
      return false;

    m_WorldMatrixLocation =
      GetUniformLocation("gWorld");
    if (m_WorldMatrixLocation == 0xFFFFFFFF)
      return false;

    m_samplerLocation =
      GetUniformLocation("gSampler");
    if (m_samplerLocation == 0xFFFFFFFF)
      return false;

    m_dirLightLocation.Color =
      GetUniformLocation("gDirectionalLight.Color");
    if (m_dirLightLocation.Color == 0xFFFFFFFF)
      return false;

    m_dirLightLocation.AmbientIntensity =
      GetUniformLocation("gDirectionalLight.AmbientIntensity");
    if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF)
      return false;

    m_dirLightLocation.Direction =
      GetUniformLocation("gDirectionalLight.Direction");
    if (m_dirLightLocation.Direction == 0xFFFFFFFF)
      return false;

    m_dirLightLocation.DiffuseIntensity =
      GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    if (m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF) 
      return false;

    return true;
  }

  void LightingTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }

  void LightingTechnique::SetWorldMatrix(const Matrix4f& W)
  {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(W.m));
  }
  
  void LightingTechnique::SetTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_samplerLocation, textureUnit - GL_TEXTURE0);
  }
  
  void LightingTechnique::SetDirectionalLight(const DirectionLight& light)
  {
    glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);

    const Vector3f direction = light.Direction.Normalized();

    glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, light.DiffuseIntensity);
  }
}
