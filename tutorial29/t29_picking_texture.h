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

#ifndef __SHADOW_MAP_FBO_H__
#define	__SHADOW_MAP_FBO_H__

#include <cstdint>
#include <GL/glew.h>

namespace t29
{
  class PickingTexture
  {
  public:
    PickingTexture() = default;

    ~PickingTexture();

    bool Init(uint32_t winWidth, uint32_t winHeight);

    void EnableWriting();
    
    void DisableWriting();
    
    struct PixelInfo {
      uint32_t ObjectID = 0;
      uint32_t DrawID = 0;
      uint32_t PrimID = 0;
    };

    PixelInfo ReadPixel(uint32_t x, uint32_t y);

  private:
    GLuint m_fbo = 0;
    GLuint m_pickingTexture = 0;
    GLuint m_depthTexture = 0;
  };
}
#endif	// __SHADOW_MAP_FBO_H__

