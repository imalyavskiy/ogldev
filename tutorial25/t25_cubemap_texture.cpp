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

#include <iostream>
#include "t25_util.h"
#include "t25_cubemap_texture.h"
#include "t25_texture.h"

namespace t25
{
  static const GLenum facetTypes[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

  CubemapTexture::CubemapTexture(const std::string& directory,
                                 const std::string& posXFilename,
                                 const std::string& negXFilename,
                                 const std::string& posYFilename,
                                 const std::string& negYFilename,
                                 const std::string& posZFilename,
                                 const std::string& negZFilename)
  {
    std::string::const_iterator it = directory.end();
    --it;
    const std::string baseDir = (*it == '/') ? directory : directory + "/";
    
    m_fileNames[0] = baseDir + posXFilename;
    m_fileNames[1] = baseDir + negXFilename;
    m_fileNames[2] = baseDir + posYFilename;
    m_fileNames[3] = baseDir + negYFilename;
    m_fileNames[4] = baseDir + posZFilename;
    m_fileNames[5] = baseDir + negZFilename;
    
    m_textureObj = 0;
  }

  CubemapTexture::~CubemapTexture()
  {
    if (m_textureObj != 0) {
      glDeleteTextures(1, &m_textureObj);
    }
  }

  bool CubemapTexture::Load()
  {
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(facetTypes) ; i++) {
      FIBITMAP* src = GenericLoader(m_fileNames[i].c_str(), 0);
      if (!src)
        return false;

      const auto colorType = FreeImage_GetColorType(src);
      if (colorType != FIC_RGB)
        return false;

      if (!FreeImage_HasPixels(src))
        return false;

      FreeImage_FlipVertical(src);

      const GLsizei imgWidth = FreeImage_GetWidth(src);
      const GLsizei imgHeight = FreeImage_GetHeight(src);
      const auto bpp = FreeImage_GetBPP(src);
      const auto data = FreeImage_GetBits(src);

      glTexImage2D(facetTypes[i], 0, GL_RGB, imgWidth, imgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

      FreeImage_Unload(src);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return true;
  }


  void CubemapTexture::Bind(GLenum TextureUnit)
  {
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
  }
}
