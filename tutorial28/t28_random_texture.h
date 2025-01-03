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

#ifndef RANDOM_TEXTURE_H
#define	RANDOM_TEXTURE_H

#include <cstdint>
#include <GL/glew.h>

namespace t28
{
  class RandomTexture
  {
  public:
    RandomTexture() = default;

    ~RandomTexture();
    
    bool InitRandomTexture(uint32_t size);

    void Bind(GLenum textureUnit);

  private:
    GLuint m_textureObj = 0;
  };
}
#endif	/* RANDOM_TEXTURE_H */

