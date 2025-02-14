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

#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include "t28_technique.h"
#include "t28_math_3d.h"

namespace t28
{
  struct BaseLight
  {
    Vector3f Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
      Color = Vector3f(0.0f, 0.0f, 0.0f);
      AmbientIntensity = 0.0f;
      DiffuseIntensity = 0.0f;
    }
  };

  struct DirectionalLight : public BaseLight
  {        
    Vector3f Direction;

    DirectionalLight()
    {
      Direction = Vector3f(0.0f, 0.0f, 0.0f);
    }
  };

  struct PointLight : public BaseLight
  {
    Vector3f Position;

    struct
    {
      float Constant;
      float Linear;
      float Exp;
    } Attenuation;

    PointLight()
    {
      Position = Vector3f(0.0f, 0.0f, 0.0f);
      Attenuation.Constant = 1.0f;
      Attenuation.Linear = 0.0f;
      Attenuation.Exp = 0.0f;
    }
  };

  struct SpotLight : public PointLight
  {
    Vector3f Direction;
    float Cutoff;

    SpotLight()
    {
      Direction = Vector3f(0.0f, 0.0f, 0.0f);
      Cutoff = 0.0f;
    }
  };

  class LightingTechnique : public Technique 
  {
  public:

    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;

    LightingTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetLightWVP(const Matrix4f& lightWVP);
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetColorTextureUnit(unsigned int textureUnit);
    void SetShadowMapTextureUnit(unsigned int textureUnit);
    void SetNormalMapTextureUnit(unsigned int textureUnit);
    void SetDirectionalLight(const DirectionalLight& light);
    void SetPointLights(unsigned int numLights, const PointLight* pLights);
    void SetSpotLights(unsigned int numLights, const SpotLight* pLights);
    void SetEyeWorldPos(const Vector3f& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);

  private:

    GLuint m_WVPLocation;
    GLuint m_LightWVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorMapLocation;
    GLuint m_shadowMapLocation;
    GLuint m_normalMapLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_numPointLightsLocation;
    GLuint m_numSpotLightsLocation;

    struct {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Direction;
    } m_dirLightLocation;

    struct {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      struct {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    struct {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      GLuint Direction;
      GLuint Cutoff;
      struct {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    } m_spotLightsLocation[MAX_SPOT_LIGHTS];
  };
}


#endif	/* LIGHTING_TECHNIQUE_H */
