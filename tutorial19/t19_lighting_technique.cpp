#include "t19_lighting_technique.h"
namespace t19
{
  static const char* pVS = 
  "  #version 330                                                                          \n"\
  "                                                                                        \n"\
  "  layout (location = 0) in vec3 Position;                                               \n"\
  "  layout (location = 1) in vec2 TexCoord;                                               \n"\
  "  layout (location = 2) in vec3 Normal;                                                 \n"\
  "                                                                                        \n"\
  "  uniform mat4 gWVP;                                                                    \n"\
  "  uniform mat4 gWorld;                                                                  \n"\
  "                                                                                        \n"\
  "  out vec2 TexCoord0;                                                                   \n"\
  "  out vec3 Normal0;                                                                     \n"\
  "  out vec3 WorldPos0;                                                                   \n"\
  "                                                                                        \n"\
  "  void main() {                                                                         \n"\
  "      gl_Position = gWVP * vec4(Position, 1.0);                                         \n"\
  "      TexCoord0 = TexCoord;                                                             \n"\
  "      Normal0   = (gWorld * vec4(Normal, 0.0)).xyz;                                     \n"\
  "      WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;                                   \n"\
  "  }                                                                                       ";

  static const char* pFS = 
  "  #version 330                                                                          \n"\
  "                                                                                        \n"\
  "  in vec2 TexCoord0;                                                                    \n"\
  "  in vec3 Normal0;                                                                      \n"\
  "  in vec3 WorldPos0;                                                                    \n"\
  "                                                                                        \n"\
  "  out vec4 FragColor;                                                                   \n"\
  "                                                                                        \n"\
  "  struct DirectionalLight {                                                             \n"\
  "      vec3 Color;                                                                       \n"\
  "      float AmbientIntensity;                                                           \n"\
  "      vec3 Direction;                                                                   \n"\
  "      float DiffuseIntensity;                                                           \n"\
  "  };                                                                                    \n"\
  "                                                                                        \n"\
  "  uniform DirectionalLight gDirectionalLight;                                           \n"\
  "  uniform sampler2D gSampler;                                                           \n"\
  "                                                                                        \n"\
  "  uniform vec3 gEyeWorldPos;                                                            \n"\
  "  uniform float gMatSpecularIntensity;                                                  \n"\
  "  uniform float gSpecularPower;                                                         \n"\
  "                                                                                        \n"\
  "  void main() {                                                                         \n"\
  "    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *                           \n"\
  "                        gDirectionalLight.AmbientIntensity;                             \n"\
  "                                                                                        \n"\
  "    vec3 LightDirection = -gDirectionalLight.Direction;                                 \n"\
  "    vec3 Normal = normalize(Normal0);                                                   \n"\
  "                                                                                        \n"\
  "    float DiffuseFactor = dot(Normal, LightDirection);                                  \n"\
  "                                                                                        \n"\
  "    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                              \n"\
  "    vec4 SpecularColor = vec4(0, 0, 0, 0);                                              \n"\
  "    if (DiffuseFactor > 0) {                                                            \n"\
  "      DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *                              \n"\
  "                     gDirectionalLight.DiffuseIntensity *                               \n"\
  "                     DiffuseFactor;                                                     \n"\
  "                                                                                        \n"\
  "      vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                           \n"\
  "      vec3 LightReflect = normalize( reflect( gDirectionalLight.Direction, Normal ) );  \n"\
  "      float SpecularFactor = dot(VertexToEye, LightReflect);                            \n"\
  "      SpecularFactor = pow(SpecularFactor, gSpecularPower);                             \n"\
  "                                                                                        \n"\
  "      if (SpecularFactor > 0) {                                                         \n"\
  "        SpecularColor = vec4(gDirectionalLight.Color, 1.0f) *                           \n"\
  "                        gMatSpecularIntensity *                                         \n"\
  "                        SpecularFactor;                                                 \n"\
  "      }                                                                                 \n"\
  "    }                                                                                   \n"\
  "                                                                                        \n"\
  "    FragColor = texture2D(gSampler, TexCoord0.xy) *                                     \n"\
  "                (AmbientColor + DiffuseColor + SpecularColor);                          \n"\
  "  }                                                                                       ";

  bool LightingTechnique::Init()
  {
    if (!Technique::Init())
      return false;

    if (!AddShader(GL_VERTEX_SHADER, pVS))
      return false;

    if (!AddShader(GL_FRAGMENT_SHADER, pFS))
      return false;

    if (!Finalize())
      return false;

    m_WVPLocation = GetUniformLocation("gWVP");
    if (m_WVPLocation == 0xFFFFFFFF)
      return false;
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    if (m_WorldMatrixLocation == 0xFFFFFFFF)
      return false;
    m_samplerLocation = GetUniformLocation("gSampler");
    if (m_samplerLocation == 0xFFFFFFFF)
      return false;

    m_eyeWorldPosition = GetUniformLocation("gEyeWorldPos");
    if (m_eyeWorldPosition == 0xFFFFFFFF)
      return false;
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    if (m_matSpecularIntensityLocation == 0xFFFFFFFF)
      return false;
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    if (m_matSpecularPowerLocation == 0xFFFFFFFF)
      return false;

    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
    if (m_dirLightLocation.Color == 0xFFFFFFFF)
      return false;
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF)
      return false;
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    if (m_dirLightLocation.Direction == 0xFFFFFFFF)
      return false;
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    if (m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF)
      return false;

    return true;
  }

  void LightingTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }

  void LightingTechnique::SetWorldMatrix(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }

  void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
  {
    glUniform1i(m_samplerLocation, TextureUnit - GL_TEXTURE0);
  }

  void LightingTechnique::SetMatSpecularIntensity(float intensity)
  {
    glUniform1f(m_matSpecularIntensityLocation, intensity);
  }

  void LightingTechnique::SetMatSpecularPower(float power)
  {
    glUniform1f(m_matSpecularPowerLocation, power);
  }

  void LightingTechnique::SetEyeWorldPos(const Vector3f& eyeWorldPos)
  {
    glUniform3f(m_eyeWorldPosition, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);
  }

  void LightingTechnique::SetDirectionalLight(const DirectionLight& Light)
  {
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);

    Vector3f Direction = Light.Direction;
    Direction.Normalize();

    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
  }
}
