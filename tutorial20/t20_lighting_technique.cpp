#include <array>

#include "t20_lighting_technique.h"

static const char* pVS =
"  #version 330                                                                                \n"\
"                                                                                              \n"\
"  layout (location = 0) in vec3 Position;                                                     \n"\
"  layout (location = 1) in vec2 TexCoord;                                                     \n"\
"  layout (location = 2) in vec3 Normal;                                                       \n"\
"                                                                                              \n"\
"  uniform mat4 gWVP;                                                                          \n"\
"  uniform mat4 gWorld;                                                                        \n"\
"                                                                                              \n"\
"  out vec2 TexCoord0;                                                                         \n"\
"  out vec3 Normal0;                                                                           \n"\
"  out vec3 WorldPos0;                                                                         \n"\
"                                                                                              \n"\
"  void main()                                                                                 \n"\
"  {                                                                                           \n"\
"    gl_Position = gWVP * vec4(Position, 1.0);                                                 \n"\
"    TexCoord0   = TexCoord;                                                                   \n"\
"    Normal0     = (gWorld * vec4(Normal, 0.0)).xyz;                                           \n"\
"    WorldPos0   = (gWorld * vec4(Position, 1.0)).xyz;                                         \n"\
"  }                                                                                           \n"\
"                                                                                                ";

static const char* pFS =
"  #version 330                                                                                \n"\
"                                                                                              \n"\
"  const int MAX_POINT_LIGHTS = 3;                                                             \n"\
"                                                                                              \n"\
"  in vec2 TexCoord0;                                                                          \n"\
"  in vec3 Normal0;                                                                            \n"\
"  in vec3 WorldPos0;                                                                          \n"\
"                                                                                              \n"\
"  out vec4 FragColor;                                                                         \n"\
"                                                                                              \n"\
"  struct BaseLight                                                                            \n"\
"  {                                                                                           \n"\
"      vec3 Color;                                                                             \n"\
"      float AmbientIntensity;                                                                 \n"\
"      float DiffuseIntensity;                                                                 \n"\
"  };                                                                                          \n"\
"                                                                                              \n"\
"  struct DirectionalLight                                                                     \n"\
"  {                                                                                           \n"\
"      BaseLight Base;                                                                         \n"\
"      vec3 Direction;                                                                         \n"\
"  };                                                                                          \n"\
"                                                                                              \n"\
"  struct Attenuation                                                                          \n"\
"  {                                                                                           \n"\
"      float Constant;                                                                         \n"\
"      float Linear;                                                                           \n"\
"      float Exp;                                                                              \n"\
"  };                                                                                          \n"\
"                                                                                              \n"\
"  struct PointLight                                                                           \n"\
"  {                                                                                           \n"\
"      BaseLight Base;                                                                         \n"\
"      vec3 Position;                                                                          \n"\
"      Attenuation Atten;                                                                      \n"\
"  };                                                                                          \n"\
"                                                                                              \n"\
"  uniform int gNumPointLights;                                                                \n"\
"  uniform DirectionalLight gDirectionalLight;                                                 \n"\
"  uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          \n"\
"  uniform sampler2D gSampler;                                                                 \n"\
"  uniform vec3 gEyeWorldPos;                                                                  \n"\
"  uniform float gMatSpecularIntensity;                                                        \n"\
"  uniform float gSpecularPower;                                                               \n"\
"                                                                                              \n"\
"  vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)                   \n"\
"  {                                                                                           \n"\
"    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                     \n"\
"    float DiffuseFactor = dot(Normal, -LightDirection);                                       \n"\
"                                                                                              \n"\
"    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                    \n"\
"    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                    \n"\
"                                                                                              \n"\
"    if (DiffuseFactor > 0) {                                                                  \n"\
"      DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;        \n"\
"                                                                                              \n"\
"      vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                                 \n"\
"      vec3 LightReflect = normalize(reflect(LightDirection, Normal));                         \n"\
"      float SpecularFactor = dot(VertexToEye, LightReflect);                                  \n"\
"      SpecularFactor = pow(SpecularFactor, gSpecularPower);                                   \n"\
"      if (SpecularFactor > 0) {                                                               \n"\
"        SpecularColor = vec4(Light.Color, 1.0f) *                                             \n"\
"                        gMatSpecularIntensity * SpecularFactor;                               \n"\
"      }                                                                                       \n"\
"    }                                                                                         \n"\
"                                                                                              \n"\
"    return (AmbientColor + DiffuseColor + SpecularColor);                                     \n"\
"  }                                                                                           \n"\
"                                                                                              \n"\
"  vec4 CalcDirectionalLight(vec3 Normal)                                                      \n"\
"  {                                                                                           \n"\
"    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);    \n"\
"  }                                                                                           \n"\
"                                                                                              \n"\
"  vec4 CalcPointLight(int Index, vec3 Normal)                                                 \n"\
"  {                                                                                           \n"\
"    vec3 LightDirection = WorldPos0 - gPointLights[Index].Position;                           \n"\
"    float Distance = length(LightDirection);                                                  \n"\
"    LightDirection = normalize(LightDirection);                                               \n"\
"                                                                                              \n"\
"    vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);         \n"\
"    float Attenuation =  gPointLights[Index].Atten.Constant +                                 \n"\
"                         gPointLights[Index].Atten.Linear * Distance +                        \n"\
"                         gPointLights[Index].Atten.Exp * Distance * Distance;                 \n"\
"                                                                                              \n"\
"    return Color / Attenuation;                                                               \n"\
"  }                                                                                           \n"\
"                                                                                              \n"\
"  void main()                                                                                 \n"\
"  {                                                                                           \n"\
"    vec3 Normal = normalize(Normal0);                                                         \n"\
"    vec4 TotalLight = CalcDirectionalLight(Normal);                                           \n"\
"                                                                                              \n"\
"    for (int i = 0 ; i < gNumPointLights ; i++) {                                             \n"\
"      TotalLight += CalcPointLight(i, Normal);                                                \n"\
"    }                                                                                         \n"\
"                                                                                              \n"\
"    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;                               \n"\
"  }                                                                                           \n"\
"                                                                                                ";


namespace t20
{
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
    if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    if (m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_samplerLocation = GetUniformLocation("gSampler");
    if (m_samplerLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    if (m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
    if (m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION)
      return false;

    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
    if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION)
      return false;

    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    if (m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION)
      return false;

    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
    if (m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION)
      return false;

    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    if (m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    if (m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION)
      return false;

    m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
    if (m_numPointLightsLocation == INVALID_UNIFORM_LOCATION)
      return false;

    for (unsigned int i = 0 ; i < m_pointLightsLocation.size() ; i++) {
      char name[128] = {};

      snprintf(name, sizeof(name), "gPointLights[%d].Base.Color", i);
      m_pointLightsLocation[i].Color = GetUniformLocation(name);
      if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Base.AmbientIntensity", i);
      m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(name);
      if (m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Position", i);
      m_pointLightsLocation[i].Position = GetUniformLocation(name);
      if (m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Base.DiffuseIntensity", i);
      m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(name);
      if (m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Atten.Constant", i);
      m_pointLightsLocation[i].attenuation.Constant = GetUniformLocation(name);
      if (m_pointLightsLocation[i].attenuation.Constant == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Atten.Linear", i);
      m_pointLightsLocation[i].attenuation.Linear = GetUniformLocation(name);
      if (m_pointLightsLocation[i].attenuation.Linear == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Atten.Exp", i);
      m_pointLightsLocation[i].attenuation.Exp = GetUniformLocation(name);
      if (m_pointLightsLocation[i].attenuation.Exp == INVALID_UNIFORM_LOCATION)
        return false;
    }

    return true;
  }

  void LightingTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }


  void LightingTechnique::SetWorldMatrix(const Matrix4f& world)
  {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(world.m));
  }


  void LightingTechnique::SetTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_samplerLocation, textureUnit - GL_TEXTURE0);
  }


  void LightingTechnique::SetDirectionalLight(const DirectionalLight& light)
  {
    glUniform3f(m_dirLightLocation.Color, light.color.x, light.color.y, light.color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, light.ambientIntensity);
    Vector3f Direction = light.direction;
    Direction.Normalize();
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, light.diffuseIntensity);
  }

  void LightingTechnique::SetEyeWorldPos(const Vector3f& eyeWorldPos)
  {
    glUniform3f(m_eyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);
  }

  void LightingTechnique::SetMatSpecularIntensity(float intensity)
  {
    glUniform1f(m_matSpecularIntensityLocation, intensity);
  }

  void LightingTechnique::SetMatSpecularPower(float power)
  {
    glUniform1f(m_matSpecularPowerLocation, power);
  }

  void LightingTechnique::SetPointLights(unsigned int numLights, const PointLight* pLights)
  {
    glUniform1i(m_numPointLightsLocation, numLights);

    for (unsigned int i = 0 ; i < numLights ; i++) {
      glUniform3f(m_pointLightsLocation[i].Color, pLights[i].color.x, pLights[i].color.y, pLights[i].color.z);
      glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].ambientIntensity);
      glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].diffuseIntensity);
      glUniform3f(m_pointLightsLocation[i].Position, pLights[i].position.x, pLights[i].position.y, pLights[i].position.z);
      glUniform1f(m_pointLightsLocation[i].attenuation.Constant, pLights[i].attenuation.constant);
      glUniform1f(m_pointLightsLocation[i].attenuation.Linear, pLights[i].attenuation.linear);
      glUniform1f(m_pointLightsLocation[i].attenuation.Exp, pLights[i].attenuation.exp);
    }
  }
}
