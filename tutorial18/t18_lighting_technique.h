#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#include "t18_technique.h"
#include "t18_math_3d.h"
namespace t18
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
    void SetTextureUnit(uint32_t TextureUnit);
    void SetDirectionalLight(const DirectionLight& Light);

  private:
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_samplerLocation;
    struct {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint Direction;
      GLuint DiffuseIntensity;
    } m_dirLightLocation;
  };
}

#endif // LIGHTINGTECHNIQUE_H
