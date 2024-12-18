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

#ifndef GBUFFER_H
#define	GBUFFER_H

#include <cstdint>
#include <GL/glew.h>

namespace t37
{
  class GBuffer
  {
  public:

    enum GBUFFER_TEXTURE_TYPE {
      GBUFFER_TEXTURE_TYPE_POSITION,
      GBUFFER_TEXTURE_TYPE_DIFFUSE,
      GBUFFER_TEXTURE_TYPE_NORMAL,
      GBUFFER_NUM_TEXTURES
    };

    GBuffer() = default;

    ~GBuffer();

    bool Init(uint32_t winWidth, uint32_t winHeight);

    void StartFrame();
    void BindForGeomPass();
    void BindForStencilPass();
    void BindForLightPass();
    void BindForFinalPass();

  private:

    GLuint m_fbo = 0;
    GLuint m_textures[GBUFFER_NUM_TEXTURES] = {};
    GLuint m_depthTexture = 0;
    GLuint m_finalTexture = 0;
  };
}
#endif	/* SHADOWMAPFBO_H */

