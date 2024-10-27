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
    LightingTechnique();
    bool Init() override;

    void SetWVP(const Matrix4f* WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionLight& Light);

  private:
    GLuint m_WVPLocation;
    GLuint m_samplerLocation;
    GLuint m_dirLightColorLocation;
    GLuint m_dirLightAmbientIntensityLocation;
  };
}

#endif // LIGHTINGTECHNIQUE_H
