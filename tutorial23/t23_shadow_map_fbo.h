#ifndef SHADOWMAPFBO_H
#define SHADOWMAPFBO_H

#include <GL/glew.h>
namespace t23
{
  class ShadowMapFBO
  {
  public:
    ShadowMapFBO() = default;

    ~ShadowMapFBO();

    bool Init(GLuint winWidth, GLuint winHeight);

    void BindForWriting();

    void BindForReading(GLenum textureUnit);

  private:
    GLuint m_FrameBufferObject = 0;
    GLuint m_ShadowMapTexture = 0;
  };
}

#endif /* SHADOWMAPFBO_H */
