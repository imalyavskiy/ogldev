#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#include "t19_technique.h"
#include "t19_math_3d.h"
#include "t19_math_3d.h"
namespace t19
{
  struct DirectionLight
  {
    Vector3f Color;
    float AmbientIntensity;
    Vector3f Direction;
    float DiffuseIntensity;
  };

  class LightingTechnique : public Technique
  {
  public:
    LightingTechnique() = default;
    bool Init() override;

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionLight& Light);

    void SetEyeWorldPos(const Vector3f& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);

  private:
    GLint m_WVPLocation = 0xFFFFFFFF;
    GLint m_WorldMatrixLocation = 0xFFFFFFFF;
    GLint m_samplerLocation = 0xFFFFFFFF;

    GLint m_eyeWorldPosition = 0xFFFFFFFF;
    GLint m_matSpecularIntensityLocation = 0xFFFFFFFF;
    GLint m_matSpecularPowerLocation = 0xFFFFFFFF;

    struct {
      GLint Color = 0xFFFFFFFF;
      GLint AmbientIntensity = 0xFFFFFFFF;
      GLint Direction = 0xFFFFFFFF;
      GLint DiffuseIntensity = 0xFFFFFFFF;
    } m_dirLightLocation;
  };
}

#endif // LIGHTINGTECHNIQUE_H
