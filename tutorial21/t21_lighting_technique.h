#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include <array>
#include "t21_technique.h"
#include "t21_math_3d.h"

namespace t21
{
  struct BaseLight
  {
    Vector3f Color{ 0.0f, 0.0f, 0.0f };
    float AmbientIntensity = 0.0f;
    float DiffuseIntensity = 0.0f;
  };

  struct DirectionalLight : public BaseLight
  {
    Vector3f Direction{ 0.0f, 0.0f, 0.0f };
  };

  struct AttenuationT
  {
    float Constant = 1.0f;
    float Linear = 0.0;
    float Exp = 0.0f;
  };

  struct PointLight : public BaseLight
  {
    Vector3f Position{ 0.0f, 0.0f, 0.0f };

    AttenuationT Attenuation{1.0, 0.0, 0.0};
  };

  struct SpotLight : public PointLight
  {
    Vector3f Direction{ 0.0f, 0.0f, 0.0f };
    float Cutoff = 0.0f;
  };

  class LightingTechnique
  : public Technique
  {
  public:

    static constexpr uint32_t MAX_POINT_LIGHTS = 2;
    static constexpr uint32_t MAX_SPOT_LIGHTS = 2;

    LightingTechnique() = default;

    bool Init() override;

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetTextureUnit(uint32_t textureUnit);
    void SetDirectionalLight(const DirectionalLight& light);
    void SetPointLights(uint32_t NumLights, const PointLight* pLights);
    void SetSpotLights(uint32_t numLights, const SpotLight* pLights);
    void SetEyeWorldPos(const Vector3f& EyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);

  private:

    struct DirectionLightLocation {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Direction;
    };
    
    struct PointLightLocations {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      struct {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    };

    struct SpotLightLocations {
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
    };

    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_samplerLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_numPointLightsLocation;
    GLuint m_numSpotLightsLocation;

    DirectionLightLocation m_dirLightLocation;

    std::array<PointLightLocations, MAX_POINT_LIGHTS> m_pointLightsLocation;

    std::array<SpotLightLocations, MAX_SPOT_LIGHTS> m_spotLightsLocation;
  };
}

#endif	/* LIGHTING_TECHNIQUE_H */
