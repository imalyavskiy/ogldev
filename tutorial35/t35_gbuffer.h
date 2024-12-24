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

#include <array>
#include <GL/glew.h>

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3

namespace t35
{
  // Geometry Buffer
  class GBuffer
  {
  public:

    enum GBUFFER_TEXTURE_TYPE {
      GBUFFER_TEXTURE_TYPE_POSITION = 0,
      GBUFFER_TEXTURE_TYPE_DIFFUSE  = 1,
      GBUFFER_TEXTURE_TYPE_NORMAL   = 2,
      GBUFFER_TEXTURE_TYPE_TEXCOORD = 3,
      GBUFFER_NUM_TEXTURES          = 4,
    };

    GBuffer() = default;

    ~GBuffer();

    bool Init(uint32_t winWidth, uint32_t winHeight);

    void BindForWriting();

    void BindForReading();

    void SetReadBuffer(GBUFFER_TEXTURE_TYPE textureType);

  private:

    GLuint m_fbo = 0;
    std::array<GLuint, GBUFFER_NUM_TEXTURES> m_textures = {};
    GLuint m_depthTexture = 0;
  };
}
#endif	/* SHADOWMAPFBO_H */

