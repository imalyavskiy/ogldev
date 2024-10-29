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

#include <cassert>
#include <iostream>

#include <FreeImage.h>

#include "t20_texture.h"
namespace t20
{
  FIBITMAP* GenericLoader(const char* lpszPathName, int flag) {
    auto fif = FIF_UNKNOWN;

    fif = FreeImage_GetFileType(lpszPathName, 0);
    if (fif == FIF_UNKNOWN) {
      fif = FreeImage_GetFIFFromFilename(lpszPathName);
    }

    if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
      return FreeImage_Load(fif, lpszPathName, flag);
    }

    return nullptr;
  }


  Texture::Texture(GLenum TextureTarget, std::string FileName)
      : m_fileName(std::move(FileName))
      , m_textureTarget(TextureTarget)
  {
    glGenTextures(1, &m_textureObj);
  }

  bool Texture::Load() const
  {
    const auto src = GenericLoader(m_fileName.c_str(), 0);
    if (!src)
      return false;

    const auto type = FreeImage_GetColorType(src);
    if (type != FIC_RGB)
      return false;

    if (!FreeImage_HasPixels(src))
      return false;

    const auto width = FreeImage_GetWidth(src);
    const auto height = FreeImage_GetHeight(src);

    const auto data = FreeImage_GetBits(src);

    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, -0.5, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(src);

    return true;
  }

  void Texture::Bind(const GLenum TextureUnit) const
  {
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
  }
}
