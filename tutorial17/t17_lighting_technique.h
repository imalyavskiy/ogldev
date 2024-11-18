#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#include "t17_technique.h"
#include "t17_math_3d.h"
namespace t17
{
  struct DirectionLight
  {
    Vector3f Color;
    float AmbientIntensity;
  };

  class LightingTechnique : public Technique
  {
  public:
    LightingTechnique() = default;
    bool Init() override;

    void SetWVP(const Matrix4f* WVP);
    void SetTextureUnit(unsigned int textureUnit);
    void SetDirectionalLight(const DirectionLight& light);

  private:
    GLuint m_WVPLocation = 0xFFFFFFFF;
    GLuint m_samplerLocation = 0xFFFFFFFF;
    GLuint m_dirLightColorLocation = 0xFFFFFFFF;
    GLuint m_dirLightAmbientIntensityLocation = 0xFFFFFFFF;
  };
}

#endif // LIGHTINGTECHNIQUE_H
