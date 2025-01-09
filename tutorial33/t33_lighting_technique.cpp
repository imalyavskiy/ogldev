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

#include <climits>
#include <cstring>

#include "t33_math_3d.h"
#include "t33_lighting_technique.h"
#include "t33_util.h"
#include "t33_technique.h"

namespace t33 {

  static const char* pVertexShader =
  "  #version 410                                                                                \n"\
  "                                                                                              \n"\
  "  layout (location = 0) in vec3 Position;                                                     \n"\
  "  layout (location = 1) in vec2 TexCoord;                                                     \n"\
  "  layout (location = 2) in vec3 Normal;                                                       \n"\
  "  layout (location = 3) in mat4 WVP;                                                          \n"\
  "  layout (location = 7) in mat4 World;                                                        \n"\
  "                                                                                              \n"\
  "  out vec2 TexCoord0;                                                                         \n"\
  "  out vec3 Normal0;                                                                           \n"\
  "  out vec3 WorldPos0;                                                                         \n"\
  "  flat out int InstanceID;                                                                    \n"\
  "                                                                                              \n"\
  "  void main()                                                                                 \n"\
  "  {                                                                                           \n"\
  "      gl_Position = WVP * vec4(Position, 1.0);                                                \n"\
  "      TexCoord0   = TexCoord;                                                                 \n"\
  "      Normal0     = (World * vec4(Normal, 0.0)).xyz;                                          \n"\
  "      WorldPos0   = (World * vec4(Position, 1.0)).xyz;                                        \n"\
  "      InstanceID = gl_InstanceID;                                                             \n"\
  "  }                                                                                           \n";

  static const char* pFragmentShader =
  "  #version 410                                                                                \n"\
  "                                                                                              \n"\
  "  const int MAX_POINT_LIGHTS = 2;                                                             \n"\
  "  const int MAX_SPOT_LIGHTS = 2;                                                              \n"\
  "                                                                                              \n"\
  "  in vec2 TexCoord0;                                                                          \n"\
  "  in vec3 Normal0;                                                                            \n"\
  "  in vec3 WorldPos0;                                                                          \n"\
  "  flat in int InstanceID;                                                                     \n"\
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
  "  struct SpotLight                                                                            \n"\
  "  {                                                                                           \n"\
  "      PointLight Base;                                                                        \n"\
  "      vec3 Direction;                                                                         \n"\
  "      float Cutoff;                                                                           \n"\
  "  };                                                                                          \n"\
  "                                                                                              \n"\
  "  uniform int gNumPointLights;                                                                \n"\
  "  uniform int gNumSpotLights;                                                                 \n"\
  "  uniform DirectionalLight gDirectionalLight;                                                 \n"\
  "  uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          \n"\
  "  uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                                             \n"\
  "  uniform sampler2D gColorMap;                                                                \n"\
  "  uniform vec3 gEyeWorldPos;                                                                  \n"\
  "  uniform float gMatSpecularIntensity;                                                        \n"\
  "  uniform float gSpecularPower;                                                               \n"\
  "  uniform vec4 gColor[4];                                                                     \n"\
  "                                                                                              \n"\
  "  vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)                   \n"\
  "  {                                                                                           \n"\
  "      vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                   \n"\
  "      float DiffuseFactor = dot(Normal, -LightDirection);                                     \n"\
  "                                                                                              \n"\
  "      vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  \n"\
  "      vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  \n"\
  "                                                                                              \n"\
  "      if (DiffuseFactor > 0) {                                                                \n"\
  "          DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;    \n"\
  "                                                                                              \n"\
  "          vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                             \n"\
  "          vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     \n"\
  "          float SpecularFactor = dot(VertexToEye, LightReflect);                              \n"\
  "          SpecularFactor = pow(SpecularFactor, gSpecularPower);                               \n"\
  "          if (SpecularFactor > 0) {                                                           \n"\
  "              SpecularColor = vec4(Light.Color, 1.0f) *                                       \n"\
  "                              gMatSpecularIntensity * SpecularFactor;                         \n"\
  "          }                                                                                   \n"\
  "      }                                                                                       \n"\
  "                                                                                              \n"\
  "      return (AmbientColor + DiffuseColor + SpecularColor);                                   \n"\
  "  }                                                                                           \n"\
  "                                                                                              \n"\
  "  vec4 CalcDirectionalLight(vec3 Normal)                                                      \n"\
  "  {                                                                                           \n"\
  "      return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);  \n"\
  "  }                                                                                           \n"\
  "                                                                                              \n"\
  "  vec4 CalcPointLight(PointLight l, vec3 Normal)                                              \n"\
  "  {                                                                                           \n"\
  "      vec3 LightDirection = WorldPos0 - l.Position;                                           \n"\
  "      float Distance = length(LightDirection);                                                \n"\
  "      LightDirection = normalize(LightDirection);                                             \n"\
  "                                                                                              \n"\
  "      vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);                         \n"\
  "      float Attenuation =  l.Atten.Constant +                                                 \n"\
  "                           l.Atten.Linear * Distance +                                        \n"\
  "                           l.Atten.Exp * Distance * Distance;                                 \n"\
  "                                                                                              \n"\
  "      return Color / Attenuation;                                                             \n"\
  "  }                                                                                           \n"\
  "                                                                                              \n"\
  "  vec4 CalcSpotLight(SpotLight l, vec3 Normal)                                                \n"\
  "  {                                                                                           \n"\
  "      vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);                             \n"\
  "      float SpotFactor = dot(LightToPixel, l.Direction);                                      \n"\
  "                                                                                              \n"\
  "      if (SpotFactor > l.Cutoff) {                                                            \n"\
  "          vec4 Color = CalcPointLight(l.Base, Normal);                                        \n"\
  "          return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   \n"\
  "      }                                                                                       \n"\
  "      else {                                                                                  \n"\
  "          return vec4(0,0,0,0);                                                               \n"\
  "      }                                                                                       \n"\
  "  }                                                                                           \n"\
  "                                                                                              \n"\
  "  void main()                                                                                 \n"\
  "  {                                                                                           \n"\
  "      vec3 Normal = normalize(Normal0);                                                       \n"\
  "      vec4 TotalLight = CalcDirectionalLight(Normal);                                         \n"\
  "                                                                                              \n"\
  "      for (int i = 0 ; i < gNumPointLights ; i++) {                                           \n"\
  "          TotalLight += CalcPointLight(gPointLights[i], Normal);                              \n"\
  "      }                                                                                       \n"\
  "                                                                                              \n"\
  "      for (int i = 0 ; i < gNumSpotLights ; i++) {                                            \n"\
  "          TotalLight += CalcSpotLight(gSpotLights[i], Normal);                                \n"\
  "      }                                                                                       \n"\
  "                                                                                              \n"\
  "      FragColor = texture(gColorMap, TexCoord0.xy) * TotalLight * gColor[InstanceID % 4];     \n"\
  "  }                                                                                           \n";
  
  LightingTechnique::LightingTechnique()
  {
  }

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

    m_colorTextureLocation = GetUniformLocation("gColorMap");
    if (m_colorTextureLocation == INVALID_UNIFORM_LOCATION)
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

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
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

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); i++) {
      char Name[128];
      memset(Name, 0, sizeof(Name));
      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
      m_spotLightsLocation[i].Color = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
      m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
      m_spotLightsLocation[i].Position = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
      m_spotLightsLocation[i].Direction = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
      m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
      m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
      m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
      m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION)
        return false;

      snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
      m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);
      if (m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION)
        return false;
    }

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_colorLocation); i++) {
      char name[32] = {};
      snprintf(name, sizeof(name), "gColor[%d]", i);

      m_colorLocation[i] = GetUniformLocation(name);
      if (m_colorLocation[i] == INVALID_UNIFORM_LOCATION)
        return false;
    }

    return true;
  }


  void LightingTechnique::SetColorTextureUnit(unsigned int textureUnit)
  {
    glUniform1i(m_colorTextureLocation, textureUnit);
  }


  void LightingTechnique::SetDirectionalLight(const DirectionalLight& light)
  {
    glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);

    Vector3f direction = light.Direction;
    direction.Normalize();

    glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
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


  void LightingTechnique::SetPointLights(unsigned int numLights, const PointLight* pLights)
  {
    glUniform1i(m_numPointLightsLocation, numLights);

    for (unsigned int i = 0; i < numLights; i++) {
      glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
      glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
      glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
      glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
      glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
      glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
      glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
  }

  void LightingTechnique::SetSpotLights(unsigned int numLights, const SpotLight* pLights)
  {
    glUniform1i(m_numSpotLightsLocation, numLights);

    for (unsigned int i = 0; i < numLights; i++) {
      glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
      glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
      glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
      glUniform3f(m_spotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);

      Vector3f direction = pLights[i].Direction;
      direction.Normalize();

      glUniform3f(m_spotLightsLocation[i].Direction, direction.x, direction.y, direction.z);
      glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
      glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
      glUniform1f(m_spotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
      glUniform1f(m_spotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
  }

  void LightingTechnique::SetColor(unsigned int index, const Vector4f& color)
  {
    glUniform4f(m_colorLocation[index], color.x, color.y, color.z, color.w);
  }
}