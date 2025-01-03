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

#include "t28_random_texture.h"
#include "t28_math_3d.h"
#include "t28_util.h"

namespace t28
{
  RandomTexture::~RandomTexture()
  {
    if (m_textureObj != 0)
      glDeleteTextures(1, &m_textureObj);
  }

  bool RandomTexture::InitRandomTexture(uint32_t size)
  {
    const auto pRandomData = new Vector3f[size];
    for (uint32_t i = 0 ; i < size ; i++) {
      pRandomData[i].x = RandomFloat();
      pRandomData[i].y = RandomFloat();
      pRandomData[i].z = RandomFloat();
    }

    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0.0f, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    
    delete[] pRandomData;
    
    return GLCheckError();
  }

  void RandomTexture::Bind(GLenum textureUnit)
  {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
  }
}
