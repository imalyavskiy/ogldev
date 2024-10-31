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
  struct BaseLight
  {
    Vector3f Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight();
  };

  struct DirectionalLight : public BaseLight
  {
    Vector3f Direction;

    DirectionalLight();
  };

  struct PointLight : public BaseLight
  {
    Vector3f Position;

    struct AttenuationT
    {
      float Constant;
      float Linear;
      float Exp;
    };

    AttenuationT Attenuation;

    PointLight();
  };

  class LightingTechnique : public Technique
  {
  public:
    LightingTechnique();
    bool Init() override;

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WorldInverse);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionalLight& Light);

    void SetEyeWorldPos(const Vector3f& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);

    void SetPointLights(unsigned int NumLights, const PointLight* pLights);

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
