#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

#include "t23_technique.h"
#include "t23_math_3d.h"

namespace t23
{
  class ShadowMapTechnique : public Technique {

  public:
    ShadowMapTechnique();
    virtual bool Init();
    void SetWVP(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);

  private:

    GLuint m_WVPLocation;
    GLuint m_textureLocation;
  };
}

#endif	/* SHADOW_MAP_TECHNIQUE_H */
