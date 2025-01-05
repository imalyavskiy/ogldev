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
#include "t44_texture.h"

namespace t44 {
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


  Texture::Texture(GLenum TextureTarget, const std::string& FileName)
    : m_fileName(std::move(FileName))
    , m_textureTarget(TextureTarget)
  {
    glGenTextures(1, &m_textureObj);
  }

  bool Texture::Load() const
  {
    bool result = false;

    const auto src = GenericLoader(m_fileName.c_str(), 0);
    if (!src)
      return result;

    const auto type = FreeImage_GetColorType(src);
    const auto width = FreeImage_GetWidth(src);
    const auto height = FreeImage_GetHeight(src);
    const auto data = FreeImage_GetBits(src);

    FreeImage_FlipVertical(src);

    if (!FreeImage_HasPixels(src))
      return result;

    glBindTexture(m_textureTarget, m_textureObj);

    if (FIC_RGB == type) {
      glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
      result = true;
    }
    else if (FIC_RGBALPHA == type) {
      glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
      result = true;
    }

    if (result) {
      glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    FreeImage_Unload(src);

    return result;
  }

  void Texture::Bind(const GLenum TextureUnit) const
  {
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
  }
}