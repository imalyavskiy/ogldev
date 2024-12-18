/*
        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>

#include "t37_util.h"
#include "t37_gbuffer.h"

namespace t37
{
  GBuffer::~GBuffer()
  {
    if (m_fbo != 0)
      glDeleteFramebuffers(1, &m_fbo);

    if (m_textures[0] != 0)
      glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);

    if (m_depthTexture != 0)
      glDeleteTextures(1, &m_depthTexture);

    if (m_finalTexture != 0)
      glDeleteTextures(1, &m_finalTexture);
  }

  bool GBuffer::Init(unsigned int winWidth, unsigned int winHeight)
  {
    // Create the FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    // Create the G-Buffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);

    glGenTextures(1, &m_depthTexture);

    glGenTextures(1, &m_finalTexture);

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) 
    {
      glBindTexture(GL_TEXTURE_2D, m_textures[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, winWidth, winHeight, 0, GL_RGB, GL_FLOAT, nullptr);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, winWidth, winHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // final
    glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, winWidth, winHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
      printf("FB error, status: 0x%x\n", status);
      return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
  }


  void GBuffer::StartFrame()
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
  }


  void GBuffer::BindForGeomPass()
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    constexpr GLenum drawBuffers[] = {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2
    };

    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawBuffers), drawBuffers);
  }


  void GBuffer::BindForStencilPass()
  {
    // must disable the draw buffers 
    glDrawBuffer(GL_NONE);
  }



  void GBuffer::BindForLightPass()
  {
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
    }
  }


  void GBuffer::BindForFinalPass()
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
  }
}