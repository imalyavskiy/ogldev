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

#include <FreeImage.h>

#include "t16_texture.h"

namespace t16
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
  
  Texture::Texture(GLenum textureTarget, std::string textureFileName)
    : m_textureFileName(std::move(textureFileName))
    , m_textureTarget(textureTarget)
  {
    constexpr GLsizei numObjects = 1;
    glGenTextures(numObjects, &m_textureObj);
  }

  bool Texture::Load() const
  {
    const auto bitmap = GenericLoader(m_textureFileName.c_str(), 0);
    if (!bitmap)
      return false;
    if (const auto type = FreeImage_GetColorType(bitmap); type != FIC_RGB)
      return false;
    if (!FreeImage_HasPixels(bitmap))
      return false;

    const auto textureWidth  = static_cast<GLsizei>(FreeImage_GetWidth(bitmap));
    const auto textureHeight = static_cast<GLsizei>(FreeImage_GetHeight(bitmap));
    const auto textureData = FreeImage_GetBits(bitmap);

    glBindTexture(m_textureTarget, m_textureObj);

    constexpr GLint level = 0; // LOD aka MIP level
    glTexImage2D(m_textureTarget, level, GL_RGB, textureWidth, textureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);

    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(bitmap);

    return true;
  }

  void Texture::ActivateAndBind(const GLenum textureUnit) const
  {
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
  }
}