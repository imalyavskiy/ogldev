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

    void SetEyeWorldPos(const Vector3f& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);

  private:
    GLint m_WVPLocation              = -1; // 0xFFFFFFFF
    GLint m_WorldMatrixLocation      = -1; // 0xFFFFFFFF
    GLint m_samplerLocation          = -1; // 0xFFFFFFFF

    GLint m_eyeWorldPosition         = -1; // 0xFFFFFFFF
    GLint m_matSpecularIntensityLocation = -1; // 0xFFFFFFFF
    GLint m_matSpecularPowerLocation = -1; // 0xFFFFFFFF

    struct {
      GLint Color                    = -1; // 0xFFFFFFFF
      GLint AmbientIntensity         = -1; // 0xFFFFFFFF
      GLint Direction                = -1; // 0xFFFFFFFF
      GLint DiffuseIntensity         = -1; // 0xFFFFFFFF
    } m_dirLightLocation;
  };
}

#endif // LIGHTINGTECHNIQUE_H
