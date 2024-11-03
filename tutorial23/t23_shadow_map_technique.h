#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

#include "t23_technique.h"
#include "t23_math_3d.h"

namespace t23
{
  class ShadowMapTechnique
    : public Technique
  {
  public:
    ShadowMapTechnique() = default;

    bool Init() override;

    void SetWVP(const Matrix4f& WVP);

    void SetTextureUnit(unsigned int TextureUnit);

  private:

    GLuint m_WVPLocation = INVALID_UNIFORM_LOCATION;
    GLuint m_textureLocation = INVALID_UNIFORM_LOCATION;
  };
}

#endif	/* SHADOW_MAP_TECHNIQUE_H */
