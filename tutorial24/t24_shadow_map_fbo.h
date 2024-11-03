#ifndef SHADOWMAPFBO_H
#define SHADOWMAPFBO_H

#include <GL/glew.h>

namespace t24
{
  class ShadowMapFBO
  {
  public:
    ShadowMapFBO() = default;

    ~ShadowMapFBO();

    bool Init(unsigned int winWidth, unsigned int winHeight);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

  private:
    GLuint m_FrameBufferObject = 0;
    GLuint m_ShadowMapTexture = 0;
  };
}

#endif /* SHADOWMAPFBO_H */
