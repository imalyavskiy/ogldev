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

#include "t44_technique.h"
#include "t44_math_3d.h"

namespace t44 {
  struct BaseLight
  {
    Vector3f Color = { 0.0f, 0.0f, 0.0f };
    float AmbientIntensity = 0.f;
    float DiffuseIntensity = 0.f;
  };

  struct DirectionalLight : public BaseLight
  {
    Vector3f Direction = { 0.0f, 0.0f, 0.0f };
  };

  struct PointLight : public BaseLight
  {
    Vector3f Position = { 0.0f, 0.0f, 0.0f };

    struct {
      float Constant;
      float Linear;
      float Exp;
    } Attenuation = { 1.f, 0.f, 0.f };
  };

  struct SpotLight : public PointLight
  {
    Vector3f Direction = { 0.0f, 0.0f, 0.0f };
    float Cutoff = 0.f;
  };

  class LightingTechnique : public Technique {
  public:

    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;

    LightingTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& worldViewProj);
    void SetWorldMatrix(const Matrix4f& world);
    void SetColorTextureUnit(unsigned int textureUnit);
    void SetDirectionalLight(const DirectionalLight& light);
    void SetPointLights(unsigned int numLights, const PointLight* pLights);
    void SetSpotLights(unsigned int numLights, const SpotLight* pLights);
    void SetEyeWorldPos(const Vector3f& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);

  private:

    GLuint m_WVPLocation;
    GLuint m_worldMatrixLocation;
    GLuint m_colorTextureLocation;
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
