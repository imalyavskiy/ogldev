#include <cstdio>

#include "t23_shadow_map_fbo.h"
namespace t23
{
  ShadowMapFBO::~ShadowMapFBO()
  {
    if (m_FrameBufferObject != 0)
      glDeleteFramebuffers(1, &m_FrameBufferObject);

    if (m_ShadowMapTexture != 0)
      glDeleteFramebuffers(1, &m_ShadowMapTexture);
  }

  bool ShadowMapFBO::Init(GLuint winWidth, GLuint winHeight)
  {
    glGenFramebuffers(1, &m_FrameBufferObject);

    glGenTextures(1, &m_ShadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, winWidth, winHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBufferObject);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapTexture, 0);

    glDrawBuffer(GL_NONE);

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE){
      printf("FB error, status: 0x%x\n", status);
      return false;
    }

    return true;
  }

  void ShadowMapFBO::BindForWriting()
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBufferObject);
  }

  void ShadowMapFBO::BindForReading(GLenum textureUnit)
  {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapTexture);
  }
}
