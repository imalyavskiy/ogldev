
#include <climits>
#include <cstring>

#include "t21_lighting_technique.h"
#include "t21_util.h"

namespace t21
{
  static const char* pVS =
 "  #version 330                                                                 \n"\
 "                                                                               \n"\
 "  layout (location = 0) in vec3 Position;                                      \n"\
 "  layout (location = 1) in vec2 TexCoord;                                      \n"\
 "  layout (location = 2) in vec3 Normal;                                        \n"\
 "                                                                               \n"\
 "  uniform mat4 gWVP;                                                           \n"\
 "  uniform mat4 gWorld;                                                         \n"\
 "                                                                               \n"\
 "  out vec2 TexCoord0;                                                          \n"\
 "  out vec3 Normal0;                                                            \n"\
 "  out vec3 WorldPos0;                                                          \n"\
 "                                                                               \n"\
 "  void main()                                                                  \n"\
 "  {                                                                            \n"\
 "    gl_Position = gWVP * vec4(Position, 1.0);                                  \n"\
 "    TexCoord0   = TexCoord;                                                    \n"\
 "    Normal0     = (gWorld * vec4(Normal, 0.0)).xyz;                            \n"\
 "    WorldPos0   = (gWorld * vec4(Position, 1.0)).xyz;                          \n"\
 "  }                                                                              ";

  static const char* pFS =
  "  #version 330                                                                \n"\
  "                                                                              \n"\
  "  const int MAX_POINT_LIGHTS = 2;                                             \n"\
  "  const int MAX_SPOT_LIGHTS = 2;                                              \n"\
  "                                                                              \n"\
  "  in vec2 TexCoord0;                                                          \n"\
  "  in vec3 Normal0;                                                            \n"\
  "  in vec3 WorldPos0;                                                          \n"\
  "                                                                              \n"\
  "  out vec4 FragColor;                                                         \n"\
  "                                                                              \n"\
  "  struct BaseLight                                                            \n"\
  "  {                                                                           \n"\
  "    vec3 Color;                                                               \n"\
  "    float AmbientIntensity;                                                   \n"\
  "    float DiffuseIntensity;                                                   \n"\
  "  };                                                                          \n"\
  "                                                                              \n"\
  "  struct DirectionalLight                                                     \n"\
  "  {                                                                           \n"\
  "    BaseLight Base;                                                           \n"\
  "    vec3 Direction;                                                           \n"\
  "  };                                                                          \n"\
  "                                                                              \n"\
  "  struct Attenuation                                                          \n"\
  "  {                                                                           \n"\
  "    float Constant;                                                           \n"\
  "    float Linear;                                                             \n"\
  "    float Exp;                                                                \n"\
  "  };                                                                          \n"\
  "                                                                              \n"\
  "  struct PointLight                                                           \n"\
  "  {                                                                           \n"\
  "    BaseLight Base;                                                           \n"\
  "    vec3 Position;                                                            \n"\
  "    Attenuation Atten;                                                        \n"\
  "  };                                                                          \n"\
  "                                                                              \n"\
  "  struct SpotLight                                                            \n"\
  "  {                                                                           \n"\
  "    PointLight Base;                                                          \n"\
  "    vec3 Direction;                                                           \n"\
  "    float Cutoff;                                                             \n"\
  "  };                                                                          \n"\
  "                                                                              \n"\
  "  uniform int gNumPointLights;                                                \n"\
  "  uniform int gNumSpotLights;                                                 \n"\
  "  uniform DirectionalLight gDirectionalLight;                                 \n"\
  "  uniform PointLight gPointLights[MAX_POINT_LIGHTS];                          \n"\
  "  uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                             \n"\
  "  uniform sampler2D gSampler;                                                 \n"\
  "  uniform vec3 gEyeWorldPos;                                                  \n"\
  "  uniform float gMatSpecularIntensity;                                        \n"\
  "  uniform float gSpecularPower;                                               \n"\
  "                                                                              \n"\
  "  vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)   \n"\
  "  {                                                                           \n"\
  "    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;     \n"\
  "    float DiffuseFactor = dot(Normal, -LightDirection);                       \n"\
  "                                                                              \n"\
  "    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                    \n"\
  "    vec4 SpecularColor = vec4(0, 0, 0, 0);                                    \n"\
  "                                                                              \n"\
  "    if (DiffuseFactor > 0) {                                                  \n"\
  "      DiffuseColor = vec4(Light.Color, 1.0f) *                                \n"\
  "                     Light.DiffuseIntensity *                                 \n"\
  "                     DiffuseFactor;                                           \n"\
  "                                                                              \n"\
  "      vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                 \n"\
  "      vec3 LightReflect = normalize(reflect(LightDirection, Normal));         \n"\
  "      float SpecularFactor = dot(VertexToEye, LightReflect);                  \n"\
  "      SpecularFactor = pow(SpecularFactor, gSpecularPower);                   \n"\
  "      if (SpecularFactor > 0) {                                               \n"\
  "        SpecularColor = vec4(Light.Color, 1.0f) *                             \n"\
  "                        gMatSpecularIntensity * SpecularFactor;               \n"\
  "      }                                                                       \n"\
  "    }                                                                         \n"\
  "                                                                              \n"\
  "    return (AmbientColor + DiffuseColor + SpecularColor);                     \n"\
  "  }                                                                           \n"\
  "                                                                              \n"\
  "  vec4 CalcDirectionalLight(vec3 Normal)                                      \n"\
  "  {                                                                           \n"\
  "    return CalcLightInternal(gDirectionalLight.Base,                          \n"\
  "                             gDirectionalLight.Direction,                     \n"\
  "                             Normal);                                         \n"\
  "  }                                                                           \n"\
  "                                                                              \n"\
  "  vec4 CalcPointLight(PointLight l, vec3 Normal)                              \n"\
  "  {                                                                           \n"\
  "    vec3 LightDirection = WorldPos0 - l.Position;                             \n"\
  "    float Distance = length(LightDirection);                                  \n"\
  "    LightDirection = normalize(LightDirection);                               \n"\
  "                                                                              \n"\
  "    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);           \n"\
  "    float Attenuation =  l.Atten.Constant +                                   \n"\
  "                         l.Atten.Linear * Distance +                          \n"\
  "                         l.Atten.Exp * Distance * Distance;                   \n"\
  "                                                                              \n"\
  "    return Color / Attenuation;                                               \n"\
  "  }                                                                           \n"\
  "                                                                              \n"\
  "  vec4 CalcSpotLight( SpotLight l, vec3 Normal)                               \n"\
  "  {                                                                           \n"\
  "    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);               \n"\
  "    float SpotFactor = dot(LightToPixel, l.Direction);                        \n"\
  "                                                                              \n"\
  "    if (SpotFactor > l.Cutoff) {                                              \n"\
  "      vec4 Color = CalcPointLight(l.Base, Normal);                            \n"\
  "      return Color *                                                          \n"\
  "              (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - l.Cutoff));            \n"\
  "    }                                                                         \n"\
  "    else {                                                                    \n"\
  "      return vec4(0,0,0,0);                                                   \n"\
  "    }                                                                         \n"\
  "  }                                                                           \n"\
  "                                                                              \n"\
  "  void main()                                                                 \n"\
  "  {                                                                           \n"\
  "    vec3 Normal = normalize(Normal0);                                         \n"\
  "    vec4 TotalLight = CalcDirectionalLight(Normal);                           \n"\
  "                                                                              \n"\
  "    for (int i = 0 ; i < gNumPointLights ; i++) {                             \n"\
  "      TotalLight += CalcPointLight(gPointLights[i], Normal);                  \n"\
  "    }                                                                         \n"\
  "                                                                              \n"\
  "    for (int i = 0 ; i < gNumSpotLights ; i++) {                              \n"\
  "      TotalLight += CalcSpotLight(gSpotLights[i], Normal);                    \n"\
  "    }                                                                         \n"\
  "                                                                              \n"\
  "    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;               \n"\
  "  }                                                                           ";

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

    m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");
    if (m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION)
      return false;

    for (unsigned int i = 0 ; i < m_pointLightsLocation.size() ; i++) 
    {
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
      m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(name);
      if (m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Atten.Linear", i);
      m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(name);
      if (m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gPointLights[%d].Atten.Exp", i);
      m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(name);
      if (m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION)
        return false;
    }

    for (unsigned int i = 0 ; i < m_spotLightsLocation.size() ; i++) 
    {
      char name[128] = {};

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Base.Color", i);
      m_spotLightsLocation[i].Color = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
      m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(name);
      if (m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Position", i);
      m_spotLightsLocation[i].Position = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Direction", i);
      m_spotLightsLocation[i].Direction = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Cutoff", i);
      m_spotLightsLocation[i].Cutoff = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
      m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(name);
      if (m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Atten.Constant", i);
      m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Atten.Linear", i);
      m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(name, sizeof(name), "gSpotLights[%d].Base.Atten.Exp", i);
      m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(name);
      if (m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION)
        return false;
    }

    return true;
  }

  void LightingTechnique::SetWVP(const Matrix4f& WVP)
  {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WVP.m));
  }


  void LightingTechnique::SetWorldMatrix(const Matrix4f& WorldInverse)
  {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(WorldInverse.m));
  }


  void LightingTechnique::SetTextureUnit(uint32_t textureUnit)
  {
    glUniform1i(m_samplerLocation, textureUnit - GL_TEXTURE0);
  }
  
  void LightingTechnique::SetDirectionalLight(const DirectionalLight& light)
  {
    glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);

    Vector3f Direction = light.Direction;
    Direction.Normalize();

    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, light.DiffuseIntensity);
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

  void LightingTechnique::SetPointLights(uint32_t NumLights, const PointLight* pLights)
  {
    glUniform1i(m_numPointLightsLocation, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++) {
      glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
      glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
      glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
      glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
      glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
      glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
      glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
  }

  void LightingTechnique::SetSpotLights(uint32_t numLights, const SpotLight* pLights)
  {
    glUniform1i(m_numSpotLightsLocation, numLights);

    for (unsigned int i = 0 ; i < numLights ; i++) {
      glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
      glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
      glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
      glUniform3f(m_spotLightsLocation[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);

      Vector3f Direction = pLights[i].Direction;
      Direction.Normalize();

      glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
      glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
      glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
      glUniform1f(m_spotLightsLocation[i].Atten.Linear,   pLights[i].Attenuation.Linear);
      glUniform1f(m_spotLightsLocation[i].Atten.Exp,      pLights[i].Attenuation.Exp);
    }
  }
}
