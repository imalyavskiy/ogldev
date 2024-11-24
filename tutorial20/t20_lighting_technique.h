#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#include <array>

#include "t20_camera.h"
#include "t20_texture.h"
#include "t20_technique.h"
#include "t20_math_3d.h"

#define MAX_POINT_LIGHTS 3

namespace t20
{
  struct AttenuationT
  {
    float constant = 0.f;
    float linear = 0.f;
    float exp = 0.f;
  };

  struct BaseLight
  {
    Vector3f color;
    float ambientIntensity = 0.f;
    float diffuseIntensity = 0.f;
  };

  struct DirectionalLight : public BaseLight
  {
    Vector3f direction;
  };

  struct PointLight : public BaseLight
  {
    Vector3f position;

    AttenuationT attenuation{1.f, 0.f, 0.f};
  };

  class LightingTechnique : public Technique
  {
  public:
    LightingTechnique() = default;

    bool Init() override;

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& world);
    void SetTextureUnit(unsigned int textureUnit);
    void SetDirectionalLight(const DirectionalLight& light);

    void SetEyeWorldPos(const Vector3f& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);

    void SetPointLights(unsigned int numLights, const PointLight* pLights);

  private:
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_samplerLocation;

    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_numPointLightsLocation;

    struct LightLocation {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint Direction;
      GLuint DiffuseIntensity;
    };

    LightLocation m_dirLightLocation;

    struct PointLightLocation {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;

      struct Attenuation {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      };

      Attenuation attenuation;
    };

    std::array<PointLightLocation, 3> m_pointLightsLocation;
  };
}

#endif // LIGHTINGTECHNIQUE_H
